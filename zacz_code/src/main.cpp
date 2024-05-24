#include <Arduino.h>
#include <WiFi.h>

#include "DataFrame.h"
#include "ReceivedData.h"


#define NETWORK     "internet2"
#define PASSWORD    "tak123pl"
#define HOST        "10.42.0.1"
#define PORT        8888

#define LED 40

WiFiServer server(8888);
WiFiClient client;

void setup() {
    pinMode(LED, OUTPUT);
   
  Serial.begin(115200);
 
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

void loop() {
    digitalWrite(LED,HIGH);
    delay(100);
    digitalWrite(LED,LOW);
    delay(100);

    client.println("Kartuzy");

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
    }
}
