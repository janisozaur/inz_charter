#include "samplingthread.h"

#include <QDebug>

SamplingThread::SamplingThread(QObject *parent) :
	QwtSamplingThread(parent)
{
	qDebug() << "SamplingThread ctor" << this;
	this->setInterval(25);
}

SamplingThread::~SamplingThread()
{
	qDebug() << "SamplingThread dtor" << this;
}

void SamplingThread::sample(double elapsed)
{
	//qDebug() << "sampling" << elapsed;
	Sample mySample;
	static float prevX = 0;
	static float prevY = 0;
	static float prevZ = 0;
	prevX += (float)(rand() % 10 - 4.5) / 10.0f;
	prevY += (float)(rand() % 10 - 4.5) / 10.0f;
	prevZ += (float)(rand() % 10 - 4.5) / 10.0f;
	mySample.x = prevX;
	mySample.y = prevY;
	mySample.z = prevZ;
	mySample.time = elapsed;
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
