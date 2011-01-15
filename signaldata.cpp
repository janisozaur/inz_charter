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
	connect(&mSampler, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
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
	switch (which) {
		case Sample::Yellow:
			return mYellowSamples.at(index);
			break;
		case Sample::Blue:
			return mBlueSamples.at(index);
			break;
		default:
			qFatal("SignalData::value: there is no marker %d.\n", which);
	}
	return Sample(); // never reached, just shut up compile warnings
}

int SignalData::size(Sample::Marker which) const
{
	QMutexLocker locker(&mMutex);
	switch (which) {
		case Sample::Yellow:
			return mYellowSamples.size();
			break;
		case Sample::Blue:
			return mBlueSamples.size();
			break;
		default:
			qFatal("SignalData::size: there is no marker %d.\n", which);
	}
	return 0; // never reached, just shut up some warnings
}

QRectF SignalData::boundingRect(Sample::Marker which) const
{
	QMutexLocker locker(&mMutex);
	QRectF result;
	switch (which) {
		case Sample::Yellow:
			result = mYellowBoundingRect;
			break;
		case Sample::Blue:
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
			case Sample::Yellow:
				br = &mYellowBoundingRect;
				mYellowSamples << mySample;
				break;
			case Sample::Blue:
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
