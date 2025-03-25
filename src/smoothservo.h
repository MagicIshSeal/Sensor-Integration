#ifndef SMOOTHSERVO_H
#define SMOOTHSERVO_H

//---------------------------------------------------------------------------------------------------------
// SMOOTHSERVO
// Creates a class for smooth servo control
//---------------------------------------------------------------------------------------------------------

#include <ESP32Servo.h>

class SmoothServo
{
public:
  SmoothServo()
  {
    _Servo = new Servo();
  }

  ~SmoothServo()
  {
    delete _Servo;
  }

  void Init(byte pinNumber, float maxStep, byte startPos, byte endPos, int minTime, int maxTime)
  {
    _pinNumber = pinNumber;
    _maxStep = maxStep;
    _targetPos = startPos;
    _actualPos = startPos;
    _startPos = startPos;
    _endPos = endPos;
    _minTime = minTime;
    _maxTime = maxTime;
    _Servo->attach(_pinNumber, _minTime, _maxTime);
  }

  void SetTargetPos(byte targetPos)
  {
    _targetPos = targetPos;
  }

  void SetTargetStart()
  {
    _targetPos = _startPos;
  }

  void SetTargetEnd()
  {
    _targetPos = _endPos;
  }

  void Update()
  {
    float error = float(_targetPos) - _actualPos;
    error = max(-_maxStep, min(_maxStep, error));
    _actualPos += error;
    _Servo->write(int(_actualPos));
  }

  byte GetPosition() const
  {
    return static_cast<byte>(_actualPos);
  }

private:
  Servo *_Servo;
  byte _pinNumber;
  byte _targetPos;
  float _actualPos;
  float _maxStep;
  byte _startPos;
  byte _endPos;
  int _minTime;
  int _maxTime;
};

//---------------------------------------------------------------------------------------------------------
// End of SMOOTHSERVO
//---------------------------------------------------------------------------------------------------------
#endif // SMOOTHSERVO_H