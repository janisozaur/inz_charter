#ifndef POSITIONDATA_H
#define POSITIONDATA_H

#include <qwt_series_data.h>
#include <QMutex>
#include "sample.h"

class PositionData : public QwtSeriesData<QPointF>
{
public:
	PositionData();
	~PositionData();

	virtual QPointF sample(size_t i) const;
	virtual size_t size() const;

	virtual QRectF boundingRect() const;

protected:
	virtual QPointF pointFromSample(const Sample &sample) const = 0;

private:
	mutable QMutex mMutex;
	QVector<QPointF> mSamples;
	QRectF mBoundingRect;
};

#endif // POSITIONDATA_H
