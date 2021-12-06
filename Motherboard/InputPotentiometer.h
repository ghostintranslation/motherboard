#ifndef InputPotentiometer_h
#define InputPotentiometer_h

#include "PhysicalInput.h"

class InputPotentiometer : public PhysicalInput
{
public:
  using PhysicalInput::PhysicalInput;
  
  bool isDirectToTeensy() {return false;}
    
  void read() override;

protected:
  String type = "InputPotentiometer";
};

inline void InputPotentiometer::read()
{
  pinMode(this->pin, INPUT);
  float val = analogRead(this->pin);
  val = map(constrain(val, 370, 4095), 370, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT

  this->setTarget(val);
}

// Prevents to have to write the namespace when using this class
#define InputPotentiometer MotherboardNamespace::InputPotentiometer

#endif
