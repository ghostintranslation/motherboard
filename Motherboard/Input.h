#ifndef Input_h
#define Input_h

#include "IO.h"

class Input : public IO
{
public:
    // Depending on the type of input, the way to read it will difer
    virtual void read() = 0;

    // Depending on the type of input, it will need to be connected to ground or not
    virtual bool needsGround() = 0;
    
    void setPin(byte pin);

    
    void setAnalogMinValue(unsigned int analogMinValue);
    
    void setAnalogMaxValue(unsigned int analogMaxValue);

    // All inputs have a change callback, but not all inputs have trigger nor quantized callbacks 
    void setOnChange(ChangeCallback changeCallback);

    void setOnGateOpen(EdgeCallback gateOpenCallback);//onGateOn ?

    void setOnGateClose(EdgeCallback gateCloseCallback);//onGateOff ?

    // On value change callback
    void onValueChange() override;

    static void onMidiControlChange(byte channel, byte controlNumber, byte value);
    void setMidiControlChange(int controlNumber);
    int getMidiControlNumber();
    void setMidiTarget(byte channel, byte controlNumber, byte value);

protected:
    // The pin on which the input can be read
    byte pin;

    bool isGateOpen = false;

    unsigned int analogMaxValue = 4095;
    
    unsigned int analogMinValue = 0;

    // Change callback function
    ChangeCallback changeCallback;

    EdgeCallback gateOpenCallback;

    EdgeCallback gateCloseCallback;
    
    elapsedMillis debounceTime;
    unsigned int debounceDelay = 100;

    int midiControlNumber = -1;;
};

/**
 * On value change call the callback.
 * This function is called in the IO class.
 */
inline void Input::onValueChange()
{
  if (this->changeCallback != nullptr)
  {
    this->changeCallback(this->index, this->value, this->value - this->previousValue);
  }

  // Debouncing
  if(this->debounceTime < this->debounceDelay){
    return;
  }

  if((this->value > 4095 / 1.5) && !this->isGateOpen){
    if (this->gateOpenCallback != nullptr){
      this->gateOpenCallback(this->index);
    }
    this->isGateOpen = true;
    this->debounceTime = 0;
  }else if((this->value < 4095 / 4) && this->isGateOpen){
    if (this->gateCloseCallback != nullptr){
      this->gateCloseCallback(this->index);
    }
    this->isGateOpen = false;
    this->debounceTime = 0;
  }
}


inline void Input::setAnalogMinValue(unsigned int analogMinValue){
  this->analogMinValue = analogMinValue;
}
    
inline void Input::setAnalogMaxValue(unsigned int analogMaxValue){
  this->analogMaxValue = analogMaxValue;
}
    
/**
 * Set the callback function to call when the value changes
 */
inline void Input::setOnChange(ChangeCallback changeCallback)
{
    this->changeCallback = changeCallback;
}

/**
 * 
 */
inline void Input::setOnGateOpen(EdgeCallback gateOpenCallback)
{
    this->gateOpenCallback = gateOpenCallback;
}

/**
 * 
 */
inline void Input::setOnGateClose(EdgeCallback gateCloseCallback)
{
    this->gateCloseCallback = gateCloseCallback;
}

inline void Input::setPin(byte pin)
{
    this->pin = pin;
}


inline static void Input::onMidiControlChange(byte channel, byte controlNumber, byte value){
//    int target = map(value, 0,127, 0, 4095);
//    setTarget(target);
}

// TODO: Something like that
// We should be able to have multiple callbacks on one control number
// We should have a function to specify the range too, so the setTarget is set with a mapped value
inline void Input::setMidiControlChange(int controlNumber){
  this->midiControlNumber = controlNumber;
}

inline int Input::getMidiControlNumber(){
  return this->midiControlNumber;
}

inline void Input::setMidiTarget(byte channel, byte controlNumber, byte value){
  
}
#endif
