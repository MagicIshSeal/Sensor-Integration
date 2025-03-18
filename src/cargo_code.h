#ifndef CARGO_H
#define CARGO_H

#include <FRTimer.h>
#include <FRPPMReceiver.h>
#include <ESP32Servo.h>
#include "smoothservo.h"

const byte NUMBEROFSERVOS = 2;
const byte PINSERVO[NUMBEROFSERVOS] = {12, 14};
const int LOOPTIMESERVOMS = 100;

Timer myCargoTimer(LOOPTIMESERVOMS);
SmoothServo myCargoServo[NUMBEROFSERVOS];
extern FRPPMReceiver myPPMReceiver;

const byte CARGOCHANNEL = 6;
triStateSwitch CargoBaySwitchState;
triStateSwitch CargoBaySwitchStatePrev;

const byte CARGOBAY = 0;
const byte BALLOONS = 1;
const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 60};
const byte SERVOSTARTPOS[NUMBEROFSERVOS] = {130, 0};
const byte SERVOENDPOS[NUMBEROFSERVOS] = {13, 176};

const byte stepSequence[] = {0, 60, 90, 120, 180};
const byte stepCount = sizeof(stepSequence) / sizeof(stepSequence[0]);
byte balloonStepCounter = 0;

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
