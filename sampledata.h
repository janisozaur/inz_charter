#ifndef POSITIONDATA_H
#define POSITIONDATA_H

#include <qwt_series_data.h>
#include <QMutex>
#include "sample.h"

class SampleData : public QwtSeriesData<QPointF>
{
public:
	SampleData(Marker which);
	~SampleData();

	virtual QPointF sample(size_t i) const;
	virtual size_t size() const;

	virtual QRectF boundingRect() const;

protected:
	virtual QPointF pointFromSample(const Sample &sample) const = 0;

protected:
	mutable QMutex mMutex;
	QVector<QPointF> mSamples;
	QRectF mBoundingRect;
	Marker mWhich;
};

#endif // POSITIONDATA_H
