#include "positiondatay.h"

PositionDataY::PositionDataY(Marker which) :
	SampleData(which)
{
	qDebug() << "PositionDataY ctor" << mWhich << this;
}

QPointF PositionDataY::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.y);
}
