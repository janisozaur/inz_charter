#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include "QHBoxLayout"

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
