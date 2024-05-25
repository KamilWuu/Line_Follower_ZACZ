#include "DataFrame.h"


DataFrame::DataFrame() {
    randomSeed(analogRead(0)); // Inicjalizacja generatora liczb losowych
}

void DataFrame::generateRandomData() {
    // Generowanie losowego statusu
    //status = random(2); // Random z zakresu 0-1

    // Generowanie losowych danych dla tablicy sensors
    for (int i = 0; i < 20; ++i) {
        sensors[i] = random(2); // Random z zakresu 0-1
        
    }

    // Generowanie losowych wartości dla pwm_L, pwm_R, w_L, w_R, z_rotation i battery
    //pwm_L = random(101); // Random z zakresu 0-100
    //pwm_R = random(101); 
    //w_L = random(9999); 
    //w_R = random(9999); 
    z_rotation = random(361); 
    battery = random(3160,3474); 
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