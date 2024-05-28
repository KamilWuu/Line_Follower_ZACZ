#include "PID.h"

#define HALF_PWM_VALUE_CPU 127
#define MAX_PWM_VALUE_CPU 255

void PCalculatePWM(const uint16_t sensors_error_, const uint16_t Kp, const uint8_t max_pwm_percent_value, uint8_t * left_pwm_percent_value, uint8_t * right_pwm_percent_value, uint16_t * left_pwm_value, uint16_t * right_pwm_value){
    uint16_t temp_left_pwm_value;
    uint16_t temp_right_pwm_value;
    uint16_t divider;

    int32_t K_parameter;

    K_parameter = sensors_error_ * Kp;
    if(K_parameter > MAX_PWM_VALUE_CPU){
        K_parameter = MAX_PWM_VALUE_CPU;
    }else if(K_parameter < -MAX_PWM_VALUE_CPU){
        K_parameter = -MAX_PWM_VALUE_CPU;
    }

    if(K_parameter > 0){

        temp_left_pwm_value = MAX_PWM_VALUE_CPU - K_parameter;
        temp_right_pwm_value = MAX_PWM_VALUE_CPU;

    }else if(K_parameter  < 0){
        temp_left_pwm_value  = MAX_PWM_VALUE_CPU;
        temp_right_pwm_value = MAX_PWM_VALUE_CPU + K_parameter;
    }else{
        temp_right_pwm_value = MAX_PWM_VALUE_CPU;
        temp_left_pwm_value = MAX_PWM_VALUE_CPU;
    }


    *left_pwm_value = (temp_left_pwm_value * max_pwm_percent_value) / 100;
    *right_pwm_value = (temp_right_pwm_value * max_pwm_percent_value) / 100;


    *left_pwm_percent_value = (*left_pwm_value * 100)/MAX_PWM_VALUE_CPU;
    *right_pwm_percent_value = (*right_pwm_value * 100)/MAX_PWM_VALUE_CPU;

}