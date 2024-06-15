#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QResizeEvent>

#include <QLabel>
#include <QDebug>
#include <QTcpSocket>
#include <QWindow>
#include <QLocale>
#include <QTranslator>

#define ESP_IP "10.42.0.44" // ZACZ IP = "10.42.0.44" || TEST_ESP IP = "10.42.0.203"
#define TCP_PORT 8888
QElapsedTimer timer1;
QElapsedTimer distance_timer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plotWindow(new PlotWindow(this)) // Initialize PlotWindow
    , robot_bkgnd("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/images/background.png")
    , main_bkgnd("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/images/main_background.png")
    , compass_bkgnd("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/images/kompas.png")


{
    ui->setupUi(this);


    connect(ui->englishButton, &QPushButton::clicked, this, [this]() {
        changeLanguage("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/LineFollower_en_150.qm");
        ui->englishButton->setEnabled(false);
        ui->polishButton->setEnabled(true);
    });

    connect(ui->polishButton, &QPushButton::clicked, this, [this]() {
        changeLanguage("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/LineFollower_pl.qm");
        ui->polishButton->setEnabled(false);
        ui->englishButton->setEnabled(true);
    });



    connect(&socket, &QTcpSocket::readyRead,this, &MainWindow::myReadSocket);
    socket.connectToHost(ESP_IP,TCP_PORT);



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


    QPixmap compas_scaled = compass_bkgnd.scaled(this->ui->compas_frame->size(), Qt::IgnoreAspectRatio);
    QPalette compas_palette;
    compas_palette.setBrush(QPalette::Window, compas_scaled);

    this->ui->compas_frame->setPalette(compas_palette);

    connect(this->ui->compas_frame, SIGNAL(resized()), this, SLOT(resizeEvent()));

    connect(ui->plotsButton, &QPushButton::clicked, this, &MainWindow::on_plotsButton_clicked);



    compassPixmap.load("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/images/robot_kompas.png");



    compassLabel = new QLabel(this);


    compassLabel->setFixedSize(160, 160);


    QPixmap scaledPixmap = compassPixmap.scaled(compassLabel->size(), Qt::KeepAspectRatio);
    compassLabel->setPixmap(scaledPixmap);
    ui->compas_frame->layout()->addWidget(compassLabel);




    frameList.append(ui->sensor0);
    frameList.append(ui->sensor1);
    frameList.append(ui->sensor2);
    frameList.append(ui->sensor3);
    frameList.append(ui->sensor4);
    frameList.append(ui->sensor5);
    frameList.append(ui->sensor6);
    frameList.append(ui->sensor7);
    frameList.append(ui->sensor8);
    frameList.append(ui->sensor9);
    frameList.append(ui->sensor10);
    frameList.append(ui->sensor11);
    frameList.append(ui->sensor12);
    frameList.append(ui->sensor13);
    frameList.append(ui->sensor14);
    frameList.append(ui->sensor15);
    frameList.append(ui->sensor16);
    frameList.append(ui->sensor17);
    frameList.append(ui->sensor18);
    frameList.append(ui->sensor19);


    foreach(QFrame* frame, frameList) {
        frame->setStyleSheet("background-color: white;"); // Ustawienie koloru tła dla każdego QFrame
    }


    changeLanguage("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/LineFollower_en_150.qm");



    ui->statistics_label->setText(tr("statistics"));
    ui->statistics_label->setStyleSheet("QLabel { color : black; }");

    ui->pid_label->setText(tr("PID CONTROLER"));
    ui->pid_label->setStyleSheet("QLabel { color : black; }");

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    ui->polishButton->setEnabled(true);
    ui->englishButton->setEnabled(false);

    data_to_send[0] = 5;
    data_to_send[1] = 0;
    data_to_send[2] = 1;
    data_to_send[3] = 50;

    ui->pidPLineEdit->setText(QString::number(data_to_send[0]));
    ui->pidILineEdit->setText(QString::number(data_to_send[1]));
    ui->pidDLineEdit->setText(QString::number(data_to_send[2]));
    ui->vMaxLineEdit->setText(QString::number(data_to_send[3]));

    ui->batteryProgressBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    ui->leftPWMProgresBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    ui->rightPWMProgresBar->setStyleSheet("QProgressBar::chunk { background-color: green; }");

    ui->velocityLCD->display(0);
    ui->vMaxLCD->display(0);
    ui->timeLCD->display(0);
    ui->distanceLCD->display(0);

    max_linear_velocity = 100;
    time = 0;
    linear_velocity = 0;
    distanceFloat = 0.0;
    average_velocity = 0;

    status = 0;
    pwm_L = 0;
    pwm_R = 0;
    w_L = 0;
    w_R = 0;
    z_rotation = 0;
    battery = 4095;




}

MainWindow::~MainWindow()
{

    delete ui;
    delete plotWindow; // Clean up

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

    QPixmap compas_scaled = compass_bkgnd.scaled(this->ui->compas_frame->size(), Qt::IgnoreAspectRatio);

    QPalette compas_palette;
    compas_palette.setBrush(QPalette::Window, compas_scaled);
    this->ui->compas_frame->setPalette(compas_palette);
}

