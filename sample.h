#ifndef SAMPLE_H
#define SAMPLE_H

#include <QVector3D>
#include <QObject>


class Sample : public QObject
{
	Q_OBJECT;
	Q_ENUMS(Marker);

public:
	enum Marker {Blue = 1, Yellow = 2};

	Sample(QObject *parent = 0);
	Sample(const Sample &other);
	Sample &operator =(const Sample &other);
	Marker marker;
	float right, up, left;
	QVector3D pos;
	float time;
};

#endif // SAMPLE_H
