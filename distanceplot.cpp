#include "distanceplot.h"
#include "distancedataleft.h"
#include "distancedataright.h"
#include "distancedataup.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <QDebug>
#include <QTimerEvent>

DistancePlot::DistancePlot(QWidget *parent) :
	QwtPlot(parent),
	mTimerId(-1),
	mInterval(10),
	mLastTime(0)
{
	qDebug() << "DistancePlot ctor" << this;
	insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
	QwtPlotCurve *curveX = new QwtPlotCurve("1");
	curveX->setStyle(QwtPlotCurve::Lines);
	curveX->setPen(QPen(Qt::green));
	//curveX->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveX->setData(new DistanceDataLeft());
	curveX->attach(this);
	mCurves << curveX;
	QwtPlotCurve *curveZ = new QwtPlotCurve("2");
	curveZ->setStyle(QwtPlotCurve::Lines);
	curveZ->setPen(QPen(Qt::blue));
	//curveZ->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveZ->setData(new DistanceDataUp());
	curveZ->attach(this);
	mCurves << curveZ;
	QwtPlotCurve *curveY = new QwtPlotCurve("3");
	curveY->setStyle(QwtPlotCurve::Lines);
	curveY->setPen(QPen(Qt::red));
	//curveY->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveY->setData(new DistanceDataRight());
	curveY->attach(this);
	mCurves << curveY;
	setAxisScale(QwtPlot::yLeft, -5.0, 100.0);
	setAxisScale(QwtPlot::xBottom, 0.0, mInterval);
	setAutoReplot(true);
	qDebug() << "cache:" << canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);
	mTimerId = startTimer(35);
}

DistancePlot::~DistancePlot()
{
	qDebug() << "DistancePlot dtor" << this;
}

void DistancePlot::replot()
{
	//qDebug() << "replotting";
	QwtPlot::replot();
}

void DistancePlot::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == mTimerId) {
		replot();
		QRectF rect = mCurves.first()->data()->boundingRect();
		if (rect.right() > mLastTime) {
			float min = qMax(0.0f, mLastTime);
			setAxisScale(QwtPlot::xBottom, min, min + mInterval);
		}
		mLastTime = rect.right() - mInterval;
		updateAxes();
	}
	QwtPlot::timerEvent(event);
}

void DistancePlot::changeInterval(double newInterval)
{
	mInterval = newInterval;
}
