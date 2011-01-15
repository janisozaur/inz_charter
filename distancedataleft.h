#ifndef DISTANCEDATALEFT_H
#define DISTANCEDATALEFT_H

#include "sampledata.h"

class DistanceDataLeft : public SampleData
{
public:
	DistanceDataLeft(Sample::Marker which);

protected:
	QPointF pointFromSample(const Sample &sample) const;
};

#endif // DISTANCEDATALEFT_H
