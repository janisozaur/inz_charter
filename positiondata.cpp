#include "positiondatax.h"
#include "signaldata.h"
#include <QMutexLocker>
#include <QDebug>

PositionData::PositionData()
{
	qDebug() << "PositionData ctor" << this;
}

PositionData::~PositionData()
{
	qDebug() << "PositionData dtor" << this;
}

QPointF PositionData::sample(size_t i) const
{
	//qDebug() << "asking for sample" << i << "out of" << size();
	const Sample mySample = SignalData::instance().value(i);
	return pointFromSample(mySample);
}

size_t PositionData::size() const
{
	return SignalData::instance().size();
}

QRectF PositionData::boundingRect() const
{
	return SignalData::instance().boundingRect();
}
