#include <Arduino.h>
#include "init.h"

int i = 0;
bool loggerRunning = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = LOW;
int buttonState = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode(PINSWITCH, INPUT);
  setupLogger();
}

void loop()
{
  
  handleButtonPress();
  if (loggerRunning)
  {
    String myString = myLogger.UpdateSensors();
    myLogger.WriteLogger();
  }

  delay(100);
  //  put your main code here, to run repeatedly:
}
