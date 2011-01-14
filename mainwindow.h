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
	void pannedSlot(int, int);
	void movedSlot(int, int);
	void threadStarted();
	void threadFinished();

	void on_markerSelectComboBox_currentIndexChanged(int index);

private:
	Ui::MainWindow *ui;
	PositionPlot *mPositionPlot;
	DistancePlot *mDistancePlot;
	QwtPlotPanner *mPositionPanner;
	QwtPlotPanner *mDistancePanner;
};

#endif // MAINWINDOW_H
