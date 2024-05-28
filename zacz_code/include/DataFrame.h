#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <Arduino.h>

class DataFrame {
private:
    uint8_t status; //1
    uint8_t sensors[20]; 
    uint8_t pwm_L; //3
    uint8_t pwm_R;//3
    uint16_t w_L;//4
    uint16_t w_R;//4
    uint16_t z_rotation;//3
    uint16_t battery;//4

public:
    DataFrame();
    //void generateRandomData();
    String createDataFrame();

    

    void setAxialVelocity(uint16_t  left, uint16_t  right);
    void setPWM(uint16_t  left, uint16_t  right);
    void setStatus(uint8_t i);
    void setBattery(uint16_t adc_measure);
    void setSensors(uint8_t * readedSensors);

    void setData(uint8_t status_, uint8_t * sensors_, uint8_t pwm_L_, uint8_t pwm_R_, uint16_t w_L_, uint16_t w_R_ ,uint16_t z_rotation_ ,uint16_t battery_);

};
#endif
