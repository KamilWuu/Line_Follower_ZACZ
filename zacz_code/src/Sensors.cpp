#include "Sensors.h"

Sensors::Sensors(){
    for(int i = 0; i < 20; i++){
        this->measures[i] = 0;
    }
}


int16_t Sensors::readSensors(){
    uint8_t founds_counter = 0; 
    int16_t weights_sum = 0;
    measures[4] = 0;
    
    for(int i = 0; i < 20; i++){
        
        if(i != 4){
        this->measures[i] = !digitalRead(  IR_SENSORS_PINS[i] );
        }
        
        if(this->measures[i] == 1){
            weights_sum += this->SENSORS_WEIGHTS[i];
            founds_counter++;
        }
    }




    if(founds_counter > 0){ //jezeli linia zostala znaleziona
        for(int i = 0; i < 20; i++){
            last_measures[i] = measures[i];
        } 
    }

    if(founds_counter != 0){
        this->sensors_error = weights_sum / founds_counter;
    }else{
        //Zmienić na graniczne położenie z ostatniego odczytu
        this->sensors_error = 0; 
    }

    return this->sensors_error;
}




const uint8_t * Sensors::getSensorsPins(){
    return this->IR_SENSORS_PINS;
}

uint8_t * Sensors::getSensorsMeasures(){
    return this->measures;
}

uint16_t Sensors::getSensorsError(){
    return this->sensors_error;
}

void Sensors::printSensorsMeasures(){
    /*Serial.print("PIN:");
    for(int i = 0; i < 20; i++){
        Serial.print("\t");
        Serial.print(this->IR_SENSORS_PINS[i]);
    }
    Serial.print("\n");*/

    Serial.print("MEASURE:");
    for(int i = 0; i < 20; i++){
        Serial.print("\t");
        Serial.print(this->measures[i]);
    }
    Serial.print("\n");
}