#include "ReceivedData.h"


ReceivedData::ReceivedData() {
    debugString = "DEBUGDEBUG";
    instruction = 'M';
    PID_P = 0;
    PID_I = 0;
    PID_D = 0;
    V_MAX = 0;
}

void ReceivedData::setData(String data) {
    debugString = data;
    data.trim();
    // Sprawdzanie poprawności formatu danych
    if (data.charAt(0) != '$' || data.charAt(data.length() - 1) != '#') {
        // Błąd: Niepoprawny format danych
        Serial.println("Błąd: Niepoprawny format danych");
        debugString = "Błąd: Niepoprawny format danych";
        return;
    }

    // Usunięcie znaków '$' i '#' z danych
    data = data.substring(1, data.length() - 1);

    // Podział danych na części na podstawie separatora 'x'
    String parts[5]; // Tablica na poszczególne części danych
    int lastIndex = 0; // Indeks ostatniego znalezionego separatora 'x'

    for (int i = 0; i < 4; i++) {
        int separatorIndex = data.indexOf('x', lastIndex + 1);
        parts[i] = data.substring(lastIndex, separatorIndex);
        lastIndex = separatorIndex + 1;
    }

    // Ostatnia część (V_MAX) będzie do końca ciągu danych
    parts[4] = data.substring(lastIndex);

    // Przypisanie odczytanych wartości do zmiennych
    char instructionTemp = parts[0].charAt(0);

    if(instructionTemp == 'U' && instruction != 'S'){
        instruction = 'U';
        PID_P = parts[1].toInt();
        PID_I = parts[2].toInt();
        PID_D = parts[3].toInt();
        V_MAX = parts[4].toInt();

        for(int i = 0; i < 3; i++){
            analogWrite(40,256);
            analogWrite(6,256);
            delay(100);
            analogWrite(40,0);
            analogWrite(6,0);
            delay(100);
        }
        instruction = 'M';

    }else if(instructionTemp == 'M'){
        instruction = 'M';
    }else if(instructionTemp == 'S'){
        instruction = 'S';
    }


}

char ReceivedData::getInstruction(){
    return instruction;
}
String ReceivedData::getString(){
    return debugString;
}

uint8_t ReceivedData::getVMax(){
    return V_MAX;
}


void ReceivedData::displayData(HardwareSerial * serial){
    serial->print("otrzymano dane: ");
    serial->println(debugString);

    // Debug - wyświetlenie odczytanych wartości
    serial->print("instruction: ");
    serial->println(instruction);
    serial->print("PID_P: ");
    serial->println(PID_P);
    serial->print("PID_I: ");
    serial->println(PID_I);
    serial->print("PID_D: ");
    serial->println(PID_D);
    serial->print("V_MAX: ");
    serial->println(V_MAX);
}

void ReceivedData::displayDataBT(BluetoothSerial  * serial){

    serial->print("otrzymano dane: ");
    serial->println(debugString);

    // Debug - wyświetlenie odczytanych wartości
    serial->print("instruction: ");
    serial->println(instruction);
    serial->print("PID_P: ");
    serial->println(PID_P);
    serial->print("PID_I: ");
    serial->println(PID_I);
    serial->print("PID_D: ");
    serial->println(PID_D);
    serial->print("V_MAX: ");
    serial->println(V_MAX);
}