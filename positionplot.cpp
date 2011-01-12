#include "positionplot.h"
#include "positiondatax.h"
#include "positiondatay.h"
#include "positiondataz.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <QDebug>
#include <QTimerEvent>

PositionPlot::PositionPlot(QWidget *parent) :
	QwtPlot(parent),
	mTimerId(-1),
	mInterval(10)
{
	qDebug() << "Plot ctor" << this;
	insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
	setAxisTitle(xBottom, "t[s]");
	setAxisTitle(yLeft, "x[cm]");
	QwtPlotCurve *curveX = new QwtPlotCurve("x");
	curveX->setStyle(QwtPlotCurve::Lines);
	curveX->setPen(QPen(Qt::green));
	//curveX->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveX->setData(new PositionDataX(Yellow));
	curveX->attach(this);
	mCurves << curveX;
	QwtPlotCurve *curveY = new QwtPlotCurve("y");
	curveY->setStyle(QwtPlotCurve::Lines);
	curveY->setPen(QPen(Qt::red));
	//curveY->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveY->setData(new PositionDataY(Yellow));
	curveY->attach(this);
	mCurves << curveY;
	QwtPlotCurve *curveZ = new QwtPlotCurve("z");
	curveZ->setStyle(QwtPlotCurve::Lines);
	curveZ->setPen(QPen(Qt::blue));
	//curveZ->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveZ->setData(new PositionDataZ(Yellow));
	curveZ->attach(this);
	mCurves << curveZ;
	setAxisScale(QwtPlot::yLeft, 0.0, 50.0);
	setAxisScale(QwtPlot::xBottom, 0.0, mInterval);
	setAutoReplot(true);
	qDebug() << "cache:" << canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);
	mTimerId = startTimer(35);
}

PositionPlot::~PositionPlot()
{
	qDebug() << "Plot dtor" << this;
}

void PositionPlot::replot()
{
	//qDebug() << "replotting";
	QwtPlot::replot();
}

void PositionPlot::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == mTimerId) {
		replot();
		QRectF rect = mCurves.first()->data()->boundingRect();
		float min = qMax(0.0, rect.right() - mInterval);
		double pps = (canvas()->size().width() - 13) / mInterval;
		float td = min - mPrevRect;
		mPanX -= td * pps;
		int panX = mPanX;
		mPanX -= panX;
		emit pan(panX, 0);
		mPrevRect = min;
	}
	QwtPlot::timerEvent(event);
}

void PositionPlot::changeInterval(double newInterval)
{
	mInterval = newInterval;
}
