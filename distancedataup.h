#ifndef DISTANCEDATAUP_H
#define DISTANCEDATAUP_H

#include "sampledata.h"

class DistanceDataUp : public SampleData
{
public:
	DistanceDataUp(Sample::Marker which);

protected:
	QPointF pointFromSample(const Sample &sample) const;
};

#endif // DISTANCEDATAUP_H
