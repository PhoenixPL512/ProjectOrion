#include "ArmFunctions.hpp"

Motor::Motor(MotorNames m_name, MotorTypes m_type, int pwmPin, int directionPin, int feedbackPin)
{
  if (m_type == MotorTypes::SERVO) {
    servo.attach(pwmPin);
  } else if (m_type == MotorTypes::DC || m_type == MotorTypes::ACTUATOR) {
    
  }
    
  name = m_name;
  type = m_type;
  pwm = pwmPin;
  direction = directionPin;
  feedback = feedbackPin;
}

int Motor::setMotor(int value)
{
  switch (type)
  {
    case MotorTypes::DC:

    break;
    case MotorTypes::SERVO:
      
    break;
    case MotorTypes::ACTUATOR:

    break;
  }
  return 0;
}

