#ifndef ToggleOnOffOn_h
#define ToggleOnOffOn_h

#include "Input.h"

class ToggleOnOffOn : public Input
{
public:
  void read() override;

  String getType() override;
};

inline void ToggleOnOffOn::read()
{
  pinMode(this->pin, INPUT);
  int val = analogRead(this->pin);
  val = map(constrain(val,8,4095), 8, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT
  if(val <= 1000){
    val = 0;
  }else if(val > 2200){
    val = 1;
  }else{
    val=-1;
  }
  this->setTarget(val);
}

inline String ToggleOnOffOn::getType()
{
  return "ToggleOnOffOn";
}

// From now on "ToggleOnOffOn" will be replaced by "new ToggleOnOffOn()"
#define ToggleOnOffOn MotherboardNamespace::ToggleOnOffOn

#endif
