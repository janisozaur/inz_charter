#ifndef POSITIONDATAX_H
#define POSITIONDATAX_H

#include <qwt_series_data.h>
#include <QMutex>
#include "sample.h"

class PositionDataX : public QwtSeriesData<QPointF>
{
public:
    PositionDataX();
	~PositionDataX();

	virtual QPointF sample(size_t i) const;
	virtual size_t size() const;

	virtual QRectF boundingRect() const;

	//void appendSamples(QVector<Sample> samples);

private:
	mutable QMutex mMutex;
	QVector<QPointF> mSamples;
	QRectF mBoundingRect;
};

#endif // POSITIONDATAX_H
