#ifndef Aled_h
#define Aled_h

#include "APhysicalOutput.h"

class ALed : public APhysicalOutput
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

//  using APhysicalOutput::APhysicalOutput;
  
  ALed(int index);

  String getType() override;

  void print() override;

  void update(unsigned int updateMillis);
  void set(Status status, int brightness);

private:
  Status status = Off;

  // The value target requested
  unsigned int requestedTarget = 0;

  // Time counter for the blinking
  elapsedMillis blinkTime;
};

inline ALed::ALed(int index):APhysicalOutput{index, "ALed"}{
  this->index = index;
}

inline void ALed::set(Status status, int brightness)
{
  this->status = status;
  this->requestedTarget = brightness;

  switch (this->status)
  {
  case Off:
    this->requestedTarget = 0;
    break;
  case BlinkOnce:
    this->blinkTime = 0;
    break;
  default:
    break;
  }
}

inline void ALed::update(unsigned int updateMillis)
{
  APhysicalOutput::update();

  switch (this->status)
  {
  case Blink:
    if (this->blinkTime % 400 < 200)
    {
      this->target = 0;
    }
    else
    {
      this->target = this->requestedTarget;
    }
    break;
  case BlinkFast:
    if (this->blinkTime % 200 < 100)
    {
      this->target = 0;
    }
    else
    {
      this->target = this->requestedTarget;
    }
    break;
  case BlinkOnce:
    if (this->blinkTime > 100)
    {
      this->target = 0;
    }else{
      this->target = this->requestedTarget;
    }
    break;

  default:
      this->target = this->requestedTarget;
    break;
  }
}

inline String ALed::getType()
{
  return "ALed";
}

inline void ALed::print()
{
  Serial.printf("%07.2f", this->value);
}

#define ALed MotherboardNamespace::ALed
#endif
