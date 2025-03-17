#ifndef INIT_H
#define INIT_H

#include <FRTimer.h>
#include <FRLogger.h>
#include <FRMPU9250.h>
#include <FRBMP280.h>
#include <FRAS5600.h>
#include <FRTinyGPS.h>
#include <FRMS4525DO.h>
#include <Arduino.h>
#include <FRPPMReceiver.h>

const byte number_chan = 8;
const byte ppm_pin = 4;
int chanval[number_chan];
const int loggerPin = 5; // The pin number for the button to start and stop logging

const float LAT0 = 51.99751239776191; // Latitude of null reference location, in this case the Terminal
const float LON0 = 4.369074612612849; // Longitude of null reference location, in this case the Terminal

const float offsetAngle = 0;

const int LOGGERLOOPTIMEMS = 100; // Loop time for logging

Timer myLoggerTimer(LOGGERLOOPTIMEMS);
Logger myLogger;
FRMPU9250 myIMUSensor;
FRBMP280 myAltitudeSensor;
FRAS5600 myAOASensor;
FRTinyGPS myGPSSensor;
FRMS4525DO myPitotSensor;
FRPPMReceiver myPPMReceiver(ppm_pin, number_chan);

void setupSensors()
{

    Wire.begin();
    myPPMReceiver.Init();
    if (!myLogger.CheckSD())
    {
        Serial.println("No SD Card");
    }

    if (!myIMUSensor.Init(Wire))
    {
        Serial.println("Failed to initialize IMU sensor");
    }

    if (!myAltitudeSensor.Init(Wire))
    {
        Serial.println("Failed to initialize Altitude sensor");
    }

    if (!myAOASensor.Init())
    {
        Serial.println("Failed to initialize AOA sensor");
    }

    if (!myGPSSensor.Init())
    {
        Serial.println("Failed to initialize GPS sensor");
    }

    if (!myPitotSensor.Init(Wire))
    {
        Serial.println("Failed to initialize Pitot sensor");
    }
    Serial.println("Sensors Initialised");
}

void calibrateSensor()
{
    myAltitudeSensor.AutoOffset();
    myIMUSensor.AutoOffsetGyro();
    myPitotSensor.AutoOffset();
    myAOASensor.AutoOffset();
}

void sensorAdd()
{
    myLogger.AddSensor(&myAltitudeSensor);
    myLogger.AddSensor(&myIMUSensor);
    myLogger.AddSensor(&myPitotSensor);
    myLogger.AddSensor(&myGPSSensor);
    myLogger.AddSensor(&myAOASensor);
    myLogger.AddSensor(&myPPMReceiver);
}

void loggerSetup()
{
    setupSensors();
    calibrateSensor();
    sensorAdd();
    myLoggerTimer.Start();
}

void loggerLoop()
{
    if (myPPMReceiver.IsChannelHigh(loggerPin) == 0 && myLogger.IsLogging() == false)
    {
        myLogger.StartLogger();
        Serial.print("Logging Started in: ");
        Serial.println(myLogger.GetLoggerFileName());
        myLogger.UpdateSensors();
        myLogger.WriteLogger();
    }
    else if (myPPMReceiver.IsChannelHigh(loggerPin) == 1 && myLogger.IsLogging() == true)
    {
        myLogger.StopLogger();
        Serial.print("Logging Stopped, data saved in: ");
        Serial.println(myLogger.GetLoggerFileName());
    }
    else
    {
        myLogger.UpdateSensors();
        myLogger.WriteLogger();
        if (myLoggerTimer.WaitUntilEnd())
        {
            Serial.println("Overrun!"); // if there are delays in the loop, you will get overruns i.e. the loop took longer than the looptime
        }
    }
}
#endif