#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <MPU6050.h>
#include <AHRS.h>
#include "ESP32TimerInterrupt.h"

#include "DataFrame.h"
#include "ReceivedData.h"
#include "defines.h"
#include "Sensors.h"
#include "Encoder.h"
#include "PID.h"

//Zmienna określająca częstotliwość pomiaru czasu
#define TIMER0_INTERVAL_MS        50

#define BATTERY_MEAN_SAMPLES 10
#define BATTERY_LOOP_ITERATIONS 100


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

unsigned long last_time_encoder;

uint8_t battery_loop_counter = 0;
uint32_t battery = 0;

MPU6050 mpu;
unsigned long last_time_mpu;
volatile bool mpuInterrupt = false; 
AHRS ahrs;
float angleOffset = 0.0; 

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
    //Serial.println(msg);
    received_data.setData(msg);
  }
}


void IRAM_ATTR dmpDataReady() {
    mpuInterrupt = true;
}


void updateAngle() {
    if (mpuInterrupt) {
        mpuInterrupt = false;

        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        // Przekształć dane z czujnika do odpowiednich jednostek
        float accelX = ax / 16384.0; // 16384 = 2g przy pełnej skali ±2g
        float accelY = ay / 16384.0;
        float accelZ = az / 16384.0;
        float gyroX = gx / 131.0; // 131 = 250°/s przy pełnej skali ±250°/s
        float gyroY = gy / 131.0;
        float gyroZ = gz / 131.0;

        // Aktualizuj orientację za pomocą AHRS
        ahrs.update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ);

        // Pobierz aktualny kąt obrotu w osi Z (yaw)
        float yaw = ahrs.getYaw();

        // Normalizacja kąta do zakresu 0-359 stopni
        if (yaw < 0) {
            yaw += 360.0;
        }

        // Aktualizuj kąt względem offsetu
        float normalizedYaw = yaw - angleOffset;
        if (normalizedYaw < 0) {
            normalizedYaw += 360.0;
        } else if (normalizedYaw >= 360.0) {
            normalizedYaw -= 360.0;
        }

        Serial.print("Z_ROT = ");
        Serial.println(normalizedYaw);
    }
}

// Funkcja resetująca kąt do zera
void resetAngle() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Przekształć dane z czujnika do odpowiednich jednostek
    float accelX = ax / 16384.0;
    float accelY = ay / 16384.0;
    float accelZ = az / 16384.0;
    float gyroX = gx / 131.0;
    float gyroY = gy / 131.0;
    float gyroZ = gz / 131.0;

    // Aktualizuj orientację za pomocą AHRS
    ahrs.update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ);

    // Pobierz aktualny kąt obrotu w osi Z (yaw)
    angleOffset = ahrs.getYaw();

    // Normalizacja offsetu do zakresu 0-359 stopni
    if (angleOffset < 0) {
        angleOffset += 360.0;
    }

    Serial.println("Angle reset to zero.");
}

uint32_t calculateBatteryMean(uint8_t num_of_iteriations){
  uint32_t battery_sum = 0;

  for(int i = 0; i < num_of_iteriations; i++){
    battery_sum += analogRead(BATTERY);
  }

  return battery_sum / num_of_iteriations;

}

void makeMeasuresAndCalculations(){

  
  if(battery_loop_counter > BATTERY_LOOP_ITERATIONS){
    battery_loop_counter = 0;
    battery = calculateBatteryMean(BATTERY_MEAN_SAMPLES);
  }else{
    battery_loop_counter++;
  }

  IR_Sensors.readSensors();
  
  //tu trzeba potem dodac jeszcze pomiar z imu 
  if(robot_status == 1){
    resetAngle();
    Controller.set_pid(received_data.getPID_parameter(K_P),received_data.getPID_parameter(K_I), received_data.getPID_parameter(K_D));
    Controller.set_base_speed(received_data.getVMax());
    Controller.regulator(IR_Sensors.getSensorsError());
    Left_pwm_percent_value=Controller.get_left_percent();
    Right_pwm_percent_value= Controller.get_right_percent();

  //PCalculatePWM(IR_Sensors.getSensorsError(), received_data.getPID_parameter(K_P), received_data.getVMax(), &Left_pwm_percent_value, &Right_pwm_percent_value, &Left_pwm_value, &Right_pwm_value);
  }else if(robot_status == 0){
    Controller.set_pid(0,0,0);
    Left_pwm_percent_value = 0;
    Right_pwm_percent_value = 0;

    Left_pwm_value = 0;
    Right_pwm_value = 0;
    Controller.regulator(IR_Sensors.getSensorsError());
    Controller.set_base_speed(0);
    robot_z_position = 0;
  }

  updateAngle();

  data_to_send.setData(robot_status, IR_Sensors.getSensorsMeasures(), Left_pwm_percent_value, Right_pwm_percent_value, Left_enc.get_speed(), Right_enc.get_speed(), robot_z_position, battery );

}






void GPIOSetup()
{

  pinMode(BATTERY, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(IMU_INT, INPUT);
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

  Wire.setPins(SDA_I2C, SCL_I2C);
 
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
  Wire.begin();
  mpu = MPU6050(); // Domyślny adres 0x68
  mpu.initialize();
  battery = calculateBatteryMean(BATTERY_LOOP_ITERATIONS);

  //Wywołanie przerwania do wyzwalania pomiaru prędkości
  if (Timer0.attachInterruptInterval(TIMER0_INTERVAL_MS*1000,TimerHandler0))
  {
		Serial.print(F("Starting  ITimer0 OK, millis() = "));
		Serial.println(millis());  
  }
  
  setupWifi();


  last_time_encoder = millis();



  mpu.initialize();
  delay(100);

  Serial.println("MPU6050 gotowy");
  mpu.CalibrateGyro();

  attachInterrupt(digitalPinToInterrupt(IMU_INT), dmpDataReady, RISING);
  mpu.setIntDataReadyEnabled(true);
  last_time_mpu = millis();
}




void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    received_data.setInstruction('M');
  }



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
