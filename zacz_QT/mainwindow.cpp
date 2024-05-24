#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QResizeEvent>

#include <QLabel>
#include <QDebug>
#include <QTcpSocket>

QElapsedTimer timer1;
QElapsedTimer distance_timer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , robot_bkgnd("/home/kamil/Applications/LineFollower/images/background.png")
    , main_bkgnd("/home/kamil/Applications/LineFollower/images/main_background.png")
    , compas_bkgnd("/home/kamil/Applications/LineFollower/images/kompas.png")


{
    ui->setupUi(this);

    /*COMPORT = new QSerialPort();
    COMPORT -> setPortName("ttyUSB0");
    COMPORT -> setBaudRate(QSerialPort::BaudRate::Baud9600);
    COMPORT -> setParity(QSerialPort::Parity::NoParity);
    COMPORT -> setDataBits(QSerialPort::DataBits::Data8);
    COMPORT -> setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT -> setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT ->open(QIODevice::ReadWrite);

    if(COMPORT->isOpen()){
        qDebug() << "Serial port is connected";
        qDebug() << COMPORT->error();
        ui->statusLabel_1->setText("ROBOT IS CONNECTED");
        ui->statusLabel_1->setStyleSheet("QLabel { color : green; }");
    }
    else
    {
        qDebug() << "Serial Port Is Not Connected";
        qDebug() << COMPORT->error();
        ui->statusLabel_1->setText("ROBOT IS DISCONNECTED");
        ui->statusLabel_1->setStyleSheet("QLabel { color : red; }");
    }

    connect(COMPORT, SIGNAL(readyRead()), this, SLOT(readData()));*/


    connect(&socket, &QTcpSocket::readyRead,this, &MainWindow::myReadSocket);
    socket.connectToHost("10.42.0.44",8888);



    QPixmap main_scaled = main_bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette main_palette;
    main_palette.setBrush(QPalette::Window, main_scaled);

    this->setPalette(main_palette);

    connect(this, SIGNAL(resized()), this, SLOT(resizeEvent()));


    QPixmap robot_scaled = robot_bkgnd.scaled(this->ui->robot_frame->size(), Qt::IgnoreAspectRatio);
    QPalette robot_palette;
    robot_palette.setBrush(QPalette::Window, robot_scaled);

    this->ui->robot_frame->setPalette(robot_palette);

    connect(this->ui->robot_frame, SIGNAL(resized()), this, SLOT(resizeEvent()));


    QPixmap compas_scaled = compas_bkgnd.scaled(this->ui->compas_frame->size(), Qt::IgnoreAspectRatio);
    QPalette compas_palette;
    compas_palette.setBrush(QPalette::Window, compas_scaled);

    this->ui->compas_frame->setPalette(compas_palette);

    connect(this->ui->compas_frame, SIGNAL(resized()), this, SLOT(resizeEvent()));


    // Dodawanie istniejących obiektów QFrame do listy
    frameList.append(ui->sensor_1);
    frameList.append(ui->sensor_2);
    frameList.append(ui->sensor_3);
    frameList.append(ui->sensor_4);
    frameList.append(ui->sensor_5);
    frameList.append(ui->sensor_6);
    frameList.append(ui->sensor_7);
    frameList.append(ui->sensor_8);
    frameList.append(ui->sensor_9);
    frameList.append(ui->sensor_10);
    frameList.append(ui->sensor_11);
    frameList.append(ui->sensor_12);
    frameList.append(ui->sensor_13);
    frameList.append(ui->sensor_14);
    frameList.append(ui->sensor_15);
    frameList.append(ui->sensor_16);
    frameList.append(ui->sensor_17);
    frameList.append(ui->sensor_18);
    frameList.append(ui->sensor_21);
    frameList.append(ui->sensor_22);

    foreach(QFrame* frame, frameList) {
        frame->setStyleSheet("background-color: white;"); // Ustawienie koloru tła dla każdego QFrame
    }


    ui->batteryProgressBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    ui->leftPWMProgresBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    ui->rightPWMProgresBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");

    ui->velocityLCD->display(0);
    ui->vMaxLCD->display(0);
    ui->timeLCD->display(0);
    ui->distanceLCD->display(0);

    max_linear_velocity = 0;
    time = 0;
    linear_velocity = 0;
    distanceFloat = 0.0;
    average_velocity = 0;
}

MainWindow::~MainWindow()
{
    if (COMPORT->isOpen()) {
        COMPORT->close();
    }
    delete COMPORT;
    delete ui;

}



