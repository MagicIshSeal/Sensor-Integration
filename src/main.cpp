#include <Arduino.h>
#include <FRPPMReceiver.h>
#include "logger.h"
#include "NLG.h"
//#include "cargo_code.h"

const byte number_chan = 10;
const byte ppm_pin = 4;

FRPPMReceiver myPPMReceiver(ppm_pin, number_chan);

void setup()
{
  Serial.begin(9600);
  myPPMReceiver.Init();
  loggerSetup();
  servoSetup();
}

void loop()
{
  loggerLoop();
  servoLoop();
  //  put your main code here, to run repeatedly:
}
