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
  unsigned int reading = analogRead(this->pin);
  
  unsigned int  val = 0.9*previousReading + 0.1*reading; // low pass filter

  unsigned int newval = map(constrain(val, 35, 4086), 35, 4086, 0, 4095);

  if(this->midiControlNumber > -1){
    this->setTarget(newval * map((float)this->midiValue,0,4095,0,1));
  }else{
    this->setTarget(newval);
  }
  
  this->previousReading = newval;
}

// Prevents to have to write the namespace when using this class
#define InputPotentiometer MotherboardNamespace::InputPotentiometer

#endif
