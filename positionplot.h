#ifndef PLOT_H
#define PLOT_H

#include "sample.h"

#include <qwt_plot.h>

class QwtPlotCurve;

class PositionPlot : public QwtPlot
{
	Q_OBJECT
public:
	explicit PositionPlot(QWidget *parent = 0);
	~PositionPlot();
	virtual void replot();

protected:
	virtual void timerEvent(QTimerEvent *event);

signals:
	void pan(int, int);

public slots:
	void changeInterval(double newInterval);
	void setMarker(Sample::Marker which);

private:
	QVector<QwtPlotCurve *> mCurves;
	int mTimerId;
	double mInterval;
	float mPanX;
	float mPrevRect;
};

#endif // PLOT_H
