#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class PositionPlot;
class DistancePlot;
class QwtPlotPanner;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_startButton_clicked();

private:
	Ui::MainWindow *ui;
	PositionPlot *mPositionPlot;
	DistancePlot *mDistancePlot;
	QwtPlotPanner *mPositionPanner;
	QwtPlotPanner *mDistancePanner;
};

#endif // MAINWINDOW_H
