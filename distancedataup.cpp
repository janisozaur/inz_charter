#include "distancedataup.h"

DistanceDataUp::DistanceDataUp()
{
}

QPointF DistanceDataUp::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.up);
}
