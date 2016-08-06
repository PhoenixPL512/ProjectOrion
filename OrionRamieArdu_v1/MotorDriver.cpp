#include "MotorDriver.hpp"

MC33926MotorDriver::MC33926MotorDriver()
{
  //empty ctor
}

void MC33926MotorDriver::setPins(int _directory, int _pwm, int _feedback, int _nD2, int _nSF, bool init)
{
  directory = _directory;
  pwm = _pwm;
  feedback = _feedback;
  nD2 = _nD2;
  nSF = _nSF;
  pinsSet = true;
  if (init) this->init();
}

void MC33926MotorDriver::init()
{
  if (!pinsSet) return;
  pinMode(directory, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(feedback, INPUT);
  pinMode(nD2, OUTPUT);
  digitalWrite(nD2, HIGH);
  pinMode(nSF, INPUT);
}

void MC33926MotorDriver::setSpeed(int speed)
{
  if (!pinsSet) return;
  bool reverse = false;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = true;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM dutycycle
    speed = 400;
    
  analogWrite(pwm, speed * 51 / 80);
  if (reverse)
    digitalWrite(directory, HIGH);
  else
    digitalWrite(directory, LOW);
}

int MC33926MotorDriver::getCurrent()
{
  if (!pinsSet) return -1;
  return analogRead(feedback) * 9;
}

bool MC33926MotorDriver::getFault()
{
  if (!pinsSet) return true;
  return !digitalRead(nSF);
}




