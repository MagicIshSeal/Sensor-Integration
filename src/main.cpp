#include <Arduino.h>
#include "init.h"

int i = 0;
bool loggerRunning = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = LOW;
int buttonState = LOW;
void calibrateSensor()
{
  myAltitudeSensor.AutoOffset();
  myIMUSensor.AutoOffsetGyro();
  myPitotSensor.AutoOffset();
  myAOASensor.AutoOffset();
}

void handleButtonPress()
{
  int reading = digitalRead(PINSWITCH);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        loggerRunning = !loggerRunning;
        if (loggerRunning)
        {
          Serial.println("Logger started");
          myLogger.StartLogger();
        }
        else
        {
          Serial.println("Logger stopped");
          myLogger.StopLogger();
        }
      }
    }
  }

  lastButtonState = reading;
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
  pinMode(PINSWITCH, INPUT);

  setupSensors();
  sensorAdd();
  calibrateSensor();
  myLoggerTimer.Start();
}

void loop()
{
  handleButtonPress();
  Serial.println(myAOASensor.GetAngle());
  if (loggerRunning)
  {
    String myString = myLogger.UpdateSensors();
    myLogger.WriteLogger();
  }

  delay(100);
  //  put your main code here, to run repeatedly:
}