void MainWindow::resizeEvent(QResizeEvent * event){

    QPixmap main_scaled = main_bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);

    QPalette main_palette;
    main_palette.setBrush(QPalette::Window, main_scaled);
    this->setPalette(main_palette);

    QPixmap robot_scaled = robot_bkgnd.scaled(this->ui->robot_frame->size(), Qt::IgnoreAspectRatio);

    QPalette robot_palette;
    robot_palette.setBrush(QPalette::Window, robot_scaled);
    this->ui->robot_frame->setPalette(robot_palette);

    QPixmap compas_scaled = compas_bkgnd.scaled(this->ui->compas_frame->size(), Qt::IgnoreAspectRatio);

    QPalette compas_palette;
    compas_palette.setBrush(QPalette::Window, compas_scaled);
    this->ui->compas_frame->setPalette(compas_palette);
}


void MainWindow::cutString(const QString& input) {
    if (!input.startsWith('$') || !input.endsWith("#\r\n")) {
        // Nieprawidłowy format ramki danych
        return;
    }

    QString data = input.mid(1, input.length() - 4); // Usunięcie znaków '$', '#' oraz "\r\n"
    //qDebug() << "data bez znakow " << data;
    QStringList tokens = data.split('x');
    int index = 0;

    for (const QString& token : tokens) {
        switch (index) {
        case 0:
            status = token.toInt();
            //qDebug() << "status " << status;
            break;
        case 1:

            for(int j = 0; j < 20; j++){
                 sensors[j] = token.at(j).digitValue(); // Konwersja QChar na int
            }
            break;
        case 2:
            pwm_L = token.toInt();
            break;
        case 3:
            pwm_R = token.toInt();
            break;
        case 4:
            w_L = token.toInt();
            break;
        case 5:
            w_R = token.toInt();
            break;
        case 6:
            z_rotation = token.toInt();
            break;
        case 7:
            battery = token.toInt();
            break;
        }
        index++;
    }
}


void MainWindow::on_connectButton_clicked()
{
    qDebug() << "Connect button clicked";
    transmit("prawda wiedza rezulataty\n");
    /*
    // Sprawdź, czy port jest już otwarty
    if (COMPORT->isOpen()) {
        // Zamykamy port, aby można było go ponownie otworzyć
        COMPORT->close();
        qDebug() << "Serial port was already open. Closed.";
    }

    // Otwórz połączenie z portem COM
    if (COMPORT->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port is connected";
        qDebug() << COMPORT->error();
        ui->statusLabel_1->setText("ROBOT IS CONNECTED");
        ui->statusLabel_1->setStyleSheet("QLabel { color : green; }");
    } else {
        qDebug() << "Failed to connect to serial port";
        qDebug() << COMPORT->error();
        ui->statusLabel_1->setText("ROBOT IS DISCONNECTED");
        ui->statusLabel_1->setStyleSheet("QLabel { color : red; }");
    }*/
}

void MainWindow::transmit(QString msg){
    if(socket.state()!=QAbstractSocket::ConnectedState){
        return;
    }
    socket.write(msg.toUtf8());
    socket.flush();
    socket.waitForBytesWritten(1000);
}

void MainWindow::myReadSocket(){
    QByteArray data = socket.readAll();
    qDebug() << "data: " << data;
}

void MainWindow::readData()
{
    if(COMPORT->isOpen())
    {
        while(COMPORT->bytesAvailable()){
            data_from_serialPort += COMPORT->readAll();
            if(data_from_serialPort.at(data_from_serialPort.length() - 1) == char(10)){
                is_data_received = true;
            }
        }
        if(is_data_received == true){
            //qDebug() << "data froms serial: " << data_from_serialPort;
            ui->label_10->setText(data_from_serialPort);
            cutString(data_from_serialPort);
            displayData();
            data_from_serialPort = "";
            is_data_received = false;

        }
        ui->statusLabel_1->setText("ROBOT IS CONNECTED");
        ui->statusLabel_1->setStyleSheet("QLabel { color : green; }");
    }else{
        ui->statusLabel_1->setText("ROBOT IS DISCONNECTED");
        ui->statusLabel_1->setStyleSheet("QLabel { color : red; }");
    }
}


/*






*/








