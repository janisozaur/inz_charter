#ifndef PLOT_H
#define PLOT_H

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

public slots:
	void changeInterval(double newInterval);

private:
	QVector<QwtPlotCurve *> mCurves;
	int mTimerId;
	double mInterval;
	float mLastTime;
};

#endif // PLOT_H
