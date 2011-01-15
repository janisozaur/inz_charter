#include "distancedataup.h"

DistanceDataUp::DistanceDataUp(Sample::Marker which) :
	SampleData(which)
{
	qDebug() << "DistanceDataUp ctor" << mWhich << this;
}

QPointF DistanceDataUp::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.up);
}
