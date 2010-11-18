#include "distancedataleft.h"

DistanceDataLeft::DistanceDataLeft()
{
}

QPointF DistanceDataLeft::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.left);
}
