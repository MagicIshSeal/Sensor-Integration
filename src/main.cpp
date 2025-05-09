#include <Arduino.h>
#include <FRPPMReceiver.h>
#include "logger.h"
#include "NLG.h"
// #include "cargo_code.h"
#include "state.h"
#include "test.h"

// Define constants for PPM receiver
const byte number_chan = 10; // Number of PPM channels
const byte ppm_pin = 4;      // Pin for PPM signal

// Create PPM receiver object
FRPPMReceiver myPPMReceiver(ppm_pin, number_chan);

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize I2C communication
   Wire.begin();

  // Initialize PPM receiver
   myPPMReceiver.Init();

  // Setup OLED display
   OLEDSetup();

  // Setup nose landing gear
   NLGSetup();
  // Setup logger
   loggerSetup();

  // Uncomment to setup servos for cargo bay / test code
  //servoSetup();
}

void loop()
{
  // Run logger loop
  loggerLoop();

  // Run nose landing gear loop
  NLGLoop();

  // Uncomment to run servo loop for cargo bay / test code
  //servoLoop();

  // Add your main code here to run repeatedly
}
