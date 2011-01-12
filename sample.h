#ifndef SAMPLE_H
#define SAMPLE_H

enum Marker {Blue = 1, Yellow = 2};

struct Sample
{
	Marker marker;
	float right, up, left;
	float x, y, z;
	float time;
};

#endif // SAMPLE_H
