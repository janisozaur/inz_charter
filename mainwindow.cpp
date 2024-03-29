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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "positionplot.h"
#include "distanceplot.h"
#include "signaldata.h"
#include "sample.h"

#include <QSerialPort>
#include <QMetaEnum>
#include <qwt_plot_panner.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mPositionPlot = new PositionPlot(this);
	mDistancePlot = new DistancePlot(this);
	ui->positionHorizontalLayout->addWidget(mPositionPlot, 10);
	ui->distanceHorizontalLayout->addWidget(mDistancePlot, 10);
	mPositionPanner = new QwtPlotPanner(mPositionPlot->canvas());
	//mPositionPanner->setMouseButton(Qt::MidButton);
	mDistancePanner = new QwtPlotPanner(mDistancePlot->canvas());
	//mDistancePanner->setMouseButton(Qt::MidButton);

	//connect(mDistancePanner, SIGNAL(moved(int,int)), this, SLOT(movedSlot(int,int)));
	//connect(mDistancePanner, SIGNAL(panned(int,int)), this, SLOT(pannedSlot(int,int)));
	connect(mDistancePlot, SIGNAL(pan(int,int)), mDistancePanner, SIGNAL(panned(int,int)));
	connect(mPositionPlot, SIGNAL(pan(int,int)), mPositionPanner, SIGNAL(panned(int,int)));

	connect(&SignalData::instance(), SIGNAL(started()), this, SLOT(threadStarted()));
	connect(&SignalData::instance(), SIGNAL(finished()), this, SLOT(threadFinished()));
	connect(&SignalData::instance(), SIGNAL(error(QString)), ui->statusBar, SLOT(showMessage(QString)));

	// use map to sort values
	QMap<QString, QPortSettings::BaudRate> map;
	// values stolen from qportsettings.h
#ifdef TNX_POSIX_SERIAL_PORT
	map.insert("BAUDR_50", QPortSettings::BAUDR_50);
	map.insert("BAUDR_75", QPortSettings::BAUDR_75);
	map.insert("BAUDR_134", QPortSettings::BAUDR_134);
	map.insert("BAUDR_150", QPortSettings::BAUDR_150);
	map.insert("BAUDR_200", QPortSettings::BAUDR_200);
	map.insert("BAUDR_1800", QPortSettings::BAUDR_1800);
#endif
#ifdef Q_OS_LINUX
	map.insert("BAUDR_230400", QPortSettings::BAUDR_230400);
	map.insert("BAUDR_460800", QPortSettings::BAUDR_460800);
	map.insert("BAUDR_500000", QPortSettings::BAUDR_500000);
	map.insert("BAUDR_576000", QPortSettings::BAUDR_576000);
	map.insert("BAUDR_921600", QPortSettings::BAUDR_921600);
#endif
#ifdef TNX_WINDOWS_SERIAL_PORT
	map.insert("BAUDR_14400", QPortSettings::BAUDR_14400);
	map.insert("BAUDR_56000", QPortSettings::BAUDR_56000);
	map.insert("BAUDR_128000", QPortSettings::BAUDR_128000);
	map.insert("BAUDR_256000", QPortSettings::BAUDR_256000);
#endif
	// baud rates supported by all OSs
	map.insert("BAUDR_110", QPortSettings::BAUDR_110);
	map.insert("BAUDR_300", QPortSettings::BAUDR_300);
	map.insert("BAUDR_600", QPortSettings::BAUDR_600);
	map.insert("BAUDR_1200", QPortSettings::BAUDR_1200);
	map.insert("BAUDR_2400", QPortSettings::BAUDR_2400);
	map.insert("BAUDR_4800", QPortSettings::BAUDR_4800);
	map.insert("BAUDR_9600", QPortSettings::BAUDR_9600);
	map.insert("BAUDR_19200", QPortSettings::BAUDR_19200);
	map.insert("BAUDR_38400", QPortSettings::BAUDR_38400);
	map.insert("BAUDR_57600", QPortSettings::BAUDR_57600);
	map.insert("BAUDR_115200", QPortSettings::BAUDR_115200);

	for (int i = 0; i < map.count(); i++) {
		ui->portBaudRateComboBox->addItem(map.keys().at(i), map.values().at(i));
	}
	ui->portBaudRateComboBox->setCurrentIndex(
				ui->portBaudRateComboBox->findText("BAUDR_9600"));

	const QMetaObject metaObject = Sample::staticMetaObject;
	const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("Marker"));
	for (int i = 0; i < metaEnum.keyCount(); i++) {
		ui->markerSelectComboBox->addItem(metaEnum.key(i), metaEnum.value(i));
	}

	int idx = ui->markerSelectComboBox->findText("Yellow");
	if (idx != -1) {
		ui->markerSelectComboBox->setCurrentIndex(idx);
	}

#ifdef Q_OS_LINUX
	ui->portNameLineEdit->setText("/dev/ttyUSB0");
#endif
}

MainWindow::~MainWindow()
{
	SignalData::instance().stop();
	SignalData::instance().wait();
	qDebug() << "MainWindow dtor";
	delete ui;
}

void MainWindow::pannedSlot(int x, int y)
{
	qDebug() << "panned(" << x << "," << y << ")";
}

void MainWindow::movedSlot(int x, int y)
{
	qDebug() << "moved(" << x << "," << y << ")";
}

void MainWindow::on_startButton_clicked()
{
	QVariant baudVariant = ui->portBaudRateComboBox->itemData(
				ui->portBaudRateComboBox->currentIndex());
	QPortSettings::BaudRate baud = (QPortSettings::BaudRate)baudVariant.toInt();
	SignalData::instance().start(ui->portNameLineEdit->text(), baud);
}

void MainWindow::on_markerSelectComboBox_currentIndexChanged(int index)
{
	mPositionPlot->setMarker((Sample::Marker)ui->markerSelectComboBox->itemData(index).toInt());
	mDistancePlot->setMarker((Sample::Marker)ui->markerSelectComboBox->itemData(index).toInt());
}

void MainWindow::threadStarted()
{
	ui->statusBar->showMessage("Thread started!");
}

void MainWindow::threadFinished()
{
	ui->statusBar->showMessage("Thread terminated!");
}