void MainWindow::changeLanguage(const QString &language)
{


    if (translator.load(language)) {
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
        setWindowTitle(tr("MainWindow"));
        //qDebug() << "Translation file loaded successfully.";
    } else {
    qDebug() << "Failed to load translation file.";
    }

    ui->pidPLineEdit->setText(QString::number(data_to_send[0]));
    ui->pidILineEdit->setText(QString::number(data_to_send[1]));
    ui->pidDLineEdit->setText(QString::number(data_to_send[2]));
    ui->vMaxLineEdit->setText(QString::number(data_to_send[3]));
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

    //qDebug() << "data: " << data;
    //ui->label_10->setText(QString(data));

    cutString(QString(data));
    displayData();


}




QString MainWindow::makeDataFrame(char instruction)
{

    QString frame, data_string[4];

    for(int i = 0; i < 4; i++){
        if(data_to_send[i] > 99){
            data_string[i] = QString::number(data_to_send[i]);
        }else if(data_to_send[i] > 9){
            data_string[i] = "0" + QString::number(data_to_send[i]);
        }else if(data_to_send[i] >= 0){
            data_string[i] = "00" + QString::number(data_to_send[i]);
        }else{
            qDebug() << "Wystapil blad przy wartosci data_to_send [ " << i << " ]" << "== " << data_to_send[i];
        }
    }


    frame = "$";
    frame += instruction;

    for(int i = 0; i < 4; i++){
        frame += "x";
        frame += data_string[i];
    }

    frame += "#";

    //qDebug() << "utworzona ramka danych to: " << frame.toUtf8();

    return frame;
}



void MainWindow::on_startButton_clicked()
{
    transmit(makeDataFrame('S') + char(10));
    if(status == 0){
        plotWindow-> clearPlot();
    }
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

}


void MainWindow::on_stopButton_clicked()
{
    transmit(makeDataFrame('M') + char(10));
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
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
            //qDebug() << "ODCZYTANO: " << data_from_line_edit[i];
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

                //qDebug() << coms[i] << "data to send: " << data_to_send[i] << " data from line edit : " << data_from_line_edit[i];
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


    /*for(int i = 0; i < 4; i++ ){
        qDebug() << "parameter to send: " << coms[i] << "= " << data_to_send[i] ;
    }*/


    ui->pidPLineEdit->setText(QString::number(data_to_send[0]));
    ui->pidILineEdit->setText(QString::number(data_to_send[1]));
    ui->pidDLineEdit->setText(QString::number(data_to_send[2]));
    ui->vMaxLineEdit->setText(QString::number(data_to_send[3]));

    transmit(makeDataFrame('U') + char(10));
}


void MainWindow::on_batteryProgressBar_valueChanged(int value)
{
    if(value < 15){
       QMessageBox::critical(this, "BATTERY WARNING!", "Battery is less than 15%. Please charge!");
    }
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
        if(sensors[index]){
            frame->setStyleSheet("background-color: black;"); // Ustawienie koloru tła dla każdego QFrame
        }else{
            frame->setStyleSheet("background-color: white;"); // Ustawienie koloru tła dla każdego QFrame
        }

        index++;
    }

}

void MainWindow::displayEncoders()
{
    int wheel_radius = 10; //mm

    left_linear_V = wheel_radius * w_L / 1000;
    right_linear_V = wheel_radius * w_R / 1000;

    ui->leftAxialVelocityLabel->setText(QString::number(w_L) + " RAD/S");
    ui->rightAxialVelocityLabel->setText(QString::number(w_R) + " RAD/S");

    ui->leftLinearVelocityLabel->setText(QString::number(left_linear_V, 'f', 2) + " M/S");
    ui->rightLinearVelocityLabel->setText(QString::number(right_linear_V, 'f', 2) + " M/S");

    this->linear_velocity = (left_linear_V + right_linear_V) / 2;


    drawArrow(ui->arrow_left, left_linear_V);
    drawArrow(ui->arrow_right, right_linear_V);
}

void MainWindow::drawArrow(QLabel* label, float velocity)
{
    QPixmap pixmap(label->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::red, 5));


    int arrow_length = static_cast<int>((velocity / max_linear_velocity) * label->width());
    int arrow_height = label->height() / 2;
    arrow_length = arrow_length - 30;

    painter.drawLine(0, arrow_height, arrow_length, arrow_height);
    painter.drawLine(arrow_length, arrow_height, arrow_length - 10, arrow_height - 5);
    painter.drawLine(arrow_length, arrow_height, arrow_length - 10, arrow_height + 5);

    label->setPixmap(pixmap);
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


    float battery_voltage = (battery * MAX_ADC_VOLTAGE * VOLTAGE_DIVIDER)/MAX_ADC_VALUE;

    if(status == 1){
        plotWindow->updatePlot(time/1000, pwm_L, pwm_R, sensors, linear_velocity, battery_voltage);
    }


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

}

void MainWindow::displayCompass()
{

    QPixmap scaledPixmap = compassPixmap.scaled(compassLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPixmap pixmap(compassLabel->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.translate(pixmap.width() / 2, pixmap.height() / 2);
    painter.rotate(z_rotation);
    painter.translate(-scaledPixmap.width() / 2, -scaledPixmap.height() / 2);
    painter.drawPixmap(0, 0, scaledPixmap);

    compassLabel->setPixmap(pixmap);
    compassLabel->update();
}

void MainWindow::displayData(){

    if(status){
        ui->statusLabel_2->setText(tr("ROBOT IS MOVING"));
        ui->statusLabel_2->setStyleSheet("QLabel { color : green; }");
    }else{
        ui->statusLabel_2->setText(tr("ROBOT IS NOT MOVING"));
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
    displayCompass();
    //

}






void MainWindow::on_plotsButton_clicked()
{

    plotWindow->show();

}






