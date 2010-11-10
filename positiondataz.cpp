#include "positiondataz.h"

PositionDataZ::PositionDataZ()
{
}

QPointF PositionDataZ::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.z);
}
