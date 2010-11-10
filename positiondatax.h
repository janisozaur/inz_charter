#ifndef POSITIONDATAX_H
#define POSITIONDATAX_H

#include "positiondata.h"

class PositionDataX : public PositionData
{
public:
	PositionDataX();

protected:
	virtual QPointF pointFromSample(const Sample &sample) const;
};

#endif // POSITIONDATAX_H
