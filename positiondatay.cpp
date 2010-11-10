#include "positiondatay.h"

PositionDataY::PositionDataY()
{
}

QPointF PositionDataY::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.y);
}
