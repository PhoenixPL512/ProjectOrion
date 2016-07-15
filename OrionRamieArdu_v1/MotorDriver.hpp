#ifndef __MOTORDRIVER_HPP__
#define __MOTORDRIVER_HPP__
#include <Arduino.h>

class MC33926MotorDriver {
  private:
  bool pinsSet;
  int directory;
  int pwm;
  int feedback;
  int nD2;
  int nSF;
  
  public:
  MC33926MotorDriver();
  void setPins(int _directory, int _pwm, int _feedback, int _nD2, int _nSF, bool init = true);
  void init();
  void setSpeed(int speed);
  int getCurrent();
  bool getFault();
};
#endif
