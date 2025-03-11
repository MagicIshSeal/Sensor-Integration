#include <Arduino.h>
#include "init.h"
#include "func.h"

void calibrateSensor()
{
  myAltitudeSensor.AutoOffset();
  myIMUSensor.SetOffsetAcc(0.0, 0.0, 9.81);
  myPitotSensor.AutoOffset();
  myAOASensor.AutoOffset();
}

void sensorCheck()
{
  if (!myAltitudeSensor.Init(Wire))
  {
    Serial.println("BMP280 not found!");
  }
  if (!myIMUSensor.Init(Wire))
  {
    Serial.println("MPU9250 not found!");
  }
  if (!myPitotSensor.Init(Wire))
  {
    Serial.println("Pitot not found!");
  }
  if (!myGPSSensor.Init(LAT0, LON0))
  {
    Serial.println("GPS not found!");
  }
  if (!myAOASensor.Init(offsetAngle))
  {
    Serial.println("AOA not found!");
  }
  if (!myLogger.CheckSD())
  {
    Serial.println("SD not found!");
  }
}

void sensorAdd()
{
  myLogger.AddSensor(&myAltitudeSensor);
  myLogger.AddSensor(&myIMUSensor);
  myLogger.AddSensor(&myPitotSensor);
  myLogger.AddSensor(&myGPSSensor);
  myLogger.AddSensor(&myAOASensor);
}

void setup()
{
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  sensorCheck();
  sensorAdd();
  calibrateSensor();
  Serial.println(myLogger.GetLoggerFileName());
}

void loop()
{
  myLogger.StartLogger();

  String myString = myLogger.UpdateSensors();
  Serial.println(myString);
  myLogger.WriteLogger();
  // put your main code here, to run repeatedly:
}
