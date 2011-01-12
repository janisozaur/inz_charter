#include "distancedataup.h"

DistanceDataUp::DistanceDataUp(Marker which) :
	SampleData(which)
{
}

QPointF DistanceDataUp::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.up);
}