QByteArray MainWindow::makeDataFrame(char instruction)
{

    QString frame, data_string[4];

    for(int i = 0; i < 4; i++){
        if(data_to_send[i] > 99){
            data_string[i] = QString::number(data_to_send[i]);
        }else if(data_to_send[i] > 9){
            data_string[i] = "0" + QString::number(data_to_send[i]);
        }else if(data_to_send[i] > 0){
            data_string[i] = "00" + QString::number(data_to_send[i]);
        }else{
            qDebug() << "Wystapill blad przy wartosci data_to_send [ " << i << " ]" << "== " << data_to_send[i];
        }
    }


    frame = "$";
    frame += instruction;

    for(int i = 0; i < 4; i++){
        frame += "x";
        frame += data_string[i];
    }

    frame += "#";

    qDebug() << "utworzona ramka danych to: " << frame.toUtf8();

    return frame.toUtf8();
}



void MainWindow::on_startButton_clicked()
{
    if(COMPORT->isOpen()){
        COMPORT->write(makeDataFrame('S') + char(10) );
        qDebug() << "ins S, data frame from Qt" << makeDataFrame('S');
    }else{
        qDebug() << "cant send start 'S', COMPORT is closed";
    }


}


void MainWindow::on_stopButton_clicked()
{
    if(COMPORT->isOpen()){
        COMPORT->write(makeDataFrame('M') + char(10));
        qDebug() << "ins M, data frame from Qt : " << makeDataFrame('M');
    }else{
        qDebug() << "cant send stop 'M', COMPORT is closed";
    }

}


void MainWindow::on_updateButton_clicked()
{
    bool ok[4];

    int data_from_line_edit[4];

    QString coms[4];

    coms[0] = "PID_P";
    coms[1] = "PID_I";
    coms[2] = "PID_D";
    coms[3] = "V_MAX";


    data_from_line_edit[0] = ui->pidPLineEdit->text().toInt(&ok[0]);
    data_from_line_edit[1] = ui->pidILineEdit->text().toInt(&ok[1]);
    data_from_line_edit[2] = ui->pidDLineEdit->text().toInt(&ok[2]);
    data_from_line_edit[3] = ui->vMaxLineEdit->text().toInt(&ok[3]);

    for(int i = 0; i < 4; i++ ){
            qDebug() << "ODCZYTANO: " << data_from_line_edit[i];
    }

    for(int i = 0; i < 4; i++ ){
        if(!ok[i]){
            qDebug() << "blad przy odczycie: " << coms[i];
            return;
        }
    }


    for(int i = 0; i < 4; i++){
        if(data_from_line_edit[i] < 1000 ){
            if(data_from_line_edit[i] >= 0 ){

                if(i == 3){
                    if(data_from_line_edit[i] > 100){
                        qDebug() << "Wpisywany parametr: " << coms[i] << " maksymalnie moze wynosic 100";
                        return;
                    }
                }

                data_to_send[i] = data_from_line_edit[i];

                qDebug() << coms[i] << "data to send: " << data_to_send[i] << " data from line edit : " << data_from_line_edit[i];
            }
            else{
                qDebug() << "Wpisywany parametr: " << coms[i] << " nie moze byc ujemny!";
                return;
            }

        }else{
            qDebug() << "Wpisywany parametr: " << coms[i] << " moze maksymalnie miec 3 znaki";
            return;
        }
    }


    for(int i = 0; i < 4; i++ ){
        qDebug() << "parameter to send: " << coms[i] << "= " << data_to_send[i] ;
    }


    if(COMPORT->isOpen()){
        COMPORT->write(makeDataFrame('U') + char(10));
        qDebug() << "ins U, data frame from Qt" << makeDataFrame('U');
    }
}


void MainWindow::on_batteryProgressBar_valueChanged(int value)
{
    /*if(value < 14){
       QMessageBox::critical(this, "BATTERY WARNING!", "Battery is less than 15%. Please charge!");
    }*/
    if (value < 35) {
        ui->batteryProgressBar->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    } else if (value < 60) {
        ui->batteryProgressBar->setStyleSheet("QProgressBar::chunk { background-color: yellow; }");
    } else {
        ui->batteryProgressBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    }
}

void MainWindow::displayBattery()
{
    //battery

    #define MIN_VOLTAGE 7.6
    #define MAX_VOLTAGE 8.4
    #define VOLTAGE_DIVIDER 3
    #define MAX_ADC_VOLTAGE 2.8 // MAX_VOLATAGE/VOLTAGE_DIVIDER
    #define MAX_ADC_VALUE 3475 // 4096*MAX_ADC_VALUE/3.3

    //qDebug() << "bateria" << battery;
    float battery_voltage = (battery * MAX_ADC_VOLTAGE * VOLTAGE_DIVIDER)/MAX_ADC_VALUE;
    //qDebug() << "voltage " << battery_voltage;
    int battery_percent = 100 - (((MAX_VOLTAGE - battery_voltage) * 100)/(MAX_VOLTAGE - MIN_VOLTAGE));
    ui->batteryProgressBar->setValue(battery_percent);
    //qDebug() << "procent " << battery_percent;
    ui->batteryVoltageLabel->setText((QString::number(battery_voltage, 'f', 2)) + "V" );
    ui->batteryPercentLabel->setText(QString::number(battery_percent)+ "%" );

}



