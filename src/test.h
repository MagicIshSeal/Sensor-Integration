#ifndef TEST_H
#define TEST_H

#include <ESP32Servo.h>
#include "smoothservo.h"

Servo gear;
Servo door;

int gear_pin = 27;
int door_pin = 26;
int pot_pin1 = 34;
int pot_pin2 = 39;

void servoSetup()
{
    gear.attach(gear_pin, 900, 2100);
    door.attach(door_pin);
}

void servoLoop()
{
    int val1 = analogRead(pot_pin2); // Read potentiometer 1
    int val2 = analogRead(pot_pin2); // Read potentiometer 2

    // Map potentiometer values (0–4095) to servo angles s(0–180)
    int angle1 = map(val1, 0, 4095, 0, 180);
    int angle2 = map(val2, 0, 4095, 0, 180);

    // Write the mapped angles to the servos
    gear.write(angle1);
    door.write(angle2);

    Serial.print("> Angle1: ");
    Serial.println(angle1);

    Serial.print("> Angle2: ");
    Serial.println(angle2);

    delay(100); // Small delay for stability
}

#endif
