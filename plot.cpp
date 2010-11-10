#include "plot.h"
#include "positiondatax.h"
#include <qwt_plot_curve.h>
#include <QDebug>
#include <QTimerEvent>

Plot::Plot(QWidget *parent) :
	QwtPlot(parent),
	mTimerId(-1)
{
	qDebug() << "Plot ctor" << this;
	mCurve = new QwtPlotCurve();
	mCurve->setStyle(QwtPlotCurve::Lines);
	mCurve->setPen(QPen(Qt::green));
	mCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	mCurve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
	// TODO: add multiple curves, one for each parameter and create an
	// interface to extract this data from CurveData.
	mCurve->setData(new PositionDataX());
	mCurve->attach(this);
	setAutoReplot(true);
	mTimerId = startTimer(10);
}

Plot::~Plot()
{
	qDebug() << "Plot dtor" << this;
}

void Plot::replot()
{
	//qDebug() << "replotting";
	QwtPlot::replot();
}

void Plot::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == mTimerId) {
		replot();
	}
	QwtPlot::timerEvent(event);
}
