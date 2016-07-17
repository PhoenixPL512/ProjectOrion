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
  Serial.begin(19200);
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
//{"RotationArmMotor":1,"BaseDownArmMotor":2,"BaseMidArmMotor":3,"BaseUpArmMotor":4,"GrasperRotationArmMotor":5,"UpArmServo":10,"RightArmServo":45,"LeftArmServo":90,"GeometryArmServo":120}
void OrionArm::parseJSON(String json, MotorInfo info[])
{
  //Serial.print("parseJSON: ");
  //Serial.println(json);
  JsonObject& root = jsonBuffer.parseObject(json);
  //Serial.print("Size of json: ");
  //Serial.println(root.size());
  //Serial.print("Success? ");
  //Serial.println(root.success());
  for (int i = 0; i < 9; i++)
  {
    //Serial.print("[");
    //Serial.print(motorNamesStr[i]);
    //Serial.print("] - ");
    //Serial.println((long)root[motorNamesStr[i]]);
    info[i].motorNumber = i;
    info[i].position = root.get<int>(motorNamesStr[i]);
  }
}

inline String OrionArm::readJSON()
{
  //String ret;
  if (Serial.available() > 0)
  {
    //ret = Serial.readStringUntil('\n');
    //Serial.print("readJSON: ");
    //Serial.println(ret);
    //return ret;
    return Serial.readStringUntil('\n');
  }
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
    /*for (int i = 0; i < 180; i++)
      { armMotors[5].setMotor(i); delay(10); }*/
    jsonBuf = readJSON();
    if (jsonBuf == "") continue;
    parseJSON(jsonBuf, info);
    for (int i = 0; i < 9; i++)
    {
      //Serial.print("Engine: ");
      //Serial.print(info[i].motorNumber);
      //Serial.print(", pos: ");
      //Serial.println(info[i].position);
      armMotors[info[i].motorNumber].setMotor(info[i].position);
    }
  }
}
