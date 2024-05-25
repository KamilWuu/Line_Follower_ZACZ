#include <Arduino.h>
#include <WiFi.h>

#include "DataFrame.h"
#include "ReceivedData.h"


#define NETWORK     "internet2"
#define PASSWORD    "tak123pl"
#define HOST        "10.42.0.1"
#define PORT        8888

#define LEFT_PWM 40
#define RIGHT_PWM 6
#define BUTTON 2
#define BATTERY 1

WiFiServer server(8888);
WiFiClient client;


DataFrame data_frame;
ReceivedData received_data;




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




void setup() {
    pinMode(LEFT_PWM, OUTPUT);
    pinMode(RIGHT_PWM, OUTPUT);
    pinMode(BUTTON, INPUT);
    pinMode(BATTERY, INPUT);

    Serial.begin(115200);

    setupWifi();
  






}

void loop() {
    

    


    clientRead();

    switch (received_data.getInstruction())
    {
    case 'S':
      analogWrite(LEFT_PWM, received_data.getVMax() ); 
      analogWrite(RIGHT_PWM, received_data.getVMax() ); 

      data_frame.setAxialVelocity(random(1001) ,random(1001));
      //frame.setAxialVelocity(1000 ,1000);
      data_frame.setPWM(random(received_data.getVMax()+1), random(received_data.getVMax() + 1));
      data_frame.setStatus(1);
      break;

    case 'M':
      analogWrite(LEFT_PWM,0);
      analogWrite(RIGHT_PWM,0); 
      data_frame.setAxialVelocity(0,0);
      data_frame.setPWM(random(0), random(0));
      data_frame.setStatus(0);
      break;


    default:
      break;
    }

    data_frame.generateRandomData(); //<= tymczasowo

    data_frame.setBattery(analogRead(BATTERY));

    client.println(data_frame.createDataFrame());    
    



}
