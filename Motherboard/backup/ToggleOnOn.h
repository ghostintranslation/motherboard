#ifndef ToggleOnOn_h
#define ToggleOnOn_h

#include "Input.h"

class ToggleOnOn : public Input
{
public:
  bool isDirectToTeensy() override {Serial.println("ToggleOnOn");return false;}
  void read() override;
  
  String getType() override;
};

inline void ToggleOnOn::read()
{
  pinMode(this->pin, INPUT);
  int val = analogRead(this->pin);
  val = map(constrain(val,8,4095), 8, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT
  if(val <= 10){
    val = 1;
  }else{
    val = 0;
  }
  this->setTarget(val);
}

inline String ToggleOnOn::getType()
{
  return "ToggleOnOn";
}

// From now on "ToggleOnOn" will be replaced by "new ToggleOnOn()"
#define ToggleOnOn new MotherboardNamespace::ToggleOnOn()

#endif
