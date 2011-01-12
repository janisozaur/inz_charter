#include "positiondataz.h"

PositionDataZ::PositionDataZ(Marker which) :
	SampleData(which)
{
}

QPointF PositionDataZ::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.z);
}
