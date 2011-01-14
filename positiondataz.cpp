#include "positiondataz.h"

PositionDataZ::PositionDataZ(Marker which) :
	SampleData(which)
{
	qDebug() << "PositionDataZ ctor" << mWhich << this;
}

QPointF PositionDataZ::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.pos.z());
}
