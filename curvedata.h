#ifndef CURVEDATA_H
#define CURVEDATA_H

#include "signaldata.h"
#include "positiondatax.h"
#include <qwt_series_data.h>

class CurveData : public QwtSeriesData<Sample>
{
public:
	CurveData();
	~CurveData();
	const SignalData &values() const;
	SignalData &values();

	virtual Sample sample(size_t i) const;
	virtual size_t size() const;

	virtual QRectF boundingRect() const;

	Sample value(size_t index) const;
};

#endif // CURVEDATA_H
