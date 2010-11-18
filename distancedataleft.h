#ifndef DISTANCEDATALEFT_H
#define DISTANCEDATALEFT_H

#include "sampledata.h"

class DistanceDataLeft : public SampleData
{
public:
    DistanceDataLeft();

protected:
	QPointF pointFromSample(const Sample &sample) const;
};

#endif // DISTANCEDATALEFT_H
