#include <Arduino.h>
#include <FRPPMReceiver.h>
#include "logger.h"
//#include "servo.h"

const byte number_chan = 10;
const byte ppm_pin = 4;

FRPPMReceiver myPPMReceiver(ppm_pin, number_chan);

void setup()
{
  myPPMReceiver.Init();
  Serial.begin(9600);
  loggerSetup();
  //servoSetup();
}

void loop()
{
  loggerLoop();
  //servoLoop();
  //  put your main code here, to run repeatedly:
}
