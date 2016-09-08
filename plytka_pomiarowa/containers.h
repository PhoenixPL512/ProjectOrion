#pragma once
#include <Servo.h>
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#define CONTAINER_SLEEP 700

class Container {
  private:
    int pin;
    Servo servo;
  public:
    Container(int pin);
    void close();
    void open();
    void setState(int state);
};

