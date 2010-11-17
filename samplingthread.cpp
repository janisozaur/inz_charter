#include "samplingthread.h"
#include <QFile>
#include <QMessageBox>
#include <QSerialPort>

#include <QDebug>

SamplingThread::SamplingThread(QObject *parent) :
	QwtSamplingThread(parent),
	mpSerport(NULL)
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
	qint64 avail = 16;//mpSerport->bytesAvailable();
	QByteArray readData;
	readData.reserve(avail);
	//mpSerport->errorString();
	qint64 bytesRead = mpSerport->read(readData.data(), avail);
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
}

void SamplingThread::open(QString fileName, QPortSettings::BaudRate baudRate)
{
	qDebug() << "Opening" << fileName << fileName.toLatin1();
	if (mpSerport == NULL) {
		QPortSettings settings;
		settings.setBaudRate(baudRate);
		settings.setDataBits(QPortSettings::DB_8);
		settings.setFlowControl(QPortSettings::FLOW_OFF);
		settings.setParity(QPortSettings::PAR_NONE);
		settings.setStopBits(QPortSettings::STOP_1);
		qDebug() << "settings:" << settings.toString();
		mpSerport = new QSerialPort(fileName, settings);
		if (!mpSerport->open()) {
			qDebug() << "failed to open serial port" << fileName;
		} else {
			qDebug() << "port" << fileName << "successfully opened";
		}
		if (!mpSerport->setCommTimeouts(QSerialPort::CtScheme_NonBlockingRead)) {
			qWarning("Cannot set communications timeout values at port %s.", qPrintable(fileName));
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
