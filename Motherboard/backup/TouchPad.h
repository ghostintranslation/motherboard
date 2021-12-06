#ifndef TouchPad_h
#define TouchPad_h

#include "InputAnalog.h"

class TouchPad : public Input
{
public:
  void read() override;
  bool isDirectToTeensy() override {Serial.println("TouchPad");return true;}
  String getType() override;
  void onValueChange() override;
//  void setSensitivity(int sensitivity);

  void setOnTouchStart(EdgeCallback touchStartCallback);
  void setOnTouchEnd(EdgeCallback touchEndCallback);

private:
//  int sensitivity = 100;
  int lowValue = 90; //64 //70
  int highValue = 130;//85 //80

  // Is is being touched
  bool isTouching = false;

  // The threshold under which the touch is started
  int touchThreshold = 5;

  // Debounce timer
  elapsedMillis touchDebounceTime;
  
  // Debounce timer duration
  unsigned int touchDebounceDelay = 10;

  // Touch start callback
  EdgeCallback touchStartCallback;
  
  // Touch end callback
  EdgeCallback touchEndCallback;
};

inline void TouchPad::read()
{
  pinMode(this->pin, INPUT_PULLDOWN);
  int val = fastTouchRead(this->pin);
  val = map(constrain(val, this->lowValue, this->highValue), this->lowValue, this->highValue, 0, 4095);// TODO: ADD ANALOG MIN-MAX TO INPUT

  this->setTarget(val);
}

inline String TouchPad::getType()
{
  return "TouchPad";
}

//inline void TouchPad::setSensitivity(int sensitivity){
////  this->sensitivity = sensitivity;
//  this->lowValue = map(constrain(sensitivity,0,100), 0, 100, 100, 64);;
//}

inline void TouchPad::setOnTouchStart(EdgeCallback touchStartCallback){
  this->touchStartCallback = touchStartCallback;
}

inline void TouchPad::setOnTouchEnd(EdgeCallback touchEndCallback){
  this->touchEndCallback = touchEndCallback;
}

/**
 * On value change call the callback.
 * This function is called in the IO class.
 */
inline void TouchPad::onValueChange()
{
  Input::onValueChange();

  // Debouncing
//  if(this->touchDebounceTime < this->touchDebounceDelay){
//    return;
//  }
    
  if(this->target >= this->touchThreshold && this->value >= this->touchThreshold && !this->isTouching){
    if (this->touchStartCallback != nullptr)
    {
      this->touchStartCallback(this->index);
      this->touchDebounceTime = 0;
    }
    this->isTouching = true;
  }else if(this->target == 0 && this->isTouching){
    if (this->touchEndCallback != nullptr)
    {
      this->touchEndCallback(this->index);
      this->touchDebounceTime = 0;
    }
    this->isTouching = false;
  }
}
// From now on "TouchPad" will be replaced by "new TouchPad()"
#define TouchPad MotherboardNamespace::TouchPad

#endif
