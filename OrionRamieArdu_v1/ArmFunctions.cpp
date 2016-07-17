#include "ArmFunctions.hpp"

//motor class


void Motor::configure(MotorNames motor_name, MotorTypes motor_type, int pwmPin, int directionPin, int feedbackPin, int nD2Pin, int nSFPin)
{
  if (motor_type == MotorTypes::SERVO) {
    servo.attach(pwmPin);
  } else if (motor_type == MotorTypes::DC || motor_type == MotorTypes::ACTUATOR) {
    motorDriver.setPins(directionPin, pwmPin, feedbackPin, nD2Pin, nSFPin);
    motorDriver.init();
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
    if (motorDriver.getFault()) return -1; else return motorDriver.getCurrent(); //if driver error (current overflow or something), -1. Else, current.
  else
    return servo.read(); //angle
}

Motor::MotorTypes Motor::getMotorType()
{
  return type;
}

Motor::MotorNames Motor::getMotorName()
{
  return name;
}

//Arm class

void OrionArm::init()
{
  Serial.begin(14400);
  armMotors[0].configure(Motor::MotorNames::ROTATION_DC, Motor::MotorTypes::DC, 2, 22, A0, 24, 25);
  armMotors[1].configure(Motor::MotorNames::BASEDOWN_ACT, Motor::MotorTypes::ACTUATOR, 3, 23, A1, 24, 25);
  armMotors[2].configure(Motor::MotorNames::BASEMID_ACT, Motor::MotorTypes::ACTUATOR, 4, 26, A2, 28, 29);
  armMotors[3].configure(Motor::MotorNames::BASEUP_DC, Motor::MotorTypes::DC, 5, 27, A3, 28, 29);
  armMotors[4].configure(Motor::MotorNames::GRASPER_DC, Motor::MotorTypes::DC, 6, 30, A4, 32, 33);
  armMotors[5].configure(Motor::MotorNames::UP_SERVO, Motor::MotorTypes::SERVO, 8);
  armMotors[6].configure(Motor::MotorNames::LEFT_SERVO, Motor::MotorTypes::SERVO, 9);
  armMotors[7].configure(Motor::MotorNames::RIGHT_SERVO, Motor::MotorTypes::SERVO, 10);
  armMotors[8].configure(Motor::MotorNames::GEOMETRY_SERVO, Motor::MotorTypes::SERVO, 11);
}

void OrionArm::parseJSON(String json, MotorInfo info[])
{
  JsonObject& root = jsonBuffer.parseObject(json);
  for (int i = 0; i < 9; i++)
  {
    info[i].motorNumber = i;
    info[i].position = root[motorNamesStr[i]];
  }
}

inline String OrionArm::readJSON()
{
  if (Serial.available() > 0)
    return Serial.readStringUntil('\r');
  else return "";
}

inline void OrionArm::writeJSON(String json)
{
  if (Serial) Serial.println(json);
}

void OrionArm::enableArmControl()
{
  String jsonBuf;
  MotorInfo info[9];
  while(true)
  {
    jsonBuf = readJSON();
    if (jsonBuf == "") continue;
    parseJSON(jsonBuf, info);
    for (int i = 0; i < 9; i++)
    {
      armMotors[info[i].motorNumber].setMotor(info[i].position);
    }
  }
}
