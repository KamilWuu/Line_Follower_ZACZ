#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include <QtCharts>

#define basicAxisXTime 15

class PlotWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = nullptr);
    ~PlotWindow();

    void updatePlot(float time, int pwmL, int pwmR, const int sensors[], float speed, float battery);
    void clearPlot();

private:
    QChartView *pwmChartView;
    QChartView *speedChartView;
    QChartView *batteryChartView;

    QLineSeries *pwmLSeries;
    QLineSeries *pwmRSeries;
    QScatterSeries *sensorSeries;

    QLineSeries *speedSeries;
    QLineSeries *batterySeries;
};

#endif // PLOTWINDOW_H
