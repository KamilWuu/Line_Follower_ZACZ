#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QDialog(parent),
    pwmChartView(new QChartView(this)),
    speedChartView(new QChartView(this)),
    batteryChartView(new QChartView(this)),
    pwmLSeries(new QLineSeries),
    pwmRSeries(new QLineSeries),
    sensorSeries(new QScatterSeries),
    speedSeries(new QLineSeries),
    batterySeries(new QLineSeries)
{
    // PWM Chart
    QChart *pwmChart = new QChart();
    pwmChart->addSeries(pwmLSeries);
    pwmChart->addSeries(pwmRSeries);
    pwmChart->addSeries(sensorSeries);
    pwmChart->setTitle("PWM and Sensor Readings");
    pwmChart->createDefaultAxes();

    // Left Y Axis (for PWM)
    QValueAxis *leftAxisY = new QValueAxis;
    leftAxisY->setTitleText("PWM Values [%]");
    leftAxisY->setRange(0, 100);
    pwmChart->setAxisY(leftAxisY, pwmLSeries);
    pwmChart->setAxisY(leftAxisY, pwmRSeries);

    // Right Y Axis (for Sensor Readings)
    QValueAxis *rightAxisY = new QValueAxis;
    rightAxisY->setTitleText("Sensors [index]");
    rightAxisY->setRange(0, 21);
    pwmChart->addAxis(rightAxisY, Qt::AlignRight);
    sensorSeries->attachAxis(rightAxisY);

    // X Axis
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Time");
    axisX->setRange(0, 20);
    pwmChart->setAxisX(axisX, pwmLSeries);
    pwmChart->setAxisX(axisX, pwmRSeries);
    pwmChart->setAxisX(axisX, sensorSeries);

    pwmChart->legend()->hide(); // Hide legend
    sensorSeries->setMarkerSize(10.0); // Set the marker size as desired
    pwmChartView->setChart(pwmChart);
    pwmChartView->setRenderHint(QPainter::Antialiasing);
    // Speed Chart
    QChart *speedChart = new QChart();
    speedChart->addSeries(speedSeries);
    speedChart->setTitle("Linear Velocity");
    speedChart->createDefaultAxes();
    speedChart->axisX()->setTitleText("Time [s]");
    speedChart->axisY()->setTitleText("Velocity[m/s]");
    speedChart->axisY()->setRange(0, 7.5);
    speedChart->axisX()->setRange(0, basicAxisXTime);
    speedChart->legend()->hide();
    speedChartView->setChart(speedChart);
    speedChartView->setRenderHint(QPainter::Antialiasing);

    // Battery Chart
    QChart *batteryChart = new QChart();
    batteryChart->addSeries(batterySeries);
    batteryChart->setTitle("Battery Voltage");
    batteryChart->createDefaultAxes();
    batteryChart->axisX()->setTitleText("Time [s]");
    batteryChart->axisY()->setTitleText("Battery [V]");
    batteryChart->axisY()->setRange(7.9, 8.5);
    batteryChart->axisX()->setRange(0, basicAxisXTime);
    batteryChart->legend()->hide();
    batteryChartView->setChart(batteryChart);
    batteryChartView->setRenderHint(QPainter::Antialiasing);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;


    mainLayout->addWidget(pwmChartView, 1);


    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(speedChartView, 1);
    bottomLayout->addWidget(batteryChartView, 1);

    mainLayout->addLayout(bottomLayout, 1);

    setLayout(mainLayout);


    setMinimumSize(1000, 900);
}

PlotWindow::~PlotWindow()
{
}

void PlotWindow::updatePlot(float time, int pwmL, int pwmR, const int sensors[], float speed, float battery)
{
    pwmLSeries->append(time, pwmL);
    pwmRSeries->append(time, pwmR);

    for(int i = 0; i < 20; ++i) {
        if(sensors[i] == 1) {
            sensorSeries->append(time, i + 1); // Adding 1 to index to match your requirement
        }
    }

    speedSeries->append(time, speed);
    batterySeries->append(time, battery);

    // Get the current maximum range of the X axis for each chart and update if necessary
    QValueAxis *pwmAxisX = qobject_cast<QValueAxis*>(pwmChartView->chart()->axisX());
    QValueAxis *speedAxisX = qobject_cast<QValueAxis*>(speedChartView->chart()->axisX());
    QValueAxis *batteryAxisX = qobject_cast<QValueAxis*>(batteryChartView->chart()->axisX());

    if (pwmAxisX && time > pwmAxisX->max()) {
        pwmAxisX->setRange(0, time);
    }

    if (speedAxisX && time > speedAxisX->max()) {
        speedAxisX->setRange(0, time);
    }

    if (batteryAxisX && time > batteryAxisX->max()) {
        batteryAxisX->setRange(0, time);
    }
}

void PlotWindow::clearPlot(){
    // Clear data from all series
    QValueAxis *pwmAxisX = qobject_cast<QValueAxis*>(pwmChartView->chart()->axisX());
    QValueAxis *speedAxisX = qobject_cast<QValueAxis*>(speedChartView->chart()->axisX());
    QValueAxis *batteryAxisX = qobject_cast<QValueAxis*>(batteryChartView->chart()->axisX());

    pwmLSeries->clear();
    pwmRSeries->clear();
    sensorSeries->clear();
    speedSeries->clear();
    batterySeries->clear();
    pwmAxisX->setRange(0, basicAxisXTime);
    speedAxisX->setRange(0, basicAxisXTime);
    batteryAxisX->setRange(0, basicAxisXTime);
}


