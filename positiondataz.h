#ifndef POSITIONDATAZ_H
#define POSITIONDATAZ_H

#include "positiondata.h"

class PositionDataZ : public PositionData
{
public:
    PositionDataZ();

protected:
	virtual QPointF pointFromSample(const Sample &sample) const;
};

#endif // POSITIONDATAZ_H
