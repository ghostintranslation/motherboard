#ifndef Potentiometer_h
#define Potentiometer_h

#include "InputAnalog.h"

class Potentiometer : public InputAnalog
{
public:
  void read() override;
  String getType() override;
};

inline String Potentiometer::getType()
{
  return "Potentiometer";
}

inline void Potentiometer::read()
{
  pinMode(this->pin, INPUT);
  int val = analogRead(this->pin);
  val = map(constrain(val, 331, 4095), 331, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT

  this->setTarget(val);
}

// From now on "Potentiometer" will be replaced by "new Potentiometer()"
#define Potentiometer MotherboardNamespace::Potentiometer

#endif
