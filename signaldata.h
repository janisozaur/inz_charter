#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <QObject>
#include <QRectF>
#include <QVector>
#include <QMutex>

#include "sample.h"
#include "samplingthread.h"

class SignalData : QObject
{
	Q_OBJECT
public:
	SignalData();

	int size() const;
	Sample value(int index) const;
	QRectF boundingRect() const;

public slots:
	void fetchSamples();

private:
	QVector<Sample> mSamples;
	QRectF mBoundingRect;
	mutable QMutex mMutex;
	SamplingThread mSampler;

public slots:
	void start();
};

#endif // SIGNALDATA_H
