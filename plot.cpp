#include "plot.h"
#include "positiondatax.h"
#include "positiondatay.h"
#include "positiondataz.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <QDebug>
#include <QTimerEvent>

Plot::Plot(QWidget *parent) :
	QwtPlot(parent),
	mTimerId(-1),
	mInterval(10),
	mLastTime(0)
{
	qDebug() << "Plot ctor" << this;
	QwtPlotCurve *curveX = new QwtPlotCurve();
	curveX->setStyle(QwtPlotCurve::Lines);
	curveX->setPen(QPen(Qt::green));
	//curveX->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveX->setData(new PositionDataX());
	curveX->attach(this);
	mCurves << curveX;
	QwtPlotCurve *curveY = new QwtPlotCurve();
	curveY->setStyle(QwtPlotCurve::Lines);
	curveY->setPen(QPen(Qt::red));
	//curveY->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveY->setData(new PositionDataY());
	curveY->attach(this);
	mCurves << curveY;
	QwtPlotCurve *curveZ = new QwtPlotCurve();
	curveZ->setStyle(QwtPlotCurve::Lines);
	curveZ->setPen(QPen(Qt::blue));
	//curveZ->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveZ->setData(new PositionDataZ());
	curveZ->attach(this);
	mCurves << curveZ;
	setAxisScale(QwtPlot::yLeft, -5.0, 2000.0);
	setAxisScale(QwtPlot::xBottom, 0.0, mInterval);
	setAutoReplot(true);
	qDebug() << "cache:" << canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);
	mTimerId = startTimer(35);
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

void Plot::changeInterval(double newInterval)
{
	mInterval = newInterval;
}
