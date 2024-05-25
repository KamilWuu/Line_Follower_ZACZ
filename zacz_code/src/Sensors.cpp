#include "Sensors.h"

Sensors::Sensors(){
    for(int i = 0; i < 20; i++){
        this->measures[i] = 0;
    }
}

void Sensors::readSensors(){
    for(int i = 0; i < 20; i++){
        this->measures[i] = digitalRead(  IR_SENSORS_PINS[i] );
    }
}

const uint8_t * Sensors::getSensorsPins(){
    return this->IR_SENSORS_PINS;
}

uint8_t * Sensors::getSensorsMeasures(){
    return this->measures;
}

void Sensors::printSensorsMeasures(){
    Serial.print("PIN:");
    for(int i = 0; i < 20; i++){
        Serial.print("/t");
        Serial.print(this->IR_SENSORS_PINS[i]);
    }
    Serial.print("\n");

    Serial.print("MEASURE:");
    for(int i = 0; i < 20; i++){
        Serial.print("/t");
        Serial.print(this->measures[i]);
    }
    Serial.print("\n");
}