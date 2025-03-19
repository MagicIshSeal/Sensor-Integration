#ifndef LOGGER_H
#define LOGGER_H

#include <FRTimer.h>
#include <FRLogger.h>
#include <FRMPU9250.h>
#include <FRBMP280.h>
#include <FRAS5600.h>
#include <FRTinyGPS.h>
#include <FRMS4525DO.h>
#include <Arduino.h>
#include <FRPPMReceiver.h>

const int buttonPin = 35;
const int loggerPin = 4; // The pin number for the button to start and stop logging

const float LAT0 = 51.99751239776191; // Latitude of null reference location, in this case the Terminal
const float LON0 = 4.369074612612849; // Longitude of null reference location, in this case the Terminal

const float offsetAngle = 0;

const int LOGGERLOOPTIMEMS = 100; // Loop time for logging

bool ButtonPressed = false;

Timer myLoggerTimer(LOGGERLOOPTIMEMS);
Logger myLogger;
FRMPU9250 myIMUSensor;
FRBMP280 myAltitudeSensor;
FRAS5600 myAOASensor;
FRTinyGPS myGPSSensor;
FRMS4525DO myPitotSensor;
extern FRPPMReceiver myPPMReceiver;
extern RGBLED myLED;
extern SSD1306AsciiWire myOLED;

void setupSensors()
{

    if (!myLogger.CheckSD())
    {
        // Serial.println("No SD Card");
        Message("SD Card not found", myLED, RED, Serial, myOLED);
    }

    if (!myIMUSensor.Init(Wire))
    {
        // Serial.println("Failed to initialize IMU sensor");
        Message("IMU sensor not found", myLED, RED, Serial, myOLED);
    }

    if (!myAltitudeSensor.Init(Wire))
    {
        // Serial.println("Failed to initialize Altitude sensor");
        Message("Altitude sensor not found", myLED, RED, Serial, myOLED);
    }

    if (!myAOASensor.Init())
    {
        // Serial.println("Failed to initialize AOA sensor");
        Message("AOA sensor not found", myLED, RED, Serial, myOLED);
    }

    if (!myGPSSensor.Init())
    {
        // Serial.println("Failed to initialize GPS sensor");
        Message("GPS sensor not found", myLED, RED, Serial, myOLED);
    }

    if (!myPitotSensor.Init(Wire))
    {
        // Serial.println("Failed to initialize Pitot sensor");
        Message("Pitot sensor not found", myLED, RED, Serial, myOLED);
    }
    // Serial.println("Sensors Initialised");
    Message("Sensors Initialised", myLED, GREEN, Serial, myOLED);
}

void calibrateSensor()
{
    while (ButtonPressed == false)
    {
        // Serial.println("Press the button to calibrate the sensors");
        Message("Press the button to calibrate the sensors", myLED, YELLOW, Serial, myOLED);
        Serial.println(digitalRead(buttonPin));
        if (digitalRead(buttonPin) == 0)
        {
            ButtonPressed = true;
            // Serial.println("Calibrating sensors");
            Message("Calibrating sensors", myLED, YELLOW, Serial, myOLED);
        }
        delay(100);
    }

    myAltitudeSensor.AutoOffset();
    myIMUSensor.AutoOffsetGyro();
    myPitotSensor.AutoOffset();
    myAOASensor.AutoOffset();
    Message("Sensors Calibrated", myLED, GREEN, Serial, myOLED);
    delay(1000);
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

void PrintGPSStatusToOLED(FRTinyGPS &_GPSSensor, SSD1306AsciiWire &_OLED)
{
    while (!_GPSSensor.HasValidData())
    {
        _OLED.setRow(2);
        _OLED.print("# Sats found: ");
        _OLED.print(_GPSSensor.GetSatellites());
        _OLED.println("   "); // some extra blank characters to overwrite old characters on that line
        _OLED.println("No GPS fix");
        delay(500); // Wait before checking again
    }
    _OLED.println("GPS fix    ");
}

void loggerSetup()
{
    pinMode(buttonPin, INPUT);
    setupSensors();
    sensorAdd();
    calibrateSensor();
    PrintGPSStatusToOLED(myGPSSensor, myOLED);
    myLoggerTimer.Start();
}

void loggerLoop()
{
    if (myPPMReceiver.IsChannelHigh(loggerPin) == 0 && myLogger.IsLogging() == false)
    {
        myLogger.StartLogger();
        String logMessage = "Logging Started in: " + String(myLogger.GetLoggerFileName());
        Message(logMessage, myLED, GREEN, Serial, myOLED);
        myLogger.UpdateSensors();
        myLogger.WriteLogger();
    }
    else if (myPPMReceiver.IsChannelHigh(loggerPin) == 1 && myLogger.IsLogging() == true)
    {
        myLogger.StopLogger();
        String logMessage = "Logging Stopped, data saved in: " + myLogger.GetLoggerFileName();
        Message(logMessage, myLED, GREEN, Serial, myOLED);
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