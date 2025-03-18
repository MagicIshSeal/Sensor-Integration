#ifndef NLG_H
#define NLG_H

#include <FRTimer.h>
#include <FRPPMReceiver.h>
#include <ESP32Servo.h>
#include "smoothservo.h"

const byte NUMBEROFSERVOS = 2; // Number of Servos to control. Change if you wannt more servos

// Pins
const byte PINSERVO[NUMBEROFSERVOS] = {26, 27}; // Servo pin numbers

// other constants
const int LOOPTIMESERVOMS = 100; // Loop time for controlling servos. Make it 10 ms for smoother control, but then remove the Serial.print statements to prevent overruns

// Create all objects
Timer myServoTimer(LOOPTIMESERVOMS); // Timer object for the clock
SmoothServo myServo[NUMBEROFSERVOS];
extern FRPPMReceiver myPPMReceiver;

const byte LANDINGGEARCHANNEL = 5;

// States for starting and stopping the logger
// States for landing gear
triStateSwitch landingGearSwitchState;
triStateSwitch landingGearSwitchStatePrev;

// Servo parameters: number, max speed, max values;
const byte SERVOLANDINGGEAR = 0; // the servo number of the landing gear
const byte SERVOLANDINGHATCH = 1;

const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 60}; // Maximum speed of the servos in degrees per sec
const byte SERVOSTARTPOS[NUMBEROFSERVOS] = {130, 27};   // The starting position of the servos, type in your values here
const byte SERVOENDPOS[NUMBEROFSERVOS] = {13, 176};     // The end position of the servos, type in your values here

void HandleMainLandingGearSwitch(triStateSwitch _SwitchState, triStateSwitch _SwitchStatePrev, SmoothServo &_ServoGear, SmoothServo &_ServoHatch)
{
    // landingGearSwitchState can have the state LOSTATE (-1), MIDSTATE (0), or HISTATE (1)
    // the function GetChannelTriState returns one of these three states depending on the value of the channel

    if (_SwitchState < _SwitchStatePrev)
    {
        // towards rectracted state
        if (_SwitchState == MIDSTATE)
        {
            // So it was HISTATE, now pull in the gear
            _ServoGear.SetTargetEnd();
            Serial.println("Retract gear");
        }
        else
        {
            // So it was in MIDSTATE, now close the hatch
            _ServoHatch.SetTargetEnd();
            Serial.println("Close hatch");
        }
    }
    if (_SwitchState > _SwitchStatePrev)
    {
        // towards extended state
        if (_SwitchState == MIDSTATE)
        {
            // So it was LOSTATE, now open the hatch
            _ServoHatch.SetTargetStart();
            Serial.println("Open hatch");
        }
        else
        {
            // So it was in MIDSTATE, now extend the gear
            _ServoGear.SetTargetStart();
            Serial.println("Extend gear");
        }
    }
}

void servoSetup()
{
    Serial.println(NUMBEROFSERVOS);

    for (int i = 0; i < NUMBEROFSERVOS; i++)
    {
        float maxStep = MAXSERVOSPEEDDEGS[i] * LOOPTIMESERVOMS / 1000.0;
        myServo[i].Init(PINSERVO[i], maxStep, SERVOSTARTPOS[i], SERVOENDPOS[i]);
    }

    myServoTimer.Start();
    Serial.println("End of Setup");
}

void servoLoop()
{
    // Read switch SWC (high/mid/low) for landing gear.
    landingGearSwitchState = myPPMReceiver.GetChannelTriState(LANDINGGEARCHANNEL);
    // Check if landing gear and hatch need to move
    HandleMainLandingGearSwitch(landingGearSwitchState, landingGearSwitchStatePrev, myServo[SERVOLANDINGGEAR], myServo[SERVOLANDINGHATCH]);
    landingGearSwitchStatePrev = landingGearSwitchState; // update the previous state for the next loop

    // Update the smooth servos. There are some debugging print statements in the Update function you want to remove to speed up the code.
    for (int i = 0; i < NUMBEROFSERVOS; i++)
    {
        myServo[i].Update();
    }
    Serial.println(); // Remove this print statement is you remove the prints in the Servo update

    // At the end of the loop, WaitUntilEnd runs until the time until looptime has passed
    if (myServoTimer.WaitUntilEnd())
    {
        Serial.println("Overrun!");
    }

    delay(100);
}

#endif