#ifndef RECEIVEDDATA_H
#define RECEIVEDDATA_H

#include <Arduino.h>
#include <BluetoothSerial.h>

class ReceivedData {
private:
    String debugString;
    char instruction;
    uint16_t PID_P;
    uint16_t PID_I;
    uint16_t PID_D;
    uint8_t V_MAX;

 

public:
    ReceivedData();
    void setData(String data);
    void displayData(HardwareSerial * serial);
    void displayDataBT(BluetoothSerial * serial);
    char getInstruction();
    uint8_t getVMax();
    String getString();
};


//$Mx123x456x789x1000#
#endif
