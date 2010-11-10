#include "curvedata.h"

CurveData::CurveData()
{
	qDebug() << "CurveData ctor" << this;
}

CurveData::~CurveData()
{
	qDebug() << "CurveData dtor" << this;
}

Sample CurveData::sample(size_t i) const
{
	return value(i);
}

Sample CurveData::value(size_t i) const
{
	return SignalData::instance().value(i);
}

size_t CurveData::size() const
{
	return SignalData::instance().size();
}

QRectF CurveData::boundingRect() const
{
	return SignalData::instance().boundingRect();
}

const SignalData &CurveData::values() const
{
	return SignalData::instance();
}

SignalData &CurveData::values()
{
	return SignalData::instance();
}
