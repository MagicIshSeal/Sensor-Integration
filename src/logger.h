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
#include <FRPPMReceiverSensor.h>
#include "state.h"

// Pin definitions
const int buttonPin = 35; // Pin for button to start/stop logging
const int loggerPin = 4;  // PPM channel for logging control

// GPS reference location
const float LAT0 = 51.92788946956197; // Latitude of reference point
const float LON0 = 4.214285252110042; // Longitude of reference point

// Logger loop timing
const int LOGGERLOOPTIMEMS = 100; // Loop time in milliseconds

// Global variables
bool ButtonPressed = false; // Tracks button press state

// Timer for logger loop
Timer myLoggerTimer(LOGGERLOOPTIMEMS);

// Logger and sensor objects
Logger myLogger;
FRMPU9250 myIMUSensor;
FRBMP280 myAltitudeSensor;
FRAS5600 myAOASensor;
FRTinyGPS myGPSSensor;
FRMS4525DO myPitotSensor;

// External objects
extern FRPPMReceiver myPPMReceiver;
extern RGBLED myLED;
extern SSD1306AsciiWire myOLED;

// Function to handle errors and allow bypass
void bypassError(String errorString)
{
    unsigned long buttonPressStart = 0;
    bool buttonHeld = false;
    bool progressDisplayed = false; // Flag to track if the progress bar is already displayed

    while (true)
    {
        // Display the error message and "Hold to Bypass" only once
        if (!progressDisplayed)
        {
            Message(errorString, myLED, RED, Serial, myOLED);
            myOLED.setRow(2); // Display "Hold to Bypass" on the second row
            myOLED.print("Hold to Bypass");
            progressDisplayed = true; // Set the flag to true
        }

        // Progress bar variables
        const int progressBarWidth = 20;     // Number of segments in the progress bar
        const unsigned long holdTime = 2000; // Time required to bypass in milliseconds

        if (digitalRead(buttonPin) == LOW) // Button pressed
        {
            if (buttonPressStart == 0)
            {
                buttonPressStart = millis(); // Start timing
            }
            else
            {
                unsigned long elapsedTime = millis() - buttonPressStart;

                // Calculate progress as a fraction of the hold time
                float progress = (float)elapsedTime / holdTime;
                if (progress > 1.0)
                    progress = 1.0; // Cap progress at 100%

                // Draw the progress bar
                myOLED.setCursor(0, 3); // Set to the first column of the last row
                myOLED.clearToEOL();    // Clear the row before drawing
                int filledSegments = progress * progressBarWidth;
                for (int i = 0; i < filledSegments; i++)
                {
                    myOLED.print("="); // Draw filled segments
                }

                // Check if the button has been held long enough
                if (elapsedTime >= holdTime)
                {
                    buttonHeld = true;
                    break;
                }
            }
        }
        else
        {
            buttonPressStart = 0;   // Reset timing if button is released
            myOLED.setCursor(0, 3); // Reset to the first column of the last row
            myOLED.clearToEOL();    // Clear the progress bar if the button is released
        }

        delay(100); // Small delay to avoid rapid updates
    }

    if (buttonHeld)
    {
        myOLED.clear();
        myOLED.println("Bypass!");
        delay(1000); // Show "Bypass!" message for 1 second
    }
}

// Function to initialize sensors
void setupSensors()
{
    if (!myLogger.CheckSD())
    {
        bypassError("SD card not found");
    }

    if (!myIMUSensor.Init(Wire))
    {
        bypassError("IMU not found");
    }

    if (!myAltitudeSensor.Init(Wire))
    {
        bypassError("Barometer not found");
    }

    if (!myAOASensor.Init())
    {
        bypassError("AOA sensor found");
    }

    if (!myGPSSensor.Init())
    {
        bypassError("GPS not found");
    }

    if (!myPitotSensor.Init(Wire))
    {
        bypassError("Pitot not found");
    }
    Message("Sensors Initialised", myLED, GREEN, Serial, myOLED);
    delay(1000);
}

// Function to calibrate sensors
void calibrateSensor()
{
    bool messageDisplayed = false; // Flag to track if the message is already displayed

    while (ButtonPressed == false)
    {
        if (!messageDisplayed)
        {
            // Display the calibration message only once
            myOLED.clear();
            myOLED.setRow(0);
            Message("Press the button to", myLED, YELLOW, Serial, myOLED);
            myOLED.setRow(1);
            myOLED.println("calibrate the sensors");
            messageDisplayed = true; // Set the flag to true
        }

        // Check if the button is pressed
        if (digitalRead(buttonPin) == 0)
        {
            ButtonPressed = true;
            myOLED.clear();
            Message("Calibrating sensors", myLED, YELLOW, Serial, myOLED);
            delay(1000); // Show "Calibrating sensors" message for 1 second
        }

        delay(100); // Small delay to reduce CPU usage
    }

    // Perform sensor calibration
    myAltitudeSensor.AutoOffset();
    myIMUSensor.AutoOffsetGyro();
    myPitotSensor.AutoOffset();
    myAOASensor.AutoOffset();

    // Display calibration completion message
    myOLED.clear();
    Message("Sensors Calibrated", myLED, GREEN, Serial, myOLED);
    delay(1000);
}

