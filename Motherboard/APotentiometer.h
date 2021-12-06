#ifndef APotentiometer_h
#define APotentiometer_h

#include "APhysicalInput.h"

class APotentiometer : public APhysicalInput
{
public:
  using APhysicalInput::APhysicalInput;
  
  bool isDirectToTeensy() {return false;}
    
  void read() override;

protected:
  String type = "APotentiometer";
};

inline void APotentiometer::read()
{
  pinMode(this->pin, INPUT);
  float val = analogRead(this->pin);
  val = map(constrain(val, 370, 4095), 370, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT

  this->setTarget(val);
}

// Prevents to have to write the namespace when using this class
#define APotentiometer MotherboardNamespace::APotentiometer

#endif
