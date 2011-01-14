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
	connect(&mSampler, SIGNAL(started()), this, SIGNAL(started()));
	connect(&mSampler, SIGNAL(finished()), this, SIGNAL(finished()));
	connect(&mSampler, SIGNAL(terminated()), this, SIGNAL(finished()));
}

SignalData::~SignalData()
{
	qDebug() << "SignalData dtor" << this;
	// TODO: stop sampling thread
}

Sample SignalData::value(Marker which, int index) const
{
	QMutexLocker locker(&mMutex);
	//qDebug() << "asking for value of sample" << index;
	//return mSamples[index];
	switch (which) {
		case Yellow:
			return mYellowSamples.at(index);
			break;
		case Blue:
			return mBlueSamples.at(index);
			break;
		default:
			qFatal("SignalData::value: there is no marker %d.\n", which);
	}
	return Sample(); // never reached, just shut up compile warnings
}

int SignalData::size(Marker which) const
{
	QMutexLocker locker(&mMutex);
	switch (which) {
		case Yellow:
			return mYellowSamples.size();
			break;
		case Blue:
			return mBlueSamples.size();
			break;
		default:
			qFatal("SignalData::size: there is no marker %d.\n", which);
	}
	return 0; // never reached, just shut up some warnings
}

QRectF SignalData::boundingRect(Marker which) const
{
	QMutexLocker locker(&mMutex);
	QRectF result;
	switch (which) {
		case Yellow:
			result = mYellowBoundingRect;
			break;
		case Blue:
			result = mBlueBoundingRect;
			break;
		default:
			qFatal("SignalData::boundingRect: there is no marker %d.\n", which);
	}
	return result;
}

void SignalData::fetchSamples()
{
	QMutexLocker locker(&mMutex);
	//qDebug() << "fetching samples";
	QVector<Sample> newSamples = mSampler.takeSamples();
	for (int i = 0; i < newSamples.count(); i++) {
		const Sample mySample = newSamples.at(i);
		qreal y = qMax(mySample.left, qMax(mySample.right, mySample.up));
		QRectF *br;
		switch (mySample.marker) {
			case Yellow:
				br = &mYellowBoundingRect;
				mYellowSamples << mySample;
				break;
			case Blue:
				br = &mBlueBoundingRect;
				mBlueSamples << mySample;
				break;
			default:
				br = 0;
				qFatal("SignalData::fetchSamples: there is no marker %d.\n", mySample.marker);
		}
		if (br->isNull()) {
			br->setRect(mySample.time, y, 0.0, 0.0);
		} else {
			br->setRight(mySample.time);
			if (y > br->bottom()) {
				br->setBottom(y);
			}
			if (y < br->top()) {
				br->setTop(y);
			}
		}
	}
}

void SignalData::start(QString portName, QPortSettings::BaudRate baudRate)
{
	qDebug() << "starting sampler";
	mSampler.open(portName, baudRate);
	mSampler.start();
}

void SignalData::stop()
{
	mSampler.stop();
}

void SignalData::wait()
{
	mSampler.wait();
}
