#ifndef SAMPLE_H
#define SAMPLE_H

#include <QVector3D>

enum Marker {Blue = 1, Yellow = 2};

struct Sample
{
	Marker marker;
	float right, up, left;
	QVector3D pos;
	float time;
};

#endif // SAMPLE_H
