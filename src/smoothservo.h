#ifndef SMOOTHSERVO_H
#define SMOOTHSERVO_H

//---------------------------------------------------------------------------------------------------------
// SMOOTHSERVO
// Creates a class for smooth servo control
//---------------------------------------------------------------------------------------------------------

#include <ESP32Servo.h>


class SmoothServo {
public:
  SmoothServo() {
    _Servo = new Servo();
  }

  ~SmoothServo(){
    delete _Servo;
  }

  void Init(byte pinNumber, float maxStep, byte startPos, byte endPos) {
    _pinNumber = pinNumber;
    _maxStep = maxStep;
    _targetPos = startPos;
    _actualPos = startPos;
    _startPos = startPos;
    _endPos = endPos;
    _Servo->attach(_pinNumber);
  }

  void SetTargetPos(byte targetPos) {
    _targetPos = targetPos;
  }

  void SetTargetStart(){
    _targetPos = _startPos;
  }

  void SetTargetEnd(){
    _targetPos = _endPos;
  }

  void Update() {
    float error = float(_targetPos) - _actualPos;
    error = max(-_maxStep, min(_maxStep, error));
    _actualPos += error;
    _Servo->write(int(_actualPos));
    Serial.print(int(_actualPos));
    Serial.print("; ");
  }

private:
  Servo* _Servo;
  byte _pinNumber;
  byte _targetPos;
  float _actualPos;
  float _maxStep;
  byte _startPos;
  byte _endPos;
};

//---------------------------------------------------------------------------------------------------------
// End of SMOOTHSERVO
//---------------------------------------------------------------------------------------------------------
#endif //SMOOTHSERVO_H