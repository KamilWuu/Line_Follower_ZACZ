#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <BindArg.h>

/*enum rotation_dir{
    backward=-1,
    no_rotation=0,
    forward=1
};*/


/*Obecna implementacja zakłada 100 ms opóźnienia w odczycie danych. 
Dla lepszych wyników można zmienić implementację, na przykład mierzyć czas pomiędzy impulsami
(kod jest zaimplementowany, wystarczy tylko odkomentować)
*/
class Encoder
{
private:
    uint8_t outPin_A;
    uint8_t outPin_B;
    
    // rotations[1] -> Obecnie zliczane impulsy
    // rotations[0] -> Impulsy z poprzedniego cyklu
    int rotations[2];

    bindArgVoidFunc_t interruptGate = nullptr;
    int               interruptNum;

    //Kierunek obrotu: 0 - przód, 1 - tył
    //bool dir;

    float speed;


public:
    Encoder();
    ~Encoder();

    //Inicjalizacja Enkodera
    void begin(uint8_t pin_a, uint8_t pin_b);
    
    //Funkcja wewnątrz przerwania, zmieniać adekwatnie do stosowanej metody pomiaru prędkości
    void update();

    // Funkcja przepisująca ilość obrotów do poprzedniej komórki w tablicy
    void calc_speed();

    int get_rotations();
    
    float get_speed();
};



#endif