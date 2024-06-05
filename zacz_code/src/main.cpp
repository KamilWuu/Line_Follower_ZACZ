#include <Arduino.h>
#include <WiFi.h>

#include "DataFrame.h"
#include "ReceivedData.h"
#include "defines.h"
#include "Wire.h"
#include "Sensors.h"
#include "Encoder.h"
#include "ESP32TimerInterrupt.h"
#include "PID.h"

//Zmienna określająca częstotliwość pomiaru czasu
#define TIMER0_INTERVAL_MS        50

WiFiServer server(8888);
WiFiClient client;

DataFrame data_to_send;
ReceivedData received_data;
Sensors IR_Sensors;
Encoder Left_enc;
Encoder Right_enc;
Regulator Controller;

ESP32Timer Timer0(0);


int8_t robot_status = 0;

uint8_t Left_pwm_percent_value = 0;
uint8_t Right_pwm_percent_value = 0;

uint16_t Left_pwm_value = 0;
uint16_t Right_pwm_value = 0;

int16_t robot_z_position = 0;



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






void makeMeasuresAndCalculations(){

  uint16_t battery = analogRead(BATTERY);
  IR_Sensors.readSensors();
  
  //tu trzeba potem dodac jeszcze pomiar z imu 
  if(robot_status == 1){
  Controller.set_pid(received_data.getPID_parameter(K_P),received_data.getPID_parameter(K_I), received_data.getPID_parameter(K_D));
  Controller.set_base_speed(received_data.getVMax());
  Controller.regulator(IR_Sensors.getSensorsError());
  Left_pwm_percent_value=Controller.get_left_percent();
  Right_pwm_percent_value= Controller.get_right_percent();
  //PCalculatePWM(IR_Sensors.getSensorsError(), received_data.getPID_parameter(K_P), received_data.getVMax(), &Left_pwm_percent_value, &Right_pwm_percent_value, &Left_pwm_value, &Right_pwm_value);
  }else if(robot_status == 0){
    Left_pwm_percent_value = 0;
    Right_pwm_percent_value = 0;

    Left_pwm_value = 0;
    Right_pwm_value = 0;
  }

  data_to_send.setData(robot_status, IR_Sensors.getSensorsMeasures(), Left_pwm_percent_value, Right_pwm_percent_value, Left_enc.get_speed(), Right_enc.get_speed(), robot_z_position, battery );

}

void GPIOSetup()
{

  pinMode(BATTERY, INPUT);
  pinMode(BUTTON, INPUT);

  /*MOTORS*/

  /*
    !!!NIE ZMIENIAĆ UŁOŻENIA!!!
  Tak zainicjowane enkodery poprawnie zliczają kroki i kierunek obrotów
  */
  Left_enc.begin(LEFT_ENC_2, LEFT_ENC_1);
  Right_enc.begin(RIGHT_ENC_1,RIGHT_ENC_2);

  Controller.begin();
  //pinMode(LEFT_PWM, OUTPUT);
  //pinMode(RIGHT_PWM, OUTPUT);

  /*IR SENSORS*/
  for (int i = 0; i < 20; i++)
  {
    pinMode(IR_Sensors.getSensorsPins()[i], INPUT);
  }

  // Wire.setPins(SDA_I2C, SCL_I2C);
}

/*Funkcja do obsługi liczenia prędkości co określony czas*/
bool IRAM_ATTR TimerHandler0(void * timerNo){
  Left_enc.calc_speed();
  Right_enc.calc_speed();
  return true;
}

void setup()
{
  Serial.begin(115200);
  GPIOSetup();
  //Wywołanie przerwania do wyzwalania pomiaru prędkości
  if (Timer0.attachInterruptInterval(TIMER0_INTERVAL_MS*1000,TimerHandler0))
  {
		Serial.print(F("Starting  ITimer0 OK, millis() = "));
		Serial.println(millis());  
    }
  
  setupWifi();
  //Wire.begin();
}

void loop()
{

  
/*
  Serial.print("Enkodery: L:\t");
  Serial.print(Left_enc.get_speed(),6);
  Serial.print("\trotations: ");
  Serial.print(Left_enc.get_rotations());
  Serial.print(" R:\t");
  Serial.print(Right_enc.get_speed(),6);
  Serial.print("\trotations: ");
  Serial.print(Right_enc.get_rotations());
  Serial.print("\n");

*/

  clientRead(); // <== Odbiera dane od clienta (aplikacji Qt) i zapisuje odczytane wartosci w strukturze received_data

  switch (received_data.getInstruction())
  {
  case 'S': // S czyli start jazdy
    robot_status = 1; //<== status 1 czyli jedzie
    break;

  case 'M': // M czyli stop jazdy
    robot_status = 0;
    break;
  }
  
  makeMeasuresAndCalculations();

  client.println(data_to_send.createDataFrame()); // <== wysyla utworzona ramke danych ze struktury data_to_send do clienta (aplikacji Qt)

  //analogWrite(LEFT_PWM, Left_pwm_value);
  //analogWrite(RIGHT_PWM, Right_pwm_value);
 
}
