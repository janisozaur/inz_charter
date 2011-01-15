#include "sample.h"

Sample::Sample(QObject *parent) :
	QObject(parent)
{
}

Sample::Sample(const Sample &other) :
	QObject(),
	marker(other.marker),
	right(other.right),
	up(other.up),
	left(other.left),
	pos(other.pos),
	time(other.time)
{
}

Sample &Sample::operator =(const Sample& other)
{
	if (this != &other) {
		this->marker = other.marker;
		this->right = other.right;
		this->up = other.up;
		this->left = other.left;
		this->pos = other.pos;
		this->time = other.time;
	}
	return *this;
}
