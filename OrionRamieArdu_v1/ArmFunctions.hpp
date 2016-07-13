#ifndef __ARMFUNCTIONS_HPP__
#define __ARMFUNCTIONS_HPP__
#include <ArduinoJson.h>
#include "MotorDriver.hpp"
#include <Servo.h>

class Motor {
  public:
  enum class MotorNames {
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

  enum class MotorTypes {
    DC,
    SERVO,
    ACTUATOR
  };
  
  private:
  MotorTypes type;
  MotorNames name;
  Servo servo;
  MC33926MotorDriver motorDriver;  

  public:
  //motor/driver functions
  Motor(MotorNames motor_name, MotorTypes motor_type, int pwmPin, int directionPin = -1, int feedbackPin = -1, int nD2Pin = -1, int nSFPin= -1);
  int setMotor(int value);
  int getFeedback();

  //get's
  int getMotorType();
  int getMotorName();
};

class OrionArm {
  private: 
  struct MotorInfo {
    Motor motorName;
    int position;
  };

  public:
  
};

#endif
