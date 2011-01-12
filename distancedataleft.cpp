#include "distancedataleft.h"

DistanceDataLeft::DistanceDataLeft(Marker which) :
	SampleData(which)
{
}

QPointF DistanceDataLeft::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.left);
}
