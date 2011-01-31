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
