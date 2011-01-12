#include "distancedataright.h"

DistanceDataRight::DistanceDataRight(Marker which) :
	SampleData(which)
{
	qDebug() << "DistanceDataRight ctor" << mWhich << this;
}

QPointF DistanceDataRight::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.right);
}
