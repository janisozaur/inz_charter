#include "positiondatax.h"

PositionDataX::PositionDataX(Marker which) :
	SampleData(which)
{
}

QPointF PositionDataX::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.x);
}
