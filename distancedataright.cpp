#include "distancedataright.h"

DistanceDataRight::DistanceDataRight()
{
}

QPointF DistanceDataRight::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.right);
}
