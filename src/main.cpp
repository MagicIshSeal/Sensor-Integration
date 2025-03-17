#include <Arduino.h>
<<<<<<< HEAD
#include "logger.h"
=======
#include "init.h"

int i = 0;
bool loggerRunning = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = LOW;
int buttonState = LOW;
>>>>>>> 3a1b68a241ae12bb70325c166f7752ba0d4ade4a

void setup()
{
  Serial.begin(9600);
<<<<<<< HEAD
  loggerSetup();
=======
  pinMode(PINSWITCH, INPUT);
  setupLogger();
>>>>>>> 3a1b68a241ae12bb70325c166f7752ba0d4ade4a
}

void loop()
{
<<<<<<< HEAD
  loggerLoop();
=======
  
  handleButtonPress();
  if (loggerRunning)
  {
    String myString = myLogger.UpdateSensors();
    myLogger.WriteLogger();
  }

  delay(100);
>>>>>>> 3a1b68a241ae12bb70325c166f7752ba0d4ade4a
  //  put your main code here, to run repeatedly:
}
