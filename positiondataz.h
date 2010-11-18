#ifndef POSITIONDATAZ_H
#define POSITIONDATAZ_H

#include "sampledata.h"

class PositionDataZ : public SampleData
{
public:
    PositionDataZ();

protected:
	virtual QPointF pointFromSample(const Sample &sample) const;
};

#endif // POSITIONDATAZ_H