void MainWindow::displaySensors()
{
    int index = 0;
    foreach(QFrame* frame, frameList) {
        // Tutaj możesz wykonywać operacje na każdym obiekcie QFrame
        // Na przykład:
        if(sensors[index]){
            frame->setStyleSheet("background-color: black;"); // Ustawienie koloru tła dla każdego QFrame
        }else{
            frame->setStyleSheet("background-color: white;"); // Ustawienie koloru tła dla każdego QFrame
        }

        index++;
    }

    foreach(QFrame* frame, frameList) {
        // Tutaj możesz wykonywać operacje na każdym obiekcie QFrame
        // Na przykład:

            frame->setStyleSheet("background-color: white;"); // Ustawienie koloru tła dla każdego QFrame


        index++;
    }
    frameList[10]->setStyleSheet("background-color: black;"); // Ustawienie koloru tła dla każdego QFrame
}

void MainWindow::displayEncoders()
{
    int wheel_radius = 10; //mm

    float left_linear_V;
    float right_linear_V;


    ui->leftAxialVelocityLabel->setText(QString::number(w_L) + "RAD/S");
    ui->rightAxialVelocityLabel->setText(QString::number(w_R) + "RAD/S");

    left_linear_V = wheel_radius*w_L/1000;
    right_linear_V = wheel_radius*w_R/1000;



    ui->leftLinearVelocityLabel->setText(QString::number(left_linear_V, 'f',2 ) + "M/S");
    ui->rightLinearVelocityLabel->setText(QString::number(right_linear_V, 'f',2  ) + "M/S");

    this->linear_velocity = (left_linear_V + right_linear_V)/2;

}


void MainWindow::displayStats()
{

    if(linear_velocity > max_linear_velocity){
        max_linear_velocity = linear_velocity;
    }

    ui->velocityLCD->display(QString::number(linear_velocity, 'f', 2));
     ui->vMaxLCD->display(QString::number(max_linear_velocity, 'f', 2));
    //ui->vMaxLCD->display(max_linear_velocity);

    if(average_velocity != 0){
        average_velocity = (average_velocity + linear_velocity)/2;
    }else{
        average_velocity = linear_velocity;
    }

    //time

    if(status && !timer1.isValid()){
        timer1.start();
        max_linear_velocity = 0;
        time=0.0;
        average_velocity = 0;
        ui->timeLCD->display(0); //time in s
        ui->velocityLCD->display(0);
        ui->vMaxLCD->display(0);
        ui->distanceLCD->display(0);

    }else if(!status && timer1.isValid()){
        timer1.invalidate();
    }else if(timer1.isValid()){
        time = timer1.elapsed();//1000; time in ms
    }
    ui->timeLCD->display(QString::number(time/1000, 'f', 2)); //time in s



    if(status){
         ui->averageVelocityLCD->display(QString::number(average_velocity, 'f', 2));
        qint64 timeQuant = distance_timer.elapsed();
        distance_timer.invalidate();

        qfloat16 distanceQuant = timeQuant*linear_velocity;

        distanceFloat += distanceQuant;



        ui->distanceLCD->display(QString::number(distanceFloat/1000, 'f', 2));



        distance_timer.start();
    }else{
        distance_timer.start();
        distanceFloat = 0;
    }

    /*/*/

    //distance =
}


void MainWindow::displayData(){

    if(status){
        ui->statusLabel_2->setText("ROBOT IS MOVING");
        ui->statusLabel_2->setStyleSheet("QLabel { color : green; }");
    }else{
        ui->statusLabel_2->setText("ROBOT IS NOT MOVING");
        ui->statusLabel_2->setStyleSheet("QLabel { color : red; }");
    }
    //PWMs
    ui->leftPWMProgresBar->setValue(pwm_L);
    ui->leftPWMLabel->setText(QString::number(pwm_L) + "%");

    ui->rightPWMProgresBar->setValue(pwm_R);
    ui->rightPWMLabel->setText(QString::number(pwm_R)+ "%");


    displayBattery();
    displaySensors();
    displayEncoders();
    displayStats();

    //

}



