#include "signaldata.h"

SignalData::SignalData()
{
}

Sample SignalData::value(int index) const
{
	QMutexLocker locker(&mMutex);
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

void SignalData::start()
{
	mSampler.start();
}
