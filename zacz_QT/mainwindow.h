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
#include <QtSerialPort>
#include <QLabel>
#include <QList>
#include <QElapsedTimer>
#include <QTcpSocket>
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

    QPixmap robot_bkgnd; /*!< Tło dla obrazu robota. */
    QPixmap main_bkgnd; /*!< Główne tło aplikacji. */
    QPixmap compas_bkgnd; /*!< Tło dla kompasu. */
    QLabel* label_10 = new QLabel( this ); /*!< Etykieta wyswietlajaca odebrana ramke danych. */
    QTcpSocket socket;


public:
    /*!
    * \brief Konstruktor klasy MainWindow.
    * \param parent Wskaźnik na obiekt rodzica.
    */
    explicit MainWindow(QWidget *parent = nullptr);

    /*!
     * \brief Destruktor głównego okna aplikacji.
     */
    ~MainWindow();
public slots:
    void transmit(QString msg);
private slots:
    /*!
     * \brief Obsługuje zmianę rozmiaru okna.
     * \param event Zdarzenie zmiany rozmiaru.
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Odczytuje dane z portu szeregowego/bluetooth.

    void readData();*/

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
     * \brief Zmienia kolor wypelnienia baterii.
     * \param value Nowa wartość paska postępu.
     */
    void on_batteryProgressBar_valueChanged(int value);

    /*!
     * \brief Obsługuje kliknięcie przycisku połączenia.
     */
    void on_connectButton_clicked();

private:
    Ui::MainWindow *ui; /*!< Interfejs użytkownika. */
    PlotWindow *plotWindow; // Add a member for PlotWindow
    QSerialPort * COMPORT; /*!< Port szeregowy. */
    //QString data_from_serialPort; /*!< Dane z portu szeregowego. */
    bool is_data_received = false; /*!< Flaga otrzymania danych. */

    /*!
     * \brief Przetwarza otrzymane dane z portu szeregowego.
     * \param input Dane wejściowe odczytane z robota.
     */
    void cutString(const QString& input);

    /*!
     * \brief Tworzy ramkę danych do wysłania przez port szeregowy.
     * \param instruction Instrukcja ktora ma zostac przeslana do robota.
     * \return Ramka danych.
     */
    QString makeDataFrame(char instruction);

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
     * \brief Wyświetla wszystkie dane.
     */
    void displayData();

    /*!
     * \brief Wyświetla statystyki.
     */
    void displayStats();
    //cleandata from serialPort

    QList<QFrame*> frameList; /*!< Lista obiektow QFrame (czujnikow odbiciowych). */

    int status; /*!< Stan w jakim znajduje sie robot. */
    int sensors[20]; /*!< Dane z czujników odbiowych. */
    int pwm_L; /*!< PWM dla lewego silnika. */
    int pwm_R; /*!< PWM dla prawego silnika. */
    float w_L; /*!< Prędkość kątowa lewego koła. */
    float w_R; /*!< Prędkość kątowa prawego koła. */
    int z_rotation; /*!< Pozycja robota wzgledem osi Z. */
    int battery; /*!< Odczyt napiecia baterii. */

    //data to send
    char instruction; /*!< Instrukcja do przekazania do robota */
    int data_to_send[4]; /*!< Dane do wysłania. */

    //calculated data
    float linear_velocity; /*!< Prędkość liniowa. */
    float max_linear_velocity; /*!< Maksymalna prędkość liniowa. */
    float average_velocity; /*!< Średnia prędkość liniowa. */
    float distanceFloat; /*!< Przebyty dystans. */
    float time; /*!< Czas trwania. */

private slots:
    void myReadSocket();
    void on_plotsButton_clicked();
};

#endif // MAINWINDOW_H
