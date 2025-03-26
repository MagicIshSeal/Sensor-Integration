#ifndef CARGO_H
#define CARGO_H

#include <FRTimer.h>
#include <FRPPMReceiver.h>
#include <ESP32Servo.h>
#include "smoothservo.h"

// Constants for servos
const byte NUMBEROFSERVOS = 2;                  // Number of servos
const byte PINSERVO[NUMBEROFSERVOS] = {12, 14}; // Servo pin numbers
const int LOOPTIMESERVOMS = 100;                // Loop time for servo control

// Timer for cargo control
Timer myCargoTimer(LOOPTIMESERVOMS);

// Smooth servo objects
SmoothServo myCargoServo[NUMBEROFSERVOS];

// External PPM receiver
extern FRPPMReceiver myPPMReceiver;

// Constants for cargo bay control
const byte CARGOCHANNEL = 6; // PPM channel for cargo bay
triStateSwitch CargoBaySwitchState;
triStateSwitch CargoBaySwitchStatePrev;

// Servo positions and speeds
const byte CARGOBAY = 0;
const byte BALLOONS = 1;
const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 60};
const byte SERVOSTARTPOS[NUMBEROFSERVOS] = {130, 0};
const byte SERVOENDPOS[NUMBEROFSERVOS] = {13, 176};

// Balloon drop sequence
const byte stepSequence[] = {0, 60, 90, 120, 180};
const byte stepCount = sizeof(stepSequence) / sizeof(stepSequence[0]);
byte balloonStepCounter = 0;

// Function to handle cargo bay switch
void HandleMainLandingGearSwitch(triStateSwitch _SwitchState, triStateSwitch _SwitchStatePrev, SmoothServo &_ServoGear, SmoothServo &_ServoHatch)
{
    if (_SwitchState < _SwitchStatePrev)
    {
        if (_SwitchState == MIDSTATE)
        {
            _ServoGear.SetTargetEnd();
            Serial.println("Open Cargobay");
        }
        else
        {
            if (balloonStepCounter < stepCount)
            {
                _ServoHatch.SetTargetPos(stepSequence[balloonStepCounter]);
                Serial.print("Drop Balloon: Turn to ");
                Serial.print(stepSequence[balloonStepCounter]);
                Serial.println(" degrees");
                balloonStepCounter++;
            }
            else
            {
                Serial.println("No more balloons to drop.");
                balloonStepCounter = 0;
            }
        }
    }
    else if (_SwitchState > _SwitchStatePrev)
    {
        if (_SwitchState == MIDSTATE)
        {
            // Do nothing when switching from HIGH to MID
        }
        else
        {
            _ServoGear.SetTargetStart();
            Serial.println("Close Cargobay");
        }
    }
}

// Function to setup servos
void servoSetup()
{
    for (int i = 0; i < NUMBEROFSERVOS; i++)
    {
        float maxStep = MAXSERVOSPEEDDEGS[i] * LOOPTIMESERVOMS / 1000.0;
        myCargoServo[i].Init(PINSERVO[i], maxStep, SERVOSTARTPOS[i], SERVOENDPOS[i]);
    }

    myCargoTimer.Start();
    Serial.println("End of Setup");
}

// Function to handle servo loop
void servoLoop()
{
    CargoBaySwitchState = myPPMReceiver.GetChannelTriState(CARGOCHANNEL);
    HandleMainLandingGearSwitch(CargoBaySwitchState, CargoBaySwitchStatePrev, myCargoServo[CARGOBAY], myCargoServo[BALLOONS]);
    CargoBaySwitchStatePrev = CargoBaySwitchState;

    for (int i = 0; i < NUMBEROFSERVOS; i++)
    {
        myCargoServo[i].Update();
    }

    if (myCargoTimer.WaitUntilEnd())
    {
        Serial.println("Overrun!");
    }
}

#endif
