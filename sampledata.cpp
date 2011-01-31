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

#include "sampledata.h"
#include "signaldata.h"
#include <QMutexLocker>
#include <QDebug>

SampleData::SampleData(Sample::Marker which) :
	mWhich(which)
{
	qDebug() << "SampleData ctor" << mWhich << this;
}

SampleData::~SampleData()
{
	qDebug() << "SampleData dtor" << this;
}

QPointF SampleData::sample(size_t i) const
{
	//qDebug() << "asking for sample" << i << "out of" << size();
	const Sample mySample = SignalData::instance().value(mWhich, i);
	return pointFromSample(mySample);
}

size_t SampleData::size() const
{
	return SignalData::instance().size(mWhich);
}

QRectF SampleData::boundingRect() const
{
	return SignalData::instance().boundingRect(mWhich);
}
