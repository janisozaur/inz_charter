#ifndef DISTANCEDATARIGHT_H
#define DISTANCEDATARIGHT_H

#include "sampledata.h"

class DistanceDataRight : public SampleData
{
public:
	DistanceDataRight(Sample::Marker which);

protected:
	QPointF pointFromSample(const Sample &sample) const;
};

#endif // DISTANCEDATAUP_H
