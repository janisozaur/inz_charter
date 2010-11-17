#include "signaldata.h"

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
}

SignalData::~SignalData()
{
	qDebug() << "SignalData dtor" << this;
	// TODO: stop sampling thread
}

Sample SignalData::value(int index) const
{
	QMutexLocker locker(&mMutex);
	//qDebug() << "asking for value of sample" << index;
	return mSamples[index];
}

int SignalData::size() const
{
	QMutexLocker locker(&mMutex);
	return mSamples.size();
}

QRectF SignalData::boundingRect() const
{
	QMutexLocker locker(&mMutex);
	return mBoundingRect;
}

void SignalData::fetchSamples()
{
	QMutexLocker locker(&mMutex);
	//qDebug() << "fetching samples";
	QVector<Sample> newSamples = mSampler.takeSamples();
	for (int i = 0; i < newSamples.count(); i++) {
		const Sample mySample = newSamples.at(i);
		qreal y = qMax(mySample.x, qMax(mySample.y, mySample.z));
		if (mBoundingRect.isNull()) {
			mBoundingRect.setRect(mySample.time, y, 0.0, 0.0);
		} else {
			mBoundingRect.setRight(mySample.time);
			if (y > mBoundingRect.bottom()) {
				mBoundingRect.setBottom(y);
			}
			if (y < mBoundingRect.top()) {
				mBoundingRect.setTop(y);
			}
		}
	}
	mSamples << newSamples;
}

void SignalData::start(QString portName, QPortSettings::BaudRate baudRate)
{
	qDebug() << "starting sampler";
	mSampler.open(portName, baudRate);
	mSampler.start();
}
