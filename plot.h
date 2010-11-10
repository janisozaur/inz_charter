#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>

class QwtPlotCurve;

class Plot : public QwtPlot
{
	Q_OBJECT
public:
	explicit Plot(QWidget *parent = 0);
	~Plot();
	virtual void replot();

protected:
	virtual void timerEvent(QTimerEvent *event);

signals:

public slots:

private:
	QVector<QwtPlotCurve *> mCurves;
	int mTimerId;
};

#endif // PLOT_H
