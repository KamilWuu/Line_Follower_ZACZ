#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QDialog(parent),
    pwmChartView(new QChartView(this)),
    velocityChartView(new QChartView(this)),
    sensorChartView(new QChartView(this)),  // Create new chart view for sensor readings
    pwmLSeries(new QLineSeries),
    pwmRSeries(new QLineSeries),
    sensorSeries(new QScatterSeries),
    velocitySeries(new QLineSeries)
{
    // PWM Chart
    QChart *pwmChart = new QChart();
    pwmChart->addSeries(pwmLSeries);
    pwmChart->addSeries(pwmRSeries);
    pwmChart->setTitle(tr("PWM Values"));
    pwmChart->createDefaultAxes();

    // Left Y Axis (for PWM)
    QValueAxis *leftAxisY = new QValueAxis;
    leftAxisY->setTitleText(tr("PWM Values [%]"));
    leftAxisY->setRange(0, 100);
    pwmChart->setAxisY(leftAxisY, pwmLSeries);
    pwmChart->setAxisY(leftAxisY, pwmRSeries);

    // X Axis
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText(tr("Time [s]"));
    axisX->setRange(0, basicAxisXTime);
    pwmChart->setAxisX(axisX, pwmLSeries);
    pwmChart->setAxisX(axisX, pwmRSeries);

    pwmChart->legend()->hide(); // Hide legend
    pwmChartView->setChart(pwmChart);
    pwmChartView->setRenderHint(QPainter::Antialiasing);

    // Sensor Chart
    QChart *sensorChart = new QChart();
    sensorChart->addSeries(sensorSeries);
    sensorChart->setTitle(tr("Sensor Readings"));
    sensorChart->createDefaultAxes();

    // Y Axis (for Sensor Readings)
    QValueAxis *sensorAxisY = new QValueAxis;
    sensorAxisY->setTitleText(tr("Sensors [index]"));
    sensorAxisY->setRange(0, 20);
    sensorChart->setAxisY(sensorAxisY, sensorSeries);

    QValueAxis *sensorAxisX = new QValueAxis;  // Create a new X axis for the sensor chart
    sensorAxisX->setTitleText(tr("Time [s]"));
    sensorAxisX->setRange(0, basicAxisXTime);
    sensorChart->setAxisX(sensorAxisX, sensorSeries);

    sensorChart->legend()->hide();
    sensorChartView->setChart(sensorChart);
    sensorChartView->setRenderHint(QPainter::Antialiasing);

    // Velocity Chart
    QChart *velocityChart = new QChart();
    velocityChart->addSeries(velocitySeries);
    velocityChart->setTitle(tr("Linear Velocity"));
    velocityChart->createDefaultAxes();
    velocityChart->axisX()->setTitleText(tr("Time [s]"));
    velocityChart->axisY()->setTitleText(tr("Velocity [m/s]"));
    velocityChart->axisY()->setRange(0, 4);
    velocityChart->axisX()->setRange(0, basicAxisXTime);
    velocityChart->legend()->hide();
    velocityChartView->setChart(velocityChart);
    velocityChartView->setRenderHint(QPainter::Antialiasing);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sensorChartView, 1);  // Add sensor chart to layout
    mainLayout->addWidget(pwmChartView, 1);
    mainLayout->addWidget(velocityChartView, 1);  // Move velocity chart to the main layout

    setLayout(mainLayout);
    setMinimumSize(1000, 900);
}

PlotWindow::~PlotWindow()
{
}

void PlotWindow::retranslateUi()
{
    this->setWindowTitle(tr("Plot Window"));

    // Update titles and labels for each chart
    QChart *pwmChart = pwmChartView->chart();
    QChart *sensorChart = sensorChartView->chart();
    QChart *velocityChart = velocityChartView->chart();

    pwmChart->setTitle(tr("PWM Values"));
    static_cast<QValueAxis*>(pwmChart->axisY())->setTitleText(tr("PWM Values [%]"));
    static_cast<QValueAxis*>(pwmChart->axisX())->setTitleText(tr("Time [s]"));

    sensorChart->setTitle(tr("Sensor Readings"));
    static_cast<QValueAxis*>(sensorChart->axisY())->setTitleText(tr("Sensors [index]"));
    static_cast<QValueAxis*>(sensorChart->axisX())->setTitleText(tr("Time [s]"));

    velocityChart->setTitle(tr("Linear Velocity"));
    static_cast<QValueAxis*>(velocityChart->axisY())->setTitleText(tr("Velocity [m/s]"));
    static_cast<QValueAxis*>(velocityChart->axisX())->setTitleText(tr("Time [s]"));
}

void PlotWindow::updatePlot(float time, int pwmL, int pwmR, const int sensors[], float velocity)
{
    pwmLSeries->append(time, pwmL);
    pwmRSeries->append(time, pwmR);

    for(int i = 0; i < 20; ++i) {
        if(sensors[i] == 1) {
            sensorSeries->append(time, i + 1);
        }
    }

    velocitySeries->append(time, velocity);

    QValueAxis *pwmAxisX = qobject_cast<QValueAxis*>(pwmChartView->chart()->axisX());
    QValueAxis *sensorAxisX = qobject_cast<QValueAxis*>(sensorChartView->chart()->axisX());
    QValueAxis *velocityAxisX = qobject_cast<QValueAxis*>(velocityChartView->chart()->axisX());

    if (pwmAxisX && time > pwmAxisX->max()) {
        pwmAxisX->setRange(time - basicAxisXTime , time);
    }

    if (sensorAxisX && time > sensorAxisX->max()) {
        sensorAxisX->setRange(time - basicAxisXTime, time);
    }

    if (velocityAxisX && time > velocityAxisX->max()) {
        velocityAxisX->setRange(time - basicAxisXTime, time);
    }
}

void PlotWindow::clearPlot()
{
    QValueAxis *pwmAxisX = qobject_cast<QValueAxis*>(pwmChartView->chart()->axisX());
    QValueAxis *sensorAxisX = qobject_cast<QValueAxis*>(sensorChartView->chart()->axisX());
    QValueAxis *velocityAxisX = qobject_cast<QValueAxis*>(velocityChartView->chart()->axisX());

    pwmLSeries->clear();
    pwmRSeries->clear();
    sensorSeries->clear();
    velocitySeries->clear();
    pwmAxisX->setRange(0, basicAxisXTime);
    sensorAxisX->setRange(0, basicAxisXTime);
    velocityAxisX->setRange(0, basicAxisXTime);
}
