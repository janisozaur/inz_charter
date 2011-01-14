#include "samplingthread.h"
#include <QFile>
#include <QMessageBox>
#include <QSerialPort>
#include <cmath>
#include <QMatrix4x4>

#include <QDebug>

SamplingThread::SamplingThread(QObject *parent) :
	QwtSamplingThread(parent),
	mpSerport(NULL)
{
	qDebug() << "SamplingThread ctor" << this;
	this->setInterval(25);
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

void SamplingThread::open(QString fileName, QPortSettings::BaudRate baudRate)
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
			qDebug() << "failed to open serial port" << fileName;
		} else {
			qDebug() << "port" << fileName << "successfully opened";
		}
		if (!mpSerport->setCommTimeouts(QSerialPort::CtScheme_NonBlockingRead)) {
			qWarning("Cannot set communications timeout values at port %s.", qPrintable(fileName));
		}
	}
}

void SamplingThread::append(Sample mySample)
{
	QMutexLocker locker(&mutex);
	samples.append(mySample);
	emit dataArrived();
}

void SamplingThread::append(const QByteArray &data, double elapsed)
{
	QMutexLocker locker(&mutex);
	mTempData.append(data);
	while (mTempData.size() >= 8) {
		// skip any leading malformed data
		while (mTempData.size() >= 8 && (!(mTempData.at(0) == Yellow || mTempData.at(0) == Blue) || mTempData.at(7) != (char)0xFF)) {
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

		mySample.marker = (Marker)mTempData.at(0);

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
