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

#include "signaldata.h"

#include <QMetaEnum>

#include <QDebug>

SignalData &SignalData::instance()
{
	static SignalData valueVector;
	return valueVector;
}

SignalData::SignalData()
{
	qDebug() << "SignalData ctor" << this;
	connect(&mSampler, SIGNAL(dataArrived()), SLOT(fetchSamples()));
	connect(&mSampler, SIGNAL(started()), this, SIGNAL(started()));
	connect(&mSampler, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(&mSampler, SIGNAL(terminated()), this, SIGNAL(finished()));
	connect(&mSampler, SIGNAL(error(QString)), this, SIGNAL(error(QString)));

	const QMetaObject metaObject = Sample::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("Marker"));
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		mSamples.insert((Sample::Marker)metaEnum.value(i), QVector<Sample>());
		mBoundingRects.insert((Sample::Marker)metaEnum.value(i), QRectF());
	}
}

SignalData::~SignalData()
{
	qDebug() << "SignalData dtor" << this;
	// TODO: stop sampling thread
}

Sample SignalData::value(Sample::Marker which, int index) const
{
	QMutexLocker locker(&mMutex);
	//qDebug() << "asking for value of sample" << index;
	//return mSamples[index];
	return mSamples.value(which).at(index);
}

int SignalData::size(Sample::Marker which) const
{
	QMutexLocker locker(&mMutex);
	return mSamples.value(which).size();
}

QRectF SignalData::boundingRect(Sample::Marker which) const
{
	QMutexLocker locker(&mMutex);
	return mBoundingRects.value(which);
}

void SignalData::fetchSamples()
{
	QMutexLocker locker(&mMutex);
	//qDebug() << "fetching samples";
	QVector<Sample> newSamples = mSampler.takeSamples();
	for (int i = 0; i < newSamples.count(); i++) {
		const Sample mySample = newSamples.at(i);
		const Sample::Marker which = mySample.marker;
		mSamples[which] << mySample;
		qreal y = qMax(mySample.left, qMax(mySample.right, mySample.up));
		if (mBoundingRects.value(which).isNull()) {
			mBoundingRects[which].setRect(mySample.time, y, 0.0, 0.0);
		} else {
			mBoundingRects[which].setRight(mySample.time);
			if (y > mBoundingRects.value(which).bottom()) {
				mBoundingRects[which].setBottom(y);
			}
			if (y < mBoundingRects.value(which).top()) {
				mBoundingRects[which].setTop(y);
			}
		}
	}
}

void SignalData::start(QString portName, QPortSettings::BaudRate baudRate)
{
	qDebug() << "starting sampler";
	if (mSampler.open(portName, baudRate)) {
		mSampler.start();
	}
}

void SignalData::stop()
{
	mSampler.stop();
}

void SignalData::wait()
{
	mSampler.wait();
}
