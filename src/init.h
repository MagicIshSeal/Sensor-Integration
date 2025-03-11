#ifndef INIT_H
#define INIT_H

#include <FRLogger.h>
#include <FRMPU9250.h>
#include <FRBMP280.h>
#include <FRAS5600.h>
#include <FRTinyGPS.h>
#include <FRMS4525DO.h>

const byte I2C_SDA = 21;   // The data pin for I2C communication
const byte I2C_SCL = 22;   // The clock pin for I2C communcation
const byte PINSWITCH = 35; // The pin number for he button to start and stop logging

const float LAT0 = 51.99751239776191; // Latitude of null reference location, in this case the Terminal
const float LON0 = 4.369074612612849; // Longitude of null reference location, in this case the Terminal

const float offsetAngle = 0;

const int LOGGERLOOPTIMEMS = 100; // Loop time for logging

Logger myLogger;
FRMPU9250 myIMUSensor;
FRBMP280 myAltitudeSensor;
FRAS5600 myAOASensor;
FRTinyGPS myGPSSensor;
FRMS4525DO myPitotSensor;

#endif