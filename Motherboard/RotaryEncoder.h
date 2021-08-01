#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include "Input.h"

class RotaryEncoder : public Input
{
public:
  void read() override;
  bool needsGround() override;
  String getType() override;
};

inline void RotaryEncoder::read()
{
  pinMode(this->pin, INPUT_PULLUP);
  int val = analogRead(this->pin);
  val = constrain(val,0,4095);// TODO: ADD ANALOG MIN-MAX TO INPUT

  if(abs(val - this->previousValue) > 4095/2){
    if(val > 4095/1.5 && val > this->previousValue){
      this->target--;
    }else if(val < 4095/1.5 && val < this->previousValue){
      this->target++;
    }
  }

  
}

inline bool RotaryEncoder::needsGround()
{
  return false;
}

inline String RotaryEncoder::getType()
{
  return "RotaryEncoder";
}

// From now on "RotaryEncoder" will be replaced by "new RotaryEncoder()"
#define RotaryEncoder new RotaryEncoder()

#endif
