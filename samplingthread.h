#ifndef SAMPLINGTHREAD_H
#define SAMPLINGTHREAD_H

#include <QVector>
#include <QMutexLocker>
#include <qwt_sampling_thread.h>

#include "sample.h"

class SamplingThread : public QwtSamplingThread
{
	Q_OBJECT
public:
	explicit SamplingThread(QObject *parent = 0);
	QVector<Sample> takeSamples();

protected:
	virtual void sample(double elapsed);
	void append(Sample mySample);

signals:
	void dataArrived();

private:
	QVector<Sample> samples;
	QMutex mutex;

signals:

public slots:

};

#endif // SAMPLINGTHREAD_H
