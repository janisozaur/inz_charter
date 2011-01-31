/*
 * charter
 * Copyright (C) 2011 Michał Janiszewski
 *
 * This work is a thesis. Due to nature of law you are obliged to
 * get a written permission from Technical University of Lodz prior to
 * using any part of this work. If you do get a permission, you are able
 * to use the work, which is thereafter licensed using GNU GPL v3 or
 * (at your option) any later.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	setAxisScale(QwtPlot::yLeft, -50.0, 50.0);
	setAxisScale(QwtPlot::xBottom, 0.0, mInterval);
	//setAutoReplot(true);
	qDebug() << "cache:" << canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);
	mTimerId = startTimer(50);
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

void PositionPlot::changeInterval(double newInterval)
{
	mInterval = newInterval;
}

void PositionPlot::setMarker(Sample::Marker which)
{
	for (int i = 0; i < mCurves.count(); i++) {
		delete mCurves.at(i);
	}
	mCurves.clear();

	QwtPlotCurve *curveX = new QwtPlotCurve("x");
	curveX->setStyle(QwtPlotCurve::Lines);
	curveX->setPen(QPen(Qt::green));
	//curveX->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveX->setData(new PositionDataX(which));
	curveX->attach(this);
	mCurves << curveX;

	QwtPlotCurve *curveY = new QwtPlotCurve("y");
	curveY->setStyle(QwtPlotCurve::Lines);
	curveY->setPen(QPen(Qt::red));
	//curveY->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveY->setData(new PositionDataY(which));
	curveY->attach(this);
	mCurves << curveY;

	QwtPlotCurve *curveZ = new QwtPlotCurve("z");
	curveZ->setStyle(QwtPlotCurve::Lines);
	curveZ->setPen(QPen(Qt::blue));
	//curveZ->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curveZ->setData(new PositionDataZ(which));
	curveZ->attach(this);
	mCurves << curveZ;
}
