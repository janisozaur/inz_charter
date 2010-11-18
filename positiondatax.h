#ifndef POSITIONDATAX_H
#define POSITIONDATAX_H

#include "sampledata.h"

class PositionDataX : public SampleData
{
public:
	PositionDataX();

protected:
	virtual QPointF pointFromSample(const Sample &sample) const;
};

#endif // POSITIONDATAX_H
