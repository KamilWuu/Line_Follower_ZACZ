#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>






class Sensors{
private:
    /*trzeba ustawic piny tak aby odpowiadaly one rzeczywistym czujnikom od lewej strony, a ten jeden przedni jako ostatni*/
    const uint8_t IR_SENSORS_PINS[20] = {3,7,8,9,10,14,15,16,17,18,19,20,21,35,36,37,38,39,47,48};  

    
    const int8_t SENSORS_WEIGHTS[20] = {-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,0}; /*PID WEIGHTS*/
    uint8_t measures[20];

public:
    Sensors();
    void readSensors();
    const uint8_t * getSensorsPins();
    uint8_t * getSensorsMeasures();
    void printSensorsMeasures();
};


#endif 