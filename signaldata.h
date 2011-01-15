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

class SignalData : public QObject
{
	Q_OBJECT
public:
	static SignalData &instance();

	int size(Sample::Marker which) const;
	Sample value(Sample::Marker which, int index) const;
	QRectF boundingRect(Sample::Marker which) const;
	void wait();

signals:
	void started();
	void finished();
	void error(QString);

public slots:
	void fetchSamples();

private:
	SignalData();
	~SignalData();
	QHash<Sample::Marker, QVector<Sample> > mSamples;
	QHash<Sample::Marker, QRectF> mBoundingRects;
	mutable QMutex mMutex;
	SamplingThread mSampler;

public slots:
	void start(QString portName, QPortSettings::BaudRate baudRate);
	void stop();
};

#endif // SIGNALDATA_H
