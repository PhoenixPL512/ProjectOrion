#include <ArduinoJson.h>
#include "containers.h"
#include "drill.h"
#include "HX711.h"
#define SKALA -1.52

Container* containers[3];
Drill* drill;
HX711* scales[2];
String data;

inline String getJsonString() {
  if (Serial.available()) {
    return Serial.readStringUntil('\n');
  }
}

inline void closeAllContainers() {
  for(int i = 0; i < 3; ++i)
    containers[i]->close();
}

inline void initScales()
{
  scales[0]->set_scale(2280.f);
  scales[0]->tare();
  scales[1]->set_scale(2280.f);
  scales[1]->tare();
}

void printFeedback() {
  float final_weigth = (scales[0]->get_units(2) + scales[1]->get_units(2)) * SKALA;
  int temperatures[3] = { analogRead(A0), analogRead(A1), analogRead(A2) };
  float x[3];

  for(int i = 0; i < 3; ++i)
    x[i] = (temperatures[i]/1024.0)*500;

  StaticJsonBuffer<256> buf;
  JsonObject& obj = buf.createObject();

  obj["MASS"] = final_weigth;
  obj["TEMP1"] = x[0];
  obj["TEMP2"] = x[1];
  obj["TEMP3"] = x[2];
  
  obj.printTo(Serial);
  Serial.print("\r\n");
}

void setup() {
  pinMode(10,OUTPUT); //drillup
  pinMode(11,OUTPUT); //drilldown
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);

  Serial.begin(115200);
  containers[0] = new Container(5);
  containers[1] = new Container(6);
  containers[2] = new Container(9);
  drill = new Drill(10, 11);
  scales[0] = new HX711(4, 3);
  scales[1] = new HX711(8, 7);

  initScales();
  closeAllContainers();
  
}

void loop() {
  data = getJsonString();
  if (data.length()) {
    printFeedback();
    StaticJsonBuffer<256> buf;
    JsonObject &root = buf.parseObject(data);
    if (root.success()) {
      containers[0]->setState(root["CNTR1"]);
      containers[1]->setState(root["CNTR2"]);
      containers[2]->setState(root["CNTR3"]);
      drill->setState(root["DRILL"]);
    }
  }
}


