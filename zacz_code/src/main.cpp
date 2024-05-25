#include <Arduino.h>
#include <WiFi.h>

#include "DataFrame.h"
#include "ReceivedData.h"
#include "defines.h"
#include "Wire.h"
#include "Sensors.h"


WiFiServer server(8888);
WiFiClient client;


DataFrame data_to_send;
ReceivedData received_data;
Sensors IR_Sensors;



void setupWifi(){
  WiFi.useStaticBuffers(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(NETWORK, PASSWORD);


  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
  }

  Serial.println(WiFi.localIP());

  server.begin();

    Serial.println("waiting for client...");
    while(!server.hasClient()) {}
    client = server.available();
    Serial.println("client found");

}

void clientRead(){

    String msg = "";
    while(client.available()) {
        char c = client.read();

        if(c=='\n') {
            break;
        } else {
            msg +=c;
        }
    }

    if(msg!="") {
        Serial.println(msg);
        received_data.setData(msg);

    }

}


void GPIOSetup(){
  
  pinMode(BATTERY, INPUT);
  pinMode(BUTTON, INPUT);


  /*MOTORS*/
  pinMode(LEFT_ENC_1, INPUT);
  pinMode(LEFT_ENC_2, INPUT);

  pinMode(RIGHT_ENC_1, INPUT);
  pinMode(RIGHT_ENC_2, INPUT);

  pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);

  /*IR SENSORS*/
  for(int i = 0; i < 20; i++){
    pinMode(IR_Sensors.getSensorsPins()[i],INPUT);
  }

  //Wire.setPins(SDA_I2C, SCL_I2C);



  

}

void setup() {
    Serial.begin(115200);
    GPIOSetup();
    setupWifi();
    //Wire.begin();






}

void loop() {
    

    


    clientRead(); // <== Odbiera dane od clienta (aplikacji Qt) i zapisuje odczytane wartosci w strukturze received_data




    switch (received_data.getInstruction()) 
    {
    case 'S': //S czyli start jazdy
      analogWrite(LEFT_PWM, received_data.getVMax() ); 
      analogWrite(RIGHT_PWM, received_data.getVMax() ); 

      data_to_send.setAxialVelocity(random(1001) ,random(1001));
      //frame.setAxialVelocity(1000 ,1000);
      data_to_send.setPWM(random(received_data.getVMax()+1), random(received_data.getVMax() + 1));
      
      data_to_send.setStatus(1);//<== status 1 czyli jedzie

      break;

    case 'M': //M czyli stop jazdy
      analogWrite(LEFT_PWM,0);
      analogWrite(RIGHT_PWM,0); 
      data_to_send.setAxialVelocity(0,0);
      data_to_send.setPWM(random(0), random(0));
      data_to_send.setStatus(0); //<== status 0 czyli stoi
      break;


    default:
      break;
    }

    data_to_send.generateRandomData(); //<= tymczasowo

    data_to_send.setBattery(analogRead(BATTERY));
    IR_Sensors.readSensors();
    IR_Sensors.printSensorsMeasures();


    data_to_send.setSensors(IR_Sensors.getSensorsMeasures());


    client.println(data_to_send.createDataFrame());    // <== wysyla utworzona ramke danych ze struktury data_to_send do clienta (aplikacji Qt)
    



}
