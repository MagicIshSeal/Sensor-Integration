#ifndef NLG_H
#define NLG_H

#include <FRTimer.h>
#include <FRPPMReceiver.h>
#include <ESP32Servo.h>
#include "smoothservo.h"
#include "state.h"

// Constants for servos
const byte NumServo = 2;                  // Number of servos
const byte ServoPin[NumServo] = {26, 27}; // Servo pin numbers
const int NLGServoTime = 100;             // Loop time for servo control

// Timer for nose landing gear
Timer myNLGTimer(NLGServoTime);

// Smooth servo objects
SmoothServo myNLGServo[NumServo];

// External objects
extern FRPPMReceiver myPPMReceiver;
extern SSD1306AsciiWire myOLED;

// PPM channel for landing gear
const byte LANDINGGEARCHANNEL = 5;

// Landing gear switch states
triStateSwitch landingGearSwitchState;
triStateSwitch landingGearSwitchStatePrev;

// Servo parameters
const byte SERVOLANDINGGEAR = 1;
const byte SERVOLANDINGHATCH = 0;
const int NLGMaxServoSpeed[NumServo] = {60, 60};
const byte NLGStartPos[NumServo] = {88, 30};
const byte NLGEndPos[NumServo] = {180, 180};
const int minTime[NumServo] = {500, 900};
const int maxTime[NumServo] = {2400, 2100};

// Function to handle landing gear switch
void HandleNoseLandingGearSwitch(triStateSwitch _SwitchState, triStateSwitch _SwitchStatePrev, SmoothServo &_ServoGear, SmoothServo &_ServoHatch)
{
    // landingGearSwitchState can have the state LOSTATE (-1), MIDSTATE (0), or HISTATE (1)
    // the function GetChannelTriState returns one of these three states depending on the value of the channel

    if (_SwitchState > _SwitchStatePrev)
    {
        // towards rectracted state
        if (_SwitchState == MIDSTATE)
        {
            // So it was HISTATE, now pull in the gear
            _ServoGear.SetTargetStart();
            // Serial.println("Retract gear");

            Message("Retract gear", myLED, YELLOW, Serial, myOLED);
        }
        else
        {
            // So it was in MIDSTATE, now close the hatch
            _ServoHatch.SetTargetStart();
            // Serial.println("Close hatch");
            Message("Close hatch", myLED, YELLOW, Serial, myOLED);
        }
    }
    if (_SwitchState < _SwitchStatePrev)
    {
        // towards extended state
        if (_SwitchState == MIDSTATE)
        {
            // So it was LOSTATE, now open the hatch
            _ServoHatch.SetTargetEnd();
            // Serial.println("Open hatch");
            Message("Open hatch", myLED, YELLOW, Serial, myOLED);
        }
        else
        {
            // So it was in MIDSTATE, now extend the gear
            _ServoGear.SetTargetEnd();
            // Serial.println("Extend gear");
            Message("Extend gear", myLED, YELLOW, Serial, myOLED);
        }
    }
}

// Function to setup nose landing gear
void NLGSetup()
{
    for (int i = 0; i < NumServo; i++)
    {
        float maxStep = NLGMaxServoSpeed[i] * NLGServoTime / 1000.0;
        myNLGServo[i].Init(ServoPin[i], maxStep, NLGStartPos[i], NLGEndPos[i], minTime[i], maxTime[i]);
        myNLGServo[i].SetTargetEnd(); // Ensure servos are in their closed position
    }

    myNLGTimer.Start();
    // Serial.println("End of Setup");
}

// Function to handle nose landing gear loop
void NLGLoop()
{
    // Read switch SWC (high/mid/low) for landing gear.
    landingGearSwitchState = myPPMReceiver.GetChannelTriState(LANDINGGEARCHANNEL);
    // Check if landing gear and hatch need to move
    HandleNoseLandingGearSwitch(landingGearSwitchState, landingGearSwitchStatePrev, myNLGServo[SERVOLANDINGGEAR], myNLGServo[SERVOLANDINGHATCH]);
    landingGearSwitchStatePrev = landingGearSwitchState; // update the previous state for the next loop

    // Update the smooth servos. There are some debugging print statements in the Update function you want to remove to speed up the code.
    for (int i = 0; i < NumServo; i++)
    {
        myNLGServo[i].Update();
    }

    // At the end of the loop, WaitUntilEnd runs until the time until looptime has passed
    if (myNLGTimer.WaitUntilEnd())
    {
        Serial.println("Overrun!");
    }
}

#endif