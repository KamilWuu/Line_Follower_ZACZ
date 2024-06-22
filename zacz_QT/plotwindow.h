/*!
 * \file plotwindow.h
 * \brief Plik nagłówkowy klasy PlotWindow.
 */

#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include <QtCharts>

#define basicAxisXTime 10 /*!< Podstawowy czas osi X dla wykresów. */
#define basicAxisYVelocity 0.2 /*!< Poczatkowa wartosc osi Y dla wykresu predkosci. */
/*!
 * \brief Klasa PlotWindow odpowiada za wyświetlanie wykresów.
 */
class PlotWindow : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy PlotWindow.
     * \param[in] parent  Wskaźnik na obiekt rodzica.
     */
    explicit PlotWindow(QWidget *parent = nullptr);

    /*!
     * \brief Destruktor klasy PlotWindow.
     */
    ~PlotWindow();

    /*!
     * \brief Metoda do tłumaczenia interfejsu użytkownika.
     */
    void retranslateUi();

    /*!
     * \brief Aktualizuje wykresy danymi.
     * \param[in] time Czas.
     * \param[in] pwmL Wartość PWM dla lewego silnika.
     * \param[in] pwmR  Wartość PWM dla prawego silnika.
     * \param[in] sensors  Tablica z danymi czujników.
     * \param[in] velocity Prędkość.
     */
    void updatePlot(float time, int pwmL, int pwmR, const int sensors[], float velocity);

    /*!
     * \brief Czyści wykresy.
     */
    void clearPlot();

private:
    QChartView *pwmChartView; /*!< Widok wykresu dla PWM. */
    QChartView *sensorChartView; /*!< Widok wykresu dla czujników. */
    QChartView *velocityChartView; /*!< Widok wykresu dla prędkości. */

    QLineSeries *pwmLSeries; /*!< Seria danych dla PWM lewego silnika. */
    QLineSeries *pwmRSeries; /*!< Seria danych dla PWM prawego silnika. */
    QScatterSeries *sensorSeries; /*!< Seria danych dla czujników. */

    QLineSeries *velocitySeries; /*!< Seria danych dla prędkości. */

};

#endif // PLOTWINDOW_H
