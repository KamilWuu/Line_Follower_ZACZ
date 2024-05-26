#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <BindArg.h>

/*enum rotation_dir{
    backward=-1,
    no_rotation=0,
    forward=1
};*/

class Encoder
{
private:
    uint8_t outPin_A;
    uint8_t outPin_B;
    
    int rotations;

    bindArgVoidFunc_t interruptGate = nullptr;
    int               interruptNum;

    unsigned long impulseTimes[2];


public:
    Encoder();
    ~Encoder();
    void begin(uint8_t pin_a, uint8_t pin_b);
    void update();
    int get_rotations();
};



#endif