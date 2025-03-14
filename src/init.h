#ifndef INIT_H
#define INIT_H

#include <FRTimer.h>
#include <FRLogger.h>
#include <FRMPU9250.h>
#include <FRBMP280.h>
#include <FRAS5600.h>
#include <FRTinyGPS.h>
#include <FRMS4525DO.h>
#include <FRPPMReceiver.h>

const int PINSWITCH = 35; // The pin number for the button to start and stop logging

const float LAT0 = 51.99751239776191; // Latitude of null reference location, in this case the Terminal
const float LON0 = 4.369074612612849; // Longitude of null reference location, in this case the Terminal

const float offsetAngle = 0;

const int LOGGERLOOPTIMEMS = 100; // Loop time for logging

FRPPMReceiver myReceiver;
Timer myLoggerTimer(LOGGERLOOPTIMEMS);
Logger myLogger;
FRMPU9250 myIMUSensor;
FRBMP280 myAltitudeSensor;
FRAS5600 myAOASensor;
FRTinyGPS myGPSSensor;
FRMS4525DO myPitotSensor;

void setupSensors()
{
    Wire.begin(21, 22);
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

void setupLogger()
{
    setupSensors();
    calibrateSensor();
    sensorAdd();
    myLoggerTimer.Start();
}

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

#endif