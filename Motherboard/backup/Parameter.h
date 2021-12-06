#ifndef Parameter_h
#define Parameter_h

#include "IO.h"

class Parameter : public IO
{
public:
  Parameter();
  
  String getType() override;
};

inline Parameter::Parameter(){
  
}

inline String Parameter::getType()
{
  return "Parameter";
}

inline void Parameter::read()
{
  pinMode(this->pin, INPUT);
  int val = analogRead(this->pin);
//  Serial.println(val);
  val = map(constrain(val, 331, 4095), 331, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT

  this->setTarget(val);
}

// From now on "Parameter" will be replaced by "new Parameter()"
#define Parameter MotherboardNamespace::Parameter

#endif
