/*!
 * \file mainwindow.h
 * \brief Plik nagłówkowy klasy MainWindow.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QResizeEvent>
#include <QLabel>
#include <QList>
#include <QElapsedTimer>
#include <QTcpSocket>
#include <QTransform>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include "plotwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \brief Główne okno aplikacji.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy MainWindow.
     * \param[in] parent  Wskaźnik na obiekt rodzica.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /*!
     * \brief Destruktor głównego okna aplikacji.
     */
    ~MainWindow();

    /*!
     * \brief Metoda do wysyłania ramek danych przez WiFi.
     * \param[in] msg Ramka do wysłania.
     */
    void transmit(QString msg);

public slots:
    /*!
     * \brief Slot obsługujący odbiór danych z WiFi.
     */
    void myReadSocket();

private slots:
    /*!
     * \brief Obsługuje zmianę rozmiaru okna.
     * \param[in] event Zdarzenie zmiany rozmiaru.
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Obsługuje kliknięcie przycisku start.
     */
    void on_startButton_clicked();

    /*!
     * \brief Obsługuje kliknięcie przycisku stop.
     */
    void on_stopButton_clicked();

    /*!
     * \brief Obsługuje kliknięcie przycisku aktualizacji.
     */
    void on_updateButton_clicked();

    /*!
     * \brief Obsługuje zmianę wartości paska postępu baterii.
     * \param[in] value Nowa wartość paska postępu.
     */
    void on_batteryProgressBar_valueChanged(int value);

    /*!
     * \brief Obsługuje kliknięcie przycisku wykresów.
     */
    void on_plotsButton_clicked();

    /*!
     * \brief Zmienia język interfejsu.
     * \param[in] language] Ścieżka do pliku tłumaczenia z rozszerzeniem qm.
     */
    void changeLanguage(const QString &language);

private:
    Ui::MainWindow *ui; /*!< Interfejs użytkownika. */
    PlotWindow *plotWindow; /*!< Okno wykresów. */

    // Grafika i etykiety
    QPixmap robot_bkgnd; /*!< Tło dla obrazu robota. */
    QPixmap main_bkgnd; /*!< Główne tło aplikacji. */
    QPixmap compass_bkgnd; /*!< Tło dla kompasu. */
    QPixmap compassPixmap; /*!< Obrazek kompasu. */
    QLabel *compassLabel; /*!< Etykieta wyświetlająca kompas. */


    // Elementy interfejsu sieciowego
    QTcpSocket socket; /*!< Socket TCP. */

    // Tłumaczenie interfejsu
    QTranslator translator; /*!< Tłumacz interfejsu. */

    QList<QFrame *> frameList; /*!< Lista obiektów QFrame (czujników odbiciowych). */

    // Dane robota i aplikacji
    int status; /*!< Stan robota. */
    int sensors[20]; /*!< Dane z czujników odbiciowych. */
    int pwm_L; /*!< PWM dla lewego silnika. */
    int pwm_R; /*!< PWM dla prawego silnika. */
    float w_L; /*!< Prędkość kątowa lewego koła. */
    float w_R; /*!< Prędkość kątowa prawego koła. */
    int z_rotation; /*!< Pozycja robota względem osi Z. */
    int battery; /*!< Poziom naładowania baterii. */

    // Dane do wysłania
    char instruction; /*!< Instrukcja do wysłania do robota. */
    int data_to_send[4]; /*!< Dane do wysłania. */

    // Dane obliczone
    float left_linear_V; /*!< Aktualna prędkość liniowa lewego koła. */
    float right_linear_V; /*!< Aktualna prędkość liniowa prawego koła. */
    float linear_velocity; /*!< Prędkość liniowa. */
    float max_linear_velocity; /*!< Maksymalna prędkość liniowa. */
    float average_velocity; /*!< Średnia prędkość liniowa. */
    float distanceFloat; /*!< Przebyty dystans. */
    float time; /*!< Czas trwania. */

    // Metody prywatne
    /*!
     * \brief Przetwarza otrzymane dane z WiFi.
     * \param[in] input Dane wejściowe odczytane z WiFi.
     */
    void cutString(const QString &input);

    /*!
     * \brief Tworzy ramkę danych do wysłania przez WiFi.
     * \param[in] instruction Instrukcja do wysłania do robota.
     * \return Ramka danych.
     */
    QString makeDataFrame(char instruction);

    /*!
     * \brief Wyświetla kompas.
     */
    void displayCompass();

    /*!
     * \brief Wyświetla stan baterii.
     */
    void displayBattery();

    /*!
     * \brief Wyświetla dane z czujników.
     */
    void displaySensors();

    /*!
     * \brief Wyświetla dane z enkoderów.
     */
    void displayEncoders();

    /*!
     * \brief Rysuje strzałkę na etykiecie.
     * \param[in] label - Etykieta, na której ma być narysowana strzałka.
     * \param[in] velocity Prędkość, reprezentowana przez długość strzałki.
     */
    void drawArrow(QLabel *label, float velocity);

    /*!
     * \brief Wyświetla wszystkie dane.
     */
    void displayData();

    /*!
     * \brief Wyświetla statystyki.
     */
    void displayStats();



};
#endif // MAINWINDOW_H
