#include "ArmFunctions.hpp"

Motor::Motor(MotorNames motor_name, MotorTypes motor_type, int pwmPin, int directionPin, int feedbackPin, int nD2Pin, int nSFPin)
{
  if (motor_type == MotorTypes::SERVO) {
    servo.attach(pwmPin);
  } else if (motor_type == MotorTypes::DC || motor_type == MotorTypes::ACTUATOR) {
    motorDriver.setPins(directionPin, pwmPin, feedbackPin, nD2Pin, nSFPin);
  }

  type = motor_type;
  name = motor_name;
}

int Motor::setMotor(int value)
{
  switch (type)
  {
    case MotorTypes::DC:
      if (value < -400 || value > 400) return 1; //value error
      motorDriver.setSpeed(value);
    break;
    case MotorTypes::SERVO:
      if (value < 0 || value > 180) return 1; //value error
      servo.write(value);
    break;
    case MotorTypes::ACTUATOR:
      switch (value)
      {
      case -1:
        motorDriver.setSpeed(-400);
        break;
      case 0:
        motorDriver.setSpeed(0);
        break;
      case 1:
        motorDriver.setSpeed(400);
        break;
      default:
        return 1; //value error
      }
    break;
  }
  return 0; //no error
}

int Motor::getFeedback()
{
  if (type == MotorTypes::DC || type == MotorTypes::ACTUATOR)
    if (motorDriver.getFault()) return -1; else return motorDriver.getCurrent(); //if driver error (current overflow), -1. Else, current.
  else
    return servo.read(); //angle
}

int Motor::getMotorType()
{
  return static_cast<int>(type);
}

int Motor::getMotorName()
{
  return static_cast<int>(name);
}

