#ifndef SAMPLE_H
#define SAMPLE_H

enum Marker {Blue, Yellow};

struct Sample
{
	Marker marker;
	float right, up, left;
	float x, y, z;
	float time;
};

#endif // SAMPLE_H
