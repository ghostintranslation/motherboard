#ifndef led_h
#define led_h

#include "Output.h"

class Led : public Output
{

public:
  enum Status
  {
    Off,
    On,
    Blink,
    BlinkFast,
    BlinkOnce
  };

  String getType() override;

  void update(unsigned int updateMillis);
  void setStatus(Status status);

private:
  Status status;

  unsigned int blinkTarget;

  // Time counter for the blinking
  elapsedMillis blinkTime;
};

inline void Led::setStatus(Status status)
{
  this->status = status;
  this->blinkTarget = this->target;

  switch (this->status)
  {
  case Off:
    this->target = 0;
    break;
  case BlinkOnce:
    this->blinkTime = 0;
    break;
  default:
    break;
  }
}

inline void Led::update(unsigned int updateMillis)
{
  Output::update(updateMillis);

  switch (this->status)
  {
  case Blink:
    if (this->blinkTime % 400 < 200)
    {
      this->target = 0;
    }
    else
    {
      this->target = this->blinkTarget;
    }
    break;
  case BlinkFast:
    if (this->blinkTime % 200 < 200)
    {
      this->target = 0;
    }
    else
    {
      this->target = this->blinkTarget;
    }
    break;
  case BlinkOnce:
    if (this->blinkTime > 200)
    {
      this->target = 0;
    }
    break;

  default:
    break;
  }
}

inline String Led::getType()
{
  return "Led";
}
#endif
