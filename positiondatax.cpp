#include "positiondatax.h"
#include "signaldata.h"
#include <QMutexLocker>
#include <QDebug>

PositionDataX::PositionDataX()
{
	qDebug() << "PositionDataX ctor" << this;
}

PositionDataX::~PositionDataX()
{
	qDebug() << "PositionDataX dtor" << this;
}

QPointF PositionDataX::sample(size_t i) const
{
	//qDebug() << "asking for sample" << i << "out of" << size();
	const Sample mySample = SignalData::instance().value(i);
	return QPointF(mySample.time, mySample.x);
}

size_t PositionDataX::size() const
{
	return SignalData::instance().size();
}

QRectF PositionDataX::boundingRect() const
{
	return SignalData::instance().boundingRect();
}
