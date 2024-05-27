#include <Arduino.h>
#include <WiFi.h>

#include "DataFrame.h"
#include "ReceivedData.h"
#include "defines.h"
#include "Wire.h"
#include "Sensors.h"
#include "Encoder.h"
#include "ESP32TimerInterrupt.h"


#define TIMER0_INTERVAL_MS        100

WiFiServer server(8888);
WiFiClient client;

DataFrame data_to_send;
ReceivedData received_data;
Sensors IR_Sensors;
Encoder Left_enc;
Encoder Right_enc;

ESP32Timer Timer0(0);

void setupWifi()
{
  WiFi.useStaticBuffers(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(NETWORK, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(WiFi.status());
  }

  Serial.println(WiFi.localIP());

  server.begin();

  Serial.println("waiting for client...");
  while (!server.hasClient())
  {
  }
  client = server.available();
  Serial.println("client found");
}

void clientRead()
{

  String msg = "";
  while (client.available())
  {
    char c = client.read();

    if (c == '\n')
    {
      break;
    }
    else
    {
      msg += c;
    }
  }

  if (msg != "")
  {
    Serial.println(msg);
    received_data.setData(msg);
  }
}

void GPIOSetup()
{

  pinMode(BATTERY, INPUT);
  pinMode(BUTTON, INPUT);

  /*MOTORS*/

  Left_enc.begin(LEFT_ENC_2, LEFT_ENC_1);
  Right_enc.begin(RIGHT_ENC_1,RIGHT_ENC_2);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);

  /*IR SENSORS*/
  for (int i = 0; i < 20; i++)
  {
    pinMode(IR_Sensors.getSensorsPins()[i], INPUT);
  }

  // Wire.setPins(SDA_I2C, SCL_I2C);
}

bool IRAM_ATTR TimerHandler0(void * timerNo){
  Left_enc.calc_speed();
  Right_enc.calc_speed();
  return true;
}

void setup()
{
  Serial.begin(115200);
  GPIOSetup();
  if (Timer0.attachInterruptInterval(TIMER0_INTERVAL_MS*1000,TimerHandler0))
  {
		Serial.print(F("Starting  ITimer0 OK, millis() = "));
		Serial.println(millis());  
    }
  
  //setupWifi();
  // Wire.begin();
}

void loop()
{
  Serial.print("Enkodery: L:\t");
  Serial.print(Left_enc.get_speed(),10);
  Serial.print("\trotations: ");
  Serial.print(Left_enc.get_rotations());
  Serial.print(" R:\t");
  Serial.print(Right_enc.get_speed(),10);
  Serial.print("\trotations: ");
  Serial.print(Right_enc.get_rotations());
  Serial.print("\n");

  //IR_Sensors.readSensors();
  //IR_Sensors.printSensorsMeasures();

  delay(100);
  /*clientRead(); // <== Odbiera dane od clienta (aplikacji Qt) i zapisuje odczytane wartosci w strukturze received_data

  switch (received_data.getInstruction())
  {
  case 'S': // S czyli start jazdy
    analogWrite(LEFT_PWM, received_data.getVMax());
    analogWrite(RIGHT_PWM, received_data.getVMax());

    data_to_send.setAxialVelocity(random(1001), random(1001));
    // frame.setAxialVelocity(1000 ,1000);
    data_to_send.setPWM(random(received_data.getVMax() + 1), random(received_data.getVMax() + 1));

    data_to_send.setStatus(1); //<== status 1 czyli jedzie

    break;

  case 'M': // M czyli stop jazdy
    analogWrite(LEFT_PWM, 0);
    analogWrite(RIGHT_PWM, 0);
    data_to_send.setAxialVelocity(0, 0);
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

  client.println(data_to_send.createDataFrame()); // <== wysyla utworzona ramke danych ze struktury data_to_send do clienta (aplikacji Qt)
  */
}
