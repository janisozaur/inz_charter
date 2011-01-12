#include "positiondatay.h"

PositionDataY::PositionDataY(Marker which) :
	SampleData(which)
{
}

QPointF PositionDataY::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.y);
}
