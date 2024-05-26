#include "Encoder.h"


Encoder::Encoder()
{
}

Encoder::~Encoder()
{
}

void Encoder::begin(uint8_t pin_a, uint8_t pin_b){
    this->outPin_A=pin_a;
    this->outPin_B=pin_b;
    pinMode(pin_a,INPUT_PULLUP);
    pinMode(pin_b,INPUT_PULLUP);
    this->rotations=0;
    interruptNum = digitalPinToInterrupt(pin_b);
    interruptGate = bindArgGateThisAllocate(&Encoder::update, this);
    attachInterrupt(interruptNum,interruptGate, CHANGE);
}

void Encoder::update(){
    // Obsługa zliczania impulsów
    if (digitalRead(this->outPin_A) == LOW)
    {
        if (digitalRead(this->outPin_B)==LOW)
        {
            ++rotations;
        }
        else {            
            --rotations;
            }
    }
}

int Encoder::get_rotations(){
    return this->rotations;
}