#ifndef __ARMFUNCTIONS_HPP__
#define __ARMFUNCTIONS_HPP__
#include <ArduinoJson.h>
#include "MotorDriver.hpp"
#include <Servo.h>
#define NUMBER_OF_ENGINES 9

class Motor {
  public:
  enum class MotorNames {
    ROTATION_DC = 0,
    BASEDOWN_ACT,
    BASEMID_ACT,
    BASEUP_DC,
    GRASPER_DC,
    UP_SERVO,
    LEFT_SERVO,
    RIGHT_SERVO,
    GEOMETRY_SERVO
  };

  enum class MotorTypes {
    DC = 0,
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
  void configure(MotorNames motor_name, MotorTypes motor_type, int pwmPin, int directionPin = -1, int feedbackPin = -1, int nD2Pin = -1, int nSFPin= -1);
  int setMotor(int value);
  int getFeedback();

  //get's
  MotorTypes getMotorType();
  MotorNames getMotorName();
};

class OrionArm {
  private: 
  struct MotorInfo {
    int motorNumber;
    int position;
  };
  
  Motor armMotors[9];
  //const String motorNamesStr[9] = {"RotationArmMotor ZAMIENIONY", "BaseDownArmMotor", "BaseMidArmMotor ZAMIENIONY", "BaseUpArmMotor", "GrasperRotationArmMotor", "UpArmServo", "LeftArmServo", "RightArmServo", "GeometryArmServo" };
  const String motorNamesStr[9] = {"EUA", "SLA", "TRT", "WUD", "WRN", "GGGT", "GGGL", "GGGR", "GGY"};
  
  bool parseJSON(String json, MotorInfo info[]);
  inline String readJSON();
  inline void writeJSON(String json);
  void armEvents();
  void sendFeedback();
  inline float getVoltage();
  
  public:
  void init();
  void enableArmControl();
  void disableArmControl();
  void demo();
};

#endif
