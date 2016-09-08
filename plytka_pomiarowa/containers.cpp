#include "containers.h"

Container::Container(int _pin) : pin(_pin) {
  //empty ctor w/ init list
}

void Container::close()
{
  servo.attach(pin);
  servo.write(180);
  delay(CONTAINER_SLEEP);
  servo.detach();
}

void Container::open()
{
  servo.attach(pin);
  servo.write(0);
  delay(CONTAINER_SLEEP);
  servo.detach();
}

void Container::setState(int state)
{
  switch (state) {
    case 1:
      close();
      break;
    case 2:
      open();
      break;
  }
}

