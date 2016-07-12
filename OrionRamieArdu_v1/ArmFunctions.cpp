#include "ArmFunctions.hpp"

Motor::Motor(MotorNames m_name, MotorTypes m_type, int pwmPin, int directionPin, int feedbackPin)
{
    name = m_name;
    type = m_type;
    pwm = pwmPin;
    direction = directionPin;
    feedback = feedbackPin;
  }
