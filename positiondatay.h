#ifndef POSITIONDATAY_H
#define POSITIONDATAY_H

#include "positiondata.h"

class PositionDataY : public PositionData
{
public:
	PositionDataY();

protected:
	virtual QPointF pointFromSample(const Sample &sample) const;
};

#endif // POSITIONDATAY_H
