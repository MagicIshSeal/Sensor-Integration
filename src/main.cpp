#include <Arduino.h>
#include "logger.h"

void setup()
{
  Serial.begin(9600);
  loggerSetup();
}

void loop()
{
  loggerLoop();
  //  put your main code here, to run repeatedly:
}
