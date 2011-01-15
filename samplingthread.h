#ifndef SAMPLINGTHREAD_H
#define SAMPLINGTHREAD_H

#include <QVector>
#include <QMutexLocker>
#include <qwt_sampling_thread.h>
#include <QFile>
#include <QSerialPort>

using namespace TNX;

#include "sample.h"

class SamplingThread : public QwtSamplingThread
{
	Q_OBJECT
public:
	explicit SamplingThread(QObject *parent = 0);
	~SamplingThread();
	QVector<Sample> takeSamples();
	bool open(QString fileName, QPortSettings::BaudRate baudRate);
	void close();

protected:
	virtual void sample(double elapsed);
	void append(Sample mySample);
	void append(const QByteArray &data, double elapsed);
	bool isValidSample(const char c) const;

signals:
	void dataArrived();
	void error(QString);

private:
	QVector<Sample> samples;
	QByteArray mTempData;
	QMutex mutex;
	QSerialPort *mpSerport;
	QVector<Sample::Marker> mMarkers;

signals:

public slots:

};

#endif // SAMPLINGTHREAD_H
