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

#ifndef SAMPLINGTHREAD_H
#define SAMPLINGTHREAD_H

#include <QVector>
#include <QMutexLocker>
#include <qwt_sampling_thread.h>
#include <QFile>
#include <QSerialPort>

using namespace TNX;

#include "sample.h"

class SamplingThread : public QwtSamplingThread
{
	Q_OBJECT
public:
	explicit SamplingThread(QObject *parent = 0);
	~SamplingThread();
	QVector<Sample> takeSamples();
	bool open(QString fileName, QPortSettings::BaudRate baudRate);
	void close();

protected:
	virtual void sample(double elapsed);
	void append(Sample mySample);
	void append(const QByteArray &data, double elapsed);
	bool isValidSample(const char c) const;

signals:
	void dataArrived();
	void error(QString);

private:
	QVector<Sample> samples;
	QByteArray mTempData;
	QMutex mutex;
	QSerialPort *mpSerport;
	QVector<Sample::Marker> mMarkers;

signals:

public slots:

};

#endif // SAMPLINGTHREAD_H
