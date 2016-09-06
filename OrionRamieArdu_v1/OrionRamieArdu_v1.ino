#include "ArmFunctions.hpp"

OrionArm arm;
void setup()
{
  //analogReference(DEFAULT);
  analogReadResolution(12);
  arm.init();
  arm.enableArmControl();
}

void loop()
{

}
