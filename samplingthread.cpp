#include "samplingthread.h"
#include <QFile>

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
	qint64 avail = 16;//mPort.bytesAvailable();
	QByteArray readData;
	readData.reserve(avail);
	qint64 bytesRead = mPort.read(readData.data(), avail);
	//qDebug() << "Read" << bytesRead << "bytes of available" << avail;
	if (bytesRead != avail) {
		//qWarning() << "Warning! Read" << bytesRead << "bytes instead of" <<
		//			  avail << "bytes available.";
	}
	if (bytesRead == -1) {
		qWarning() << "No further data can be read. Stopping sampler.";
		stop();
		return;
	}
	readData.resize(bytesRead);
	append(readData, elapsed);
	/*Sample mySample;
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
	append(mySample);*/
}

void SamplingThread::open(QString fileName)
{
	if (!mPort.isOpen()) {
		qDebug() << "Opening" << fileName;
		mPort.setFileName(fileName);
		bool opened = mPort.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
		qDebug() << "opened:" << opened;
		if (!opened) {
			qDebug() << mPort.errorString();
		}
	}
}

void SamplingThread::append(Sample mySample)
{
	QMutexLocker locker(&mutex);
	samples.append(mySample);
	emit dataArrived();
}

void SamplingThread::append(const QByteArray &data, double elapsed)
{
	QMutexLocker locker(&mutex);
	mTempData.append(data);
	// skip any leading malformed data
	while (mTempData.size() >= 8 && mTempData.at(0) != 0 && mTempData.at(7) != (char)0xFF) {
		qDebug() << "****************** removing data";
		mTempData.remove(0, 1);
	}
	while (mTempData.size() >= 8) {
		Sample mySample;
		mySample.x = 256 * (unsigned char)mTempData.at(2) + (unsigned char)mTempData.at(1);
		mySample.y = 256 * (unsigned char)mTempData.at(4) + (unsigned char)mTempData.at(3);
		mySample.z = 256 * (unsigned char)mTempData.at(6) + (unsigned char)mTempData.at(5);
		mySample.time = elapsed;
		//qDebug() << mTempData.left(8).toHex();
		//qDebug() << "Sample(" << mySample.x << "," << mySample.y << "," <<
		//			mySample.z << "," << mySample.time << ")";
		samples << mySample;
		mTempData.remove(0, 8);
	}
	emit dataArrived();
}

QVector<Sample> SamplingThread::takeSamples()
{
	QMutexLocker locker(&mutex);
	QVector<Sample> result = samples.mid(0);
	samples.clear();
	return result;
}
