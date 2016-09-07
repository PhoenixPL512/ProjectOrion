#include "ArmFunctions.hpp"

//motor class

void Motor::configure(MotorNames motor_name, MotorTypes motor_type, int pwmPin, int directionPin, int feedbackPin, int nD2Pin, int nSFPin)
{
  if (motor_type == MotorTypes::SERVO) {
    servo.attach(pwmPin);
    servo.write(30);
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
  Serial.begin(115200);
  //Serial3.begin(115200);
  //armMotors[0].configure(Motor::MotorNames::ROTATION_DC, Motor::MotorTypes::DC, 2, 22, A0, 24, 25);
  //STEROWNIK #1
  armMotors[0].configure(Motor::MotorNames::BASEMID_ACT, Motor::MotorTypes::ACTUATOR, 2, 22, A0, 24, 25);
  armMotors[1].configure(Motor::MotorNames::BASEDOWN_ACT, Motor::MotorTypes::ACTUATOR, 3, 23, A1, 24, 25);
  //armMotors[2].configure(Motor::MotorNames::BASEMID_ACT, Motor::MotorTypes::ACTUATOR, 4, 26, A2, 28, 29);
  //STEROWNIK #2
  armMotors[2].configure(Motor::MotorNames::ROTATION_DC, Motor::MotorTypes::DC, 4, 26, A2, 28, 29);
  armMotors[3].configure(Motor::MotorNames::BASEUP_DC, Motor::MotorTypes::DC, 5, 27, A3, 28, 29);
  //STEROWNIK #3
  armMotors[4].configure(Motor::MotorNames::GRASPER_DC, Motor::MotorTypes::DC, 6, 30, A4, 32, 33);
  //SERWOMOTORY
  armMotors[5].configure(Motor::MotorNames::UP_SERVO, Motor::MotorTypes::SERVO, 8);
  armMotors[6].configure(Motor::MotorNames::LEFT_SERVO, Motor::MotorTypes::SERVO, 9);
  armMotors[7].configure(Motor::MotorNames::RIGHT_SERVO, Motor::MotorTypes::SERVO, 10);
  armMotors[8].configure(Motor::MotorNames::GEOMETRY_SERVO, Motor::MotorTypes::SERVO, 11);
  //Serial3.println("BOOT COMPLETE");
}


bool OrionArm::parseJSON(String json, MotorInfo info[])
{
  StaticJsonBuffer<256> jsonBuffer;
  //Serial3.print("parseJSON: ");
  //Serial3.println(json);
  JsonObject& root = jsonBuffer.parseObject(json);
  //Serial.print("Size of json: ");
  //Serial.println(root.size());
  //Serial3.print("Success? ");
  //Serial3.println(root.success());
  if (!root.success()) { return false; }
  for (int i = 0; i < NUMBER_OF_ENGINES; i++)
  {
    //Serial.print("[");
    //Serial.print(motorNamesStr[i]);
    //Serial.print("] - ");
    //Serial.println((long)root[motorNamesStr[i]]);
    info[i].motorNumber = i;
    info[i].position = root.get<int>(motorNamesStr[i]);
  }
  return true;
}

inline float OrionArm::getVoltage()
{
  float readValue = static_cast<float>(analogRead(A5));
  float vIn = readValue * (3.3/4095.0);
  return (9.47943284 * vIn) + 0.86341353;
}

void OrionArm::sendFeedback()
{
  StaticJsonBuffer<256> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["EUAF"] = armMotors[0].getFeedback();
  root["SLAF"] = armMotors[1].getFeedback();
  root["TRTF"] = armMotors[2].getFeedback();
  root["WUDF"] = armMotors[3].getFeedback();
  root["WRNF"] = armMotors[4].getFeedback();
  root["VOUT"] = getVoltage();
  root.printTo(Serial);
  Serial.print("\r\n");
}

inline String OrionArm::readJSON()
{
  //String ret;
  if (Serial.available() > 0)
  {
    //ret = Serial.readStringUntil('\n');
    //Serial3.print("readJSON: ");
    //Serial3.println(ret);
    //return ret;
    sendFeedback();
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
    if (!parseJSON(jsonBuf, info)) continue;
    
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



