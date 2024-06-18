#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QDialog(parent),
    pwmChartView(new QChartView(this)),
    velocityChartView(new QChartView(this)),
    sensorChartView(new QChartView(this)),
    pwmLSeries(new QLineSeries),
    pwmRSeries(new QLineSeries),
    sensorSeries(new QScatterSeries),
    velocitySeries(new QLineSeries)
{
    // PWM Chart
    QChart *pwmChart = new QChart();
    pwmLSeries->setName(tr("PWM Left"));
    pwmRSeries->setName(tr("PWM Right"));
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

    // Show legend
    pwmChart->legend()->setVisible(false);
    pwmChart->legend()->setAlignment(Qt::AlignBottom);

    pwmChartView->setChart(pwmChart);
    pwmChartView->setRenderHint(QPainter::Antialiasing);

    // Sensor Chart
    QChart *sensorChart = new QChart();
    sensorChart->addSeries(sensorSeries);
    sensorChart->setTitle(tr("Sensor Readings"));
    sensorChart->createDefaultAxes();

    // Y Axis (for Sensor Readings)
    QValueAxis *sensorAxisY = new QValueAxis;
    sensorSeries->setMarkerSize(3.0);
    sensorSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    sensorSeries->setColor(Qt::black);
    sensorSeries->setBorderColor(Qt::black);
    sensorAxisY->setTitleText(tr("Sensors [index]"));
    sensorAxisY->setRange(0, 20);
    sensorChart->setAxisY(sensorAxisY, sensorSeries);

    QValueAxis *sensorAxisX = new QValueAxis;
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
    velocityChart->axisY()->setRange(0, basicAxisYVelocity);
    velocityChart->axisX()->setRange(0, basicAxisXTime);
    velocityChart->legend()->hide();
    velocityChartView->setChart(velocityChart);
    velocityChartView->setRenderHint(QPainter::Antialiasing);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sensorChartView, 1);
    mainLayout->addWidget(pwmChartView, 1);
    mainLayout->addWidget(velocityChartView, 1);

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
    QValueAxis *velocityAxisY = qobject_cast<QValueAxis*>(velocityChartView->chart()->axisY());

    if (pwmAxisX && time > pwmAxisX->max()) {
        pwmAxisX->setRange(time - basicAxisXTime , time);
    }

    if (sensorAxisX && time > sensorAxisX->max()) {
        sensorAxisX->setRange(time - basicAxisXTime, time);
    }

    if (velocityAxisX && time > velocityAxisX->max()) {
        velocityAxisX->setRange(time - basicAxisXTime, time);
    }

    if(velocityAxisY && velocity > velocityAxisY->max()){
        velocityAxisY->setRange(0, velocity*2);
    }
}

void PlotWindow::clearPlot()
{
    QValueAxis *pwmAxisX = qobject_cast<QValueAxis*>(pwmChartView->chart()->axisX());
    QValueAxis *sensorAxisX = qobject_cast<QValueAxis*>(sensorChartView->chart()->axisX());
    QValueAxis *velocityAxisX = qobject_cast<QValueAxis*>(velocityChartView->chart()->axisX());
    QValueAxis *velocityAxisY = qobject_cast<QValueAxis*>(velocityChartView->chart()->axisY());

    pwmLSeries->clear();
    pwmRSeries->clear();
    sensorSeries->clear();
    velocitySeries->clear();
    pwmAxisX->setRange(0, basicAxisXTime);
    sensorAxisX->setRange(0, basicAxisXTime);
    velocityAxisX->setRange(0, basicAxisXTime);
    velocityAxisY->setRange(0, basicAxisYVelocity);
}
