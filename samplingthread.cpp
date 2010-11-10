#include "samplingthread.h"

#include <QDebug>

SamplingThread::SamplingThread(QObject *parent) :
	QwtSamplingThread(parent)
{
	qDebug() << "SamplingThread ctor" << this;
}

SamplingThread::~SamplingThread()
{
	qDebug() << "SamplingThread dtor" << this;
}

void SamplingThread::sample(double elapsed)
{
	qDebug() << "sampling" << elapsed;
	Sample mySample;
	static int prevX = 0;
	static int prevY = 0;
	static int prevZ = 0;
	prevX += rand() % 10 - 5;
	prevY += rand() % 10 - 5;
	prevZ += rand() % 10 - 5;
	mySample.x = prevX;
	mySample.y = prevY;
	mySample.z = prevZ;
	mySample.time = (int)elapsed;
	append(mySample);
}

void SamplingThread::append(Sample mySample)
{
	QMutexLocker locker(&mutex);
	samples.append(mySample);
	emit dataArrived();
}

QVector<Sample> SamplingThread::takeSamples()
{
	QMutexLocker locker(&mutex);
	QVector<Sample> result = samples.mid(0);
	samples.clear();
	return result;
}
