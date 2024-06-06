#ifndef PID_H
#define PID_H


#include <Arduino.h>
#include "Sensors.h"
#include "defines.h"

#define MAX_PWM_VALUE_CPU 255
#define SAMPLING_TIME 2 //ms 

class Regulator{

    private:
    uint8_t baseSpeed;

    int16_t last_ang_err;

    uint8_t leftPWM_value;
    uint8_t rightPWM_value;

    uint8_t leftPWM_percent;
    uint8_t rightPWM_percent;

    uint8_t Kp, Ki, Kd;

    uint32_t sum_I;

    int16_t PID(const int16_t ang_error);
    int16_t integrate(const int16_t curr_value);

    public:

    Regulator();
    void begin();
    void regulator(const int16_t ang_error);

    void set_pid(uint8_t k_p, uint8_t k_i, uint8_t k_d);
    void set_base_speed(uint8_t speed);

    uint8_t get_right_percent();
    uint8_t get_left_percent();
    
    uint8_t get_right_value();
    uint8_t get_left_value();

};

//void PCalculatePWM(const uint16_t sensors_error_, const uint16_t Kp, const uint8_t max_pwm_percent_value, uint8_t * left_pwm_percent_value, uint8_t * right_pwm_percent_value, uint16_t * left_pwm_value, uint16_t * right_pwm_value);

#endif 