/*
 * charter
 * Copyright (C) 2011 Michał Janiszewski
 *
 * This work is a thesis. Due to nature of law you are obliged to
 * get a written permission from Technical University of Lodz prior to
 * using any part of this work. If you do get a permission, you are able
 * to use the work, which is thereafter licensed using GNU GPL v3 or
 * (at your option) any later.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "samplingthread.h"
#include <QFile>
#include <QMessageBox>
#include <QSerialPort>
#include <cmath>
#include <QMatrix4x4>
#include <QMetaEnum>

#include <QDebug>

SamplingThread::SamplingThread(QObject *parent) :
	QwtSamplingThread(parent),
	mpSerport(NULL)
{
	qDebug() << "SamplingThread ctor" << this;
	this->setInterval(25);

	const QMetaObject metaObject = Sample::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("Marker"));
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		mMarkers.append((Sample::Marker)metaEnum.value(i));
	}
}

SamplingThread::~SamplingThread()
{
	qDebug() << "SamplingThread dtor" << this;
}

void SamplingThread::sample(double elapsed)
{
	//qDebug() << "sampling" << elapsed;
	qint64 avail = 16;//mpSerport->bytesAvailable();
	QByteArray readData;
	readData.reserve(avail);
	//mpSerport->errorString();
	qint64 bytesRead = mpSerport->read(readData.data(), avail);
	//qDebug() << "Read" << bytesRead << "bytes of available" << avail;
	if (bytesRead != avail) {
		//qWarning() << "Warning! Read" << bytesRead << "bytes instead of" <<
		//			  avail << "bytes available.";
	}
	if (bytesRead == -1) {
		qWarning() << "No further data can be read. Stopping sampler.";
		stop();
		return;
	}
	readData.resize(bytesRead);
	append(readData, elapsed);
}

bool SamplingThread::open(QString fileName, QPortSettings::BaudRate baudRate)
{
	qDebug() << "Opening" << fileName << fileName.toLatin1();
	if (mpSerport == NULL) {
		QPortSettings settings;
		settings.setBaudRate(baudRate);
		settings.setDataBits(QPortSettings::DB_8);
		settings.setFlowControl(QPortSettings::FLOW_OFF);
		settings.setParity(QPortSettings::PAR_NONE);
		settings.setStopBits(QPortSettings::STOP_1);
		qDebug() << "settings:" << settings.toString();
		mpSerport = new QSerialPort(fileName, settings);
		if (!mpSerport->open()) {
			qDebug() << "failed to open serial port" << fileName << "("
					 << mpSerport->errorString() << ")";
			emit error(QString("Failed to open serial port %1 (%2)")
					   .arg(fileName, mpSerport->errorString()));
			// cleanup
			delete mpSerport;
			// allow another attempt to open to be made
			mpSerport = NULL;
			return false;
		} else {
			qDebug() << "port" << fileName << "successfully opened";
		}
		if (!mpSerport->setCommTimeouts(QSerialPort::CtScheme_NonBlockingRead)) {
			qWarning("Cannot set communications timeout values at port %s.", qPrintable(fileName));
		}
		return true;
	} else {
		emit error(QString("Already opened serial port %1")
				   .arg(mpSerport->portName()));
		return false;
	}
}

void SamplingThread::append(Sample mySample)
{
	QMutexLocker locker(&mutex);
	samples.append(mySample);
	emit dataArrived();
}

bool SamplingThread::isValidSample(const char c) const
{
	return mMarkers.contains((Sample::Marker)c);
}

void SamplingThread::append(const QByteArray &data, double elapsed)
{
	QMutexLocker locker(&mutex);
	mTempData.append(data);
	while (mTempData.size() >= 8) {
		// skip any leading malformed data
		while (mTempData.size() >= 8 && ((mTempData.at(7) != (char)0xFF) || !isValidSample(mTempData.at(0)))) {
			//qDebug() << "****************** removing data";
			mTempData.remove(0, 1);
		}
		if (mTempData.size() < 8) {
			break;
		}
		// 8MHz with prescaler clk/8, premultiplied by ovfCounter
		float up = 256 * (unsigned char)mTempData.at(2) + (unsigned char)mTempData.at(1);
		float right = 256 * (unsigned char)mTempData.at(4) + (unsigned char)mTempData.at(3);
		float left = 256 * (unsigned char)mTempData.at(6) + (unsigned char)mTempData.at(5);
		Sample mySample;

		mySample.marker = (Sample::Marker)mTempData.at(0);

		// multiplied by speed of sound in air and divided by 100cm/m
		// results in distance in cm from receiver
		mySample.left = left * 333 / 10000;
		mySample.right = right * 333 / 10000;
		mySample.up = up * 333 / 10000;
		mySample.time = elapsed;

		// distance on x axis of right receiver
		float d = 33;
		// distance on x axis of up receiver
		float i = 16.5f;
		// distance on y axis of 2nd receiver (sqrt(3)/2 * d)
		float j = 28.5788383f;

		qreal x = (pow(mySample.left, 2) - pow(mySample.right, 2) + pow(d, 2)) / (2 * d);
		qreal y = ((pow(mySample.left, 2) - pow(mySample.up, 2) + pow(i, 2) + pow(j, 2)) / (2 * j)) - ((i * x) / j);
		qreal z = sqrt(pow(mySample.left, 2) - pow(x, 2) - pow(y, 2));

		QMatrix4x4 rot;
		rot.rotate(60, 0, 0, 1);

		QVector3D pos(-x, -y, -z);
		mySample.pos = pos * rot;

		//qDebug() << mySample.marker << mTempData.left(8).toHex();
		//qDebug() << "Sample(" << mySample.left << "," << mySample.right << "," <<
		//			mySample.up << "," << mySample.time << ")";

		samples << mySample;
		mTempData.remove(0, 8);
	}
	emit dataArrived();
}

QVector<Sample> SamplingThread::takeSamples()
{
	QMutexLocker locker(&mutex);
	QVector<Sample> result = samples.mid(0);
	samples.clear();
	return result;
}
