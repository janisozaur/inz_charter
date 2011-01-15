#include "sampledata.h"
#include "signaldata.h"
#include <QMutexLocker>
#include <QDebug>

SampleData::SampleData(Sample::Marker which) :
	mWhich(which)
{
	qDebug() << "SampleData ctor" << mWhich << this;
}

SampleData::~SampleData()
{
	qDebug() << "SampleData dtor" << this;
}

QPointF SampleData::sample(size_t i) const
{
	//qDebug() << "asking for sample" << i << "out of" << size();
	const Sample mySample = SignalData::instance().value(mWhich, i);
	return pointFromSample(mySample);
}

size_t SampleData::size() const
{
	return SignalData::instance().size(mWhich);
}

QRectF SampleData::boundingRect() const
{
	return SignalData::instance().boundingRect(mWhich);
}
