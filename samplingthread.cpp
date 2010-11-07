#include "samplingthread.h"

SamplingThread::SamplingThread(QObject *parent) :
	QwtSamplingThread(parent)
{
}

void SamplingThread::sample(double /*elapsed*/)
{
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
	append(mySample);
}

void SamplingThread::append(Sample mySample)
{
	QMutexLocker locker(&mutex);
	samples.append(mySample);
}

QVector<Sample> SamplingThread::takeSamples()
{
	QMutexLocker locker(&mutex);
	QVector<Sample> result = samples.mid(0);
	samples.clear();
	return result;
}
