#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <QObject>
#include <QRectF>
#include <QVector>
#include <QMutex>
#include <QSerialPort>

#include "sample.h"
#include "samplingthread.h"
#include "positiondatax.h"

class SignalData : QObject
{
	Q_OBJECT
public:
	static SignalData &instance();

	int size(Marker which) const;
	Sample value(Marker which, int index) const;
	QRectF boundingRect(Marker which) const;
	void wait();

public slots:
	void fetchSamples();

private:
	SignalData();
	~SignalData();
	QVector<Sample> mBlueSamples, mYellowSamples;
	QRectF mBlueBoundingRect, mYellowBoundingRect;
	mutable QMutex mMutex;
	SamplingThread mSampler;

public slots:
	void start(QString portName, QPortSettings::BaudRate baudRate);
	void stop();
};

#endif // SIGNALDATA_H
