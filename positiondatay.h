#ifndef POSITIONDATAY_H
#define POSITIONDATAY_H

#include "sampledata.h"

class PositionDataY : public SampleData
{
public:
	PositionDataY(Sample::Marker which);

protected:
	virtual QPointF pointFromSample(const Sample &sample) const;
};

#endif // POSITIONDATAY_H
