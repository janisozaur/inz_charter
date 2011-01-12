#ifndef DISTANCEPLOT_H
#define DISTANCEPLOT_H

#include <qwt_plot.h>

class QwtPlotCurve;

class DistancePlot : public QwtPlot
{
	Q_OBJECT
public:
	explicit DistancePlot(QWidget *parent = 0);
	~DistancePlot();
	virtual void replot();

protected:
	virtual void timerEvent(QTimerEvent *event);

signals:
	void pan(int, int);

public slots:
	void changeInterval(double newInterval);

private:
	QVector<QwtPlotCurve *> mCurves;
	int mTimerId;
	double mInterval;
	float mPanX;
	float mPrevRect;
};

#endif // DISTANCEPLOT_H
