#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class Drill {
  private:
    int pin1, pin2;
  public:
    Drill(int p1, int p2);
    void setState(int state);
};

