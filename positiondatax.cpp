#include "positiondatax.h"

PositionDataX::PositionDataX()
{
}

QPointF PositionDataX::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.x);
}
