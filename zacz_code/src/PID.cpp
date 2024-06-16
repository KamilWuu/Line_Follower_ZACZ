#include "PID.h"

/*
Elementy regulatora silników:
Zadajnik prędkości  -- stała (Może być zmienna, ale to regulator nieliniowy) prędkość, która jest modyfikowana w celu ogólnego przyśpeszenia/zwolnienia robota (może być też zmienna)
Przekształcenie odczytów czujników podłoża na kąt odchylenia od linii
Regulator PD -- sterowany kątem odchylenia i sterujący silnikami, modyfikuje wartość zadajnika prędkości 

!!! Prawy i lewy silnik sterowane są tym samym sygnałem, tylko w przeciwnej fazie !!! 

*/

    int16_t Regulator::PID(const int16_t ang_error){
        uint16_t Proportional,Integral, Differencial;
        if(this->Kp!=0) Proportional = this->Kp*ang_error;
        else Proportional = 0;

        if(this->Ki!=0) Integral = this->Ki*integrate(ang_error);
        else Integral = 0;

        if(this->Kd!=0) Differencial = this->Kd*((ang_error-last_ang_err)/SAMPLING_TIME);
        else Differencial = 0;

        int16_t pid_val = Proportional + Integral + Differencial;

        last_ang_err=ang_error;
        Serial.print(ang_error);
        Serial.print("\t");
        Serial.println(pid_val);

        return pid_val;
    }

    int16_t Regulator::integrate(const int16_t curr_value){
        this->sum_I +=curr_value*SAMPLING_TIME; //Trzeba jakoś znormalizować, bo to duże wartości
        return this->sum_I;
    }

    Regulator::Regulator(){}

    void Regulator::begin(){
        pinMode(LEFT_PWM, OUTPUT);
        pinMode(RIGHT_PWM, OUTPUT);
    }

    void Regulator::set_pid(uint8_t k_p, uint8_t k_i, uint8_t k_d){
        this->Kp=k_p;
        this->Ki=k_i;
        this->Kd=k_d;
    }

    void Regulator::set_base_speed(uint32_t speed){

        this->baseSpeed=speed*255/100;
    }

    void Regulator::regulator(const int16_t ang_error){

    int16_t pid_val = PID(ang_error);
    int16_t leftPWM=this->baseSpeed+pid_val, rightPWM=this->baseSpeed-pid_val;
    if (leftPWM<0)  leftPWM=0;
    if (rightPWM<0)  rightPWM=0;
    if (leftPWM> MAX_PWM_VALUE_CPU) leftPWM = MAX_PWM_VALUE_CPU;
    if (rightPWM> MAX_PWM_VALUE_CPU)    rightPWM = MAX_PWM_VALUE_CPU;

    this->leftPWM_percent = (leftPWM * 100)/MAX_PWM_VALUE_CPU;
    this->rightPWM_percent = (rightPWM * 100)/MAX_PWM_VALUE_CPU;
    this->leftPWM_value = leftPWM;
    this->rightPWM_value=rightPWM;

    analogWrite(LEFT_PWM,leftPWM);
    analogWrite(RIGHT_PWM, rightPWM); 

    }

    uint8_t Regulator::get_right_percent(){
        return this->rightPWM_percent;
    }
    uint8_t Regulator::get_left_percent(){
        return this->leftPWM_percent;
    }
    
    uint8_t Regulator::get_right_value(){
        return this->rightPWM_value;
    }

    uint8_t Regulator::get_left_value(){
        return this->leftPWM_value;
    }

/*void PCalculatePWM(const uint16_t sensors_error_, const uint16_t Kp, const uint8_t max_pwm_percent_value, uint8_t * left_pwm_percent_value, uint8_t * right_pwm_percent_value, uint16_t * left_pwm_value, uint16_t * right_pwm_value){
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

}*/

