#ifndef SMOOTHSERVO_H
#define SMOOTHSERVO_H

#include <ESP32Servo.h>

// SmoothServo class for smooth servo control
class SmoothServo
{
public:
  // Constructor
  SmoothServo()
  {
    _Servo = new Servo();
  }

  // Destructor
  ~SmoothServo()
  {
    delete _Servo;
  }

  // Initialize servo with parameters
  void Init(byte pinNumber, float maxStep, byte startPos, byte endPos, int minTime = 500, int maxTime = 2500)
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

  // Set target position
  void SetTargetPos(byte targetPos)
  {
    _targetPos = targetPos;
  }

  // Set target to start position
  void SetTargetStart()
  {
    _targetPos = _startPos;
  }

  // Set target to end position
  void SetTargetEnd()
  {
    _targetPos = _endPos;
  }

  // Update servo position smoothly
  void Update()
  {
    float error = float(_targetPos) - _actualPos;
    error = max(-_maxStep, min(_maxStep, error));
    _actualPos += error;
    _Servo->write(int(_actualPos));
  }

  // Get current servo position
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

#endif // SMOOTHSERVO_H