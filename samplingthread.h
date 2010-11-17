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
	void open(QString fileName = "/dev/ttyUSB0");
	void close();

protected:
	virtual void sample(double elapsed);
	void append(Sample mySample);
	void append(const QByteArray &data, double elapsed);

signals:
	void dataArrived();

private:
	QVector<Sample> samples;
	QByteArray mTempData;
	QMutex mutex;
	QSerialPort *mpSerport;

signals:

public slots:

};

#endif // SAMPLINGTHREAD_H
