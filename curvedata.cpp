#include "curvedata.h"

CurveData::CurveData()
{
}

Sample CurveData::sample(size_t i) const
{
	return value(i);
}

Sample CurveData::value(size_t i) const
{
	return mSignal.value(i);
}

size_t CurveData::size() const
{
	return mSignal.size();
}

QRectF CurveData::boundingRect() const
{
	return mSignal.boundingRect();
}

const SignalData &CurveData::values() const
{
	return mSignal;
}

SignalData &CurveData::values()
{
	return mSignal;
}