// Function to add sensors to the logger
void sensorAdd()
{
    myLogger.AddSensor(&myAltitudeSensor);
    myLogger.AddSensor(&myIMUSensor);
    myLogger.AddSensor(&myPitotSensor);
    myLogger.AddSensor(&myGPSSensor);
    myLogger.AddSensor(&myAOASensor);
    myLogger.AddSensor(&myPPMReceiver);
}

// Function to display GPS status on OLED
void PrintGPSStatusToOLED()
{
    myOLED.clear();
    unsigned long buttonPressStart = 0;
    bool buttonHeld = false;
    bool progressDisplayed = false;

    while (!myGPSSensor.HasValidData())
    {
        myLogger.UpdateSensors();
        // Display GPS status and "Hold to Bypass" message
        if (!progressDisplayed)
        {
            myOLED.setRow(0);
            myOLED.print("Hold to Bypass");
            progressDisplayed = true;
        }
        myOLED.setCursor(0, 1);
        myOLED.print("# Sats found: ");
        myOLED.print(myGPSSensor.GetSatellites());

        myOLED.println("   "); // Extra blank characters to overwrite old data
        myOLED.setRow(3);
        myLED.SetColor(RED);
        myOLED.println("No GPS fix");

        // Progress bar variables
        const int progressBarWidth = 20;     // Number of segments in the progress bar
        const unsigned long holdTime = 2000; // Time required to bypass in milliseconds

        if (digitalRead(buttonPin) == LOW) // Button pressed
        {
            if (buttonPressStart == 0)
            {
                buttonPressStart = millis(); // Start timing
            }
            else
            {
                unsigned long elapsedTime = millis() - buttonPressStart;

                // Calculate progress as a fraction of the hold time
                float progress = (float)elapsedTime / holdTime;
                if (progress > 1.0)
                    progress = 1.0; // Cap progress at 100%

                // Draw the progress bar
                myOLED.setCursor(0, 2); // Set to the first column of the second-to-last row
                myOLED.clearToEOL();    // Clear the row before drawing
                int filledSegments = progress * progressBarWidth;
                for (int i = 0; i < filledSegments; i++)
                {
                    myOLED.print("="); // Draw filled segments
                }

                // Check if the button has been held long enough
                if (elapsedTime >= holdTime)
                {
                    buttonHeld = true;
                    break;
                }
            }
        }
        else
        {
            buttonPressStart = 0; // Reset timing if button is released
            myOLED.setCursor(0, 2);
            myOLED.clearToEOL(); // Clear the progress bar if the button is released
        }

        delay(1000); // Wait before checking again
    }

    if (buttonHeld)
    {
        myOLED.clear();
        myOLED.println("Bypass!");
        delay(1000); // Show "Bypass!" message for 1 second
    }
    else
    {
        myOLED.clear();
        Message("GPS Fix Acquired", myLED, GREEN, Serial, myOLED);
        delay(2000);
    }
}

// Function to setup logger
void loggerSetup()
{
    pinMode(buttonPin, INPUT);
    setupSensors();
    sensorAdd();
    calibrateSensor();
    myLoggerTimer.Start();
    PrintGPSStatusToOLED();
}

// Function to handle logger loop
void loggerLoop()
{
    if (myPPMReceiver.IsChannelHigh(loggerPin) == 0 && myLogger.IsLogging() == false)
    {
        myLogger.StartLogger();
        String logMessage1 = "Logging Started in: ";
        String logMessage2 = myLogger.GetLoggerFileName();
        myOLED.setRow(0);
        Message(logMessage1, myLED, GREEN, Serial, myOLED);
        myOLED.setRow(1);
        myOLED.println(logMessage2);
        myLogger.UpdateSensors();
        myLogger.WriteLogger();
    }
    else if (myPPMReceiver.IsChannelHigh(loggerPin) == 1 && myLogger.IsLogging() == true)
    {
        myLogger.StopLogger();
        String logMessage1 = "Logging Stopped, ";
        String logMessage2 = "data saved in: ";
        String logMessage3 = myLogger.GetLoggerFileName();
        myOLED.setRow(0);
        Message(logMessage1, myLED, GREEN, Serial, myOLED);
        myOLED.setRow(1);
        myOLED.println(logMessage2);
        myOLED.setRow(2);
        myOLED.println(logMessage3);
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