#include "ArmFunctions.hpp"

OrionArm arm;
void setup()
{
  //analogReference(DEFAULT);
  arm.init();
  arm.enableArmControl();
}

void loop()
{

}



