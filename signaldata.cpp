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
