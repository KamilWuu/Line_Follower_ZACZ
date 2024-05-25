#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>






class Sensors{
private:
    const uint8_t IR_SENSORS_PINS[20] = {3,7,8,9,10,14,15,16,17,18,19,20,21,35,36,37,38,39,47,48};
    uint8_t measures[20];

public:
    Sensors();
    void readSensors();
    const uint8_t * getSensorsPins();
    uint8_t * getSensorsMeasures();
    void printSensorsMeasures();
};


#endif 