#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include "signaldata.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mPlot = new Plot(this);
	ui->horizontalLayout->addWidget(mPlot, 10);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_startButton_clicked()
{
	SignalData::instance().start();
}
