#include "ArmFunctions.hpp"

Motor testMotor;

void setup()
{
	testMotor.configure(Motor::MotorNames::ROTATION_DC, Motor::MotorTypes::DC, 2, 22, A10, 30, 31);
}

void loop()
{
	for (int i = -400; i <= 400; i+= 10)
 {
    testMotor.setMotor(i);
    delay(100);
 }
  for (int i = 400; i >= -400; i-= 10)
  {
    testMotor.setMotor(i);
    delay(100);
  }
}
