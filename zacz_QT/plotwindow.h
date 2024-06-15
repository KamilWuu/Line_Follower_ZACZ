/*!
 * \file plotwindow.h
 * \brief Plik nagłówkowy klasy PlotWindow.
 */

#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include <QtCharts>

#define basicAxisXTime 10 /*!< Podstawowy czas osi X dla wykresów. */

/*!
 * \brief Klasa PlotWindow odpowiada za wyświetlanie wykresów.
 */
class PlotWindow : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy PlotWindow.
     * \param parent Wskaźnik na obiekt rodzica.
     */
    explicit PlotWindow(QWidget *parent = nullptr);

    /*!
     * \brief Destruktor klasy PlotWindow.
     */
    ~PlotWindow();

    /*!
     * \brief Aktualizuje wykresy danymi.
     * \param time Czas.
     * \param pwmL Wartość PWM dla lewego silnika.
     * \param pwmR Wartość PWM dla prawego silnika.
     * \param sensors Tablica z danymi czujników.
     * \param speed Prędkość.
     * \param battery Poziom naładowania baterii.
     */
    void updatePlot(float time, int pwmL, int pwmR, const int sensors[], float speed, float battery);

    /*!
     * \brief Czyści wykresy.
     */
    void clearPlot();

private:
    QChartView *pwmChartView; /*!< Widok wykresu dla PWM. */
    QChartView *speedChartView; /*!< Widok wykresu dla prędkości. */
    QChartView *batteryChartView; /*!< Widok wykresu dla baterii. */

    QLineSeries *pwmLSeries; /*!< Seria danych dla PWM lewego silnika. */
    QLineSeries *pwmRSeries; /*!< Seria danych dla PWM prawego silnika. */
    QScatterSeries *sensorSeries; /*!< Seria danych dla czujników. */

    QLineSeries *speedSeries; /*!< Seria danych dla prędkości. */
    QLineSeries *batterySeries; /*!< Seria danych dla baterii. */
};

#endif // PLOTWINDOW_H
