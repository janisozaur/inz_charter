#include "positiondatax.h"

PositionDataX::PositionDataX(Marker which) :
	SampleData(which)
{
	qDebug() << "PositionDataX ctor" << mWhich << this;
}

QPointF PositionDataX::pointFromSample(const Sample &sample) const
{
	return QPointF(sample.time, sample.pos.x());
}
