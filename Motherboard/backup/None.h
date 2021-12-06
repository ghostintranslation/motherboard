#ifndef None_h
#define None_h

#include "Input.h"
#include "Output.h"

class None : public Input, public Output
{
public:
  bool isDirectToTeensy() override {Serial.println("None");return false;}
  void read() override;

  String getType() override;
};

inline void None::read()
{
}

inline String None::getType()
{
  return "None";
}

// From now on "None" will be replaced by "new None()"
#define None new MotherboardNamespace::None()

#endif