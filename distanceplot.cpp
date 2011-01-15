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
	mInterval(10)
{
	mPanX = 0;
	mPrevRect = 0;
	qDebug() << "DistancePlot ctor" << this;
	insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
	setAxisTitle(xBottom, "t[s]");
	setAxisTitle(yLeft, "x");
	setAxisScale(QwtPlot::yLeft, -5.0, 100.0);
	setAxisScale(QwtPlot::xBottom, 0.0, mInterval);
	//setAutoReplot(true);
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
	if (event->timerId() == mTimerId && !mCurves.isEmpty()) {
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

void DistancePlot::changeInterval(double newInterval)
{
	mInterval = newInterval;
}

void DistancePlot::setMarker(Sample::Marker which)
{
	for (int i = 0; i < mCurves.count(); i++) {
		delete mCurves.at(i);
	}
	mCurves.clear();

	QwtPlotCurve *curveX = new QwtPlotCurve("1");
	curveX->setStyle(QwtPlotCurve::Lines);
	curveX->setPen(QPen(Qt::green));
	//curveX->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveX->setData(new DistanceDataLeft(which));
	curveX->attach(this);
	mCurves << curveX;

	QwtPlotCurve *curveZ = new QwtPlotCurve("2");
	curveZ->setStyle(QwtPlotCurve::Lines);
	curveZ->setPen(QPen(Qt::blue));
	//curveZ->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveZ->setData(new DistanceDataUp(which));
	curveZ->attach(this);
	mCurves << curveZ;

	QwtPlotCurve *curveY = new QwtPlotCurve("3");
	curveY->setStyle(QwtPlotCurve::Lines);
	curveY->setPen(QPen(Qt::red));
	//curveY->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveY->setData(new DistanceDataRight(which));
	curveY->attach(this);
	mCurves << curveY;
}
