#include "drill.h"

Drill::Drill(int p1, int p2) : pin1(p1), pin2(p2) {
  //empty ctor w/ init list
}

void Drill::setState(int state) {
  switch (state) {
    case 1:
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      break;
    case 2:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      break;
    case 3:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      break;  
  }
}

