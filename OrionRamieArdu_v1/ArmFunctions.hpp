#ifndef __ARMFUNCTIONS_HPP__
#define __ARMFUNCTIONS_HPP__
#include <ArduinoJson.h>
#include <DualMC33926MotorShield.h>
#include <Servo.h>

class Motor {
  public:
  enum MotorNames {
    ROTATION_DC,
    BASEDOWN_ACT,
    BASEMID_ACT,
    BASEUP_DC,
    GRASPER_DC,
    UP_SERVO,
    LEFT_SERVO,
    RIGHT_SERVO,
    DOWN_SERVO
  };

  enum MotorTypes {
    DC,
    SERVO,
    ACTUATOR
  };

  MotorTypes type;
  MotorNames name;
  int pwm;
  int direction;
  int feedback;

  Motor(MotorNames m_name, MotorTypes m_type, int pwmPin, int directionPin = -1, int feedbackPin = -1)
  {
    name = m_name;
    type = m_type;
    pwm = pwmPin;
    direction = directionPin;
    feedback = feedbackPin;
  }
};

class OrionArm {
  public:  
  struct MotorInfo {
    Motor motorName;
    int position;
  };
  
  void moveMotor(MotorInfo motor)
  {
    
  }
};

#endif
