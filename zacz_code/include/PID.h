



#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "Sensors.h"
void PCalculatePWM(const uint16_t sensors_error_, const uint16_t Kp, const uint8_t max_pwm_percent_value, uint8_t * left_pwm_percent_value, uint8_t * right_pwm_percent_value, uint16_t * left_pwm_value, uint16_t * right_pwm_value);

#endif 