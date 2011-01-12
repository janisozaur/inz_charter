#include "distancedataleft.h"

DistanceDataLeft::DistanceDataLeft(Marker which) :
	SampleData(which)
{
	qDebug() << "DistanceDataLeft ctor" << mWhich << this;
}

QPointF DistanceDataLeft::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.left);
}
