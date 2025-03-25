#include <Arduino.h>
#include <FRPPMReceiver.h>
#include "logger.h"
#include "NLG.h"
// #include "cargo_code.h"
#include "state.h"
#include "test.h"
const byte number_chan = 10;
const byte ppm_pin = 4;

FRPPMReceiver myPPMReceiver(ppm_pin, number_chan);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  myPPMReceiver.Init();
  OLEDSetup();
  loggerSetup();
  NLGSetup();
  // servoSetup();
}

void loop()
{
  loggerLoop();
  NLGLoop();
  // servoLoop();
  //   put your main code here, to run repeatedly:
}
