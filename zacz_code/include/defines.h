#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>

#define NETWORK     "internet2"
#define PASSWORD    "tak123pl"
#define HOST        "10.42.0.1"
#define PORT        8888

/*INTERFACE*/
#define BATTERY 1
#define BUTTON 2
#define SDA_I2C 11
#define SCL_I2C 12
#define IMU_INT 13



/*LEFT MOTOR*/
#define LEFT_PWM 40
#define LEFT_ENC_2 41
#define LEFT_ENC_1 42

/*RIGHT MOTOR*/

#define RIGHT_PWM 6
#define RIGHT_ENC_2 4
#define RIGHT_ENC_1 5

#endif