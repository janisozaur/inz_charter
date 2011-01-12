#include "distancedataright.h"

DistanceDataRight::DistanceDataRight(Marker which) :
	SampleData(which)
{
}

QPointF DistanceDataRight::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.right);
}
