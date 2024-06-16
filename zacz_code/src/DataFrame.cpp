#include "DataFrame.h"


DataFrame::DataFrame() {
    //randomSeed(analogRead(0)); // Inicjalizacja generatora liczb losowych
}



void DataFrame::setData(uint8_t status_, uint8_t * sensors_, uint8_t pwm_L_, uint8_t pwm_R_, uint16_t w_L_, uint16_t w_R_ ,uint16_t z_rotation_ ,uint16_t battery_)
{
    this->status = status_;

    for(int i = 0; i < 20; i++){
        this->sensors[i] = sensors_[i];
    }
    


    this->pwm_L = pwm_L_;
    this->pwm_R = pwm_R_;
    this->w_L = w_L_;
    this->w_R = w_R_;

    int16_t z_rot =  z_rotation_;
    // Normalizacja kąta do zakresu 0-359 stopni
    if (z_rot >= 360.0) {
        z_rot -= 360.0;
    } else if (z_rot < 0.0) {
        z_rot += 360.0;
    }

    this->z_rotation = -z_rot;



    this->battery = battery_;
}

void DataFrame::setAxialVelocity(uint16_t  left, uint16_t  right){
    w_L = left;
    w_R = right;
}

void DataFrame::setPWM(uint16_t  left, uint16_t  right){
    pwm_L = left;
    pwm_R = right;
}

void DataFrame::setStatus(uint8_t i){
    status = i;
}

void DataFrame::setBattery(uint16_t adc_measure){
    battery = adc_measure;
}

void DataFrame::setSensors(uint8_t * readedSensors){
    for(int i = 20; i < 20; i++){
        this->sensors[i] = readedSensors[i];
    }
}

String DataFrame::createDataFrame() {
    String frameString = "$";

    // Dodawanie statusu
    frameString += String(status, DEC);
    frameString += "x";

    // Dodawanie danych z tablicy sensors
    for (int i = 0; i < 20; ++i) {
        frameString += String(sensors[i], DEC);
        
    }
    frameString += "x";

    // Dodawanie pwm_L
    if (pwm_L < 10)
        frameString += "00";
    else if (pwm_L < 100)
        frameString += "0";
    frameString += String(pwm_L, DEC) + "x";

    // Dodawanie pwm_R
    if (pwm_R < 10)
        frameString += "00";
    else if (pwm_R < 100)
        frameString += "0";
    frameString += String(pwm_R, DEC) + "x";

    // Dodawanie w_L
    if (w_L < 10)
        frameString += "000";
    else if (w_L < 100)
        frameString += "00";
    else if (w_L < 1000)
        frameString += "0";
    frameString += String(w_L, DEC) + "x";

    // Dodawanie w_R
    if (w_R < 10)
        frameString += "000";
    else if (w_R < 100)
        frameString += "00";
    else if (w_R < 1000)
        frameString += "0";
    frameString += String(w_R, DEC) + "x";

    // Dodawanie z_rotation
    if (z_rotation < 10)
        frameString += "00";
    else if (z_rotation < 100)
        frameString += "0";
    frameString += String(z_rotation, DEC) + "x";

    // Dodawanie battery
    if (battery < 10)
        frameString += "000";
    else if (battery < 100)
        frameString += "00";
    else if (battery < 1000)
        frameString += "0";
    frameString += String(battery, DEC);

    frameString += "#";

    return frameString;
}