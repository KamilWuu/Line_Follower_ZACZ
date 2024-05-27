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
    this->rotations[0]=0;
    this->rotations[1]=0;
    interruptNum = digitalPinToInterrupt(pin_b);
    interruptGate = bindArgGateThisAllocate(&Encoder::update, this);
    //this->impulseTime[0]=esp_timer_get_time();
    //this->impulseTime[1]=esp_timer_get_time();
    //this->dir=0;
    attachInterrupt(interruptNum,interruptGate, CHANGE);
}

void Encoder::update(){
    // Obsługa zliczania impulsów
    if (digitalRead(this->outPin_A) == LOW)
    {
        if (digitalRead(this->outPin_B)==LOW)
        {
            ++rotations[1];
        }
        else {            
            --rotations[1];
            }
    }

    // Liczenie czasu pomiędzy impulsami
    /*if (digitalRead(this->outPin_A) == LOW)
    {
        this->impulseTime[0]=this->impulseTime[1];
        this->impulseTime[1]=esp_timer_get_time();
        if (digitalRead(this->outPin_B)==LOW)
        {
            this->dir=0;
        }
        else {            
            this->dir=1;
        }
    }*/
}

int Encoder::get_rotations(){
    return this->rotations[0];
}

//Zliczanie impulsów w czasie, z okresowym przerwaniem
void Encoder::calc_speed(){
    rotations[0] = rotations[1];
    rotations[1] = 0;
}

//Zliczanie czasu pomiędzy impilsami, bez dodatkowego przerwania
/*void Encoder::calc_speed(){
    speed = 1.00/(impulseTime[1]-impulseTime[0]);
    if (dir!=0)
    {
        speed = -speed;
    } 
}*/

float Encoder::get_speed(){
    speed = ((float)rotations[0])/100000; //Ususnąć jeśli korzystamy z czasu pomiędzy impulsami
    return speed;
}