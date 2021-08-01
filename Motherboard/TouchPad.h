#ifndef TouchPad_h
#define TouchPad_h

#include "Input.h"
#include "FastTouch.h"

class TouchPad : public Input
{
public:
  void read() override;
  bool needsGround() override;
  String getType() override;
};

inline void TouchPad::read()
{
  pinMode(this->pin, INPUT_PULLDOWN);
  int val = fastTouchRead(this->pin);
  val = map(constrain(val, 64, 85), 64, 85, 0, 4095);// TODO: ADD ANALOG MIN-MAX TO INPUT
  this->setTarget(val);
}

inline bool TouchPad::needsGround()
{
  return false;
}

inline String TouchPad::getType()
{
  return "TouchPad";
}

// From now on "TouchPad" will be replaced by "new TouchPad()"
#define TouchPad new TouchPad()

#endif
