#ifndef InputAnalog_h
#define InputAnalog_h

#include "Input.h"

class InputAnalog : public Input
{
public:
    void read() override;

    String getType() override;
    
    // On value change callback
    void onValueChange() override;
    
    void setOnChangeInRange(unsigned int rangeLow, unsigned int rangeHigh, ChangeCallback ftpr);
    void setOnChangeQuantized(ChangeQuantizedCallback ftpr);

private:

    // The previous quantizedvalue
    unsigned int previousValueQuantized = 0;
    
    // Callbacks functions
    ChangeQuantizedCallback changeQuantizedCallback;
};

inline void InputAnalog::read()
{
  pinMode(this->pin, INPUT);
  int val = analogRead(this->pin);
//  val = map(constrain(val, 160, 4095), 160, 4095, 0, 4095); // TODO: ADD ANALOG MIN-MAX TO INPUT

  this->setTarget(val);
}

inline void InputAnalog::setOnChangeInRange(unsigned int rangeLow, unsigned int rangeHigh, ChangeCallback ftpr){
  this->changeCallback = ftpr;
  // TODO: Trigger the callback only between the range
}

/**
 * Set the press down callback
 */
inline void InputAnalog::setOnChangeQuantized(ChangeQuantizedCallback fptr)
{
    this->changeQuantizedCallback = fptr;
}

inline String InputAnalog::getType()
{
    return "InputAnalog";
}

inline void InputAnalog::onValueChange(){
  Input::onValueChange();
  
  if (this->changeQuantizedCallback != nullptr)
  {
    unsigned int valueQuantized = map((unsigned int)this->value, 0, 4095, 0, 60);
    
    if(valueQuantized != this->previousValueQuantized){
      this->changeQuantizedCallback(this->index, valueQuantized);
      this->previousValueQuantized = previousValueQuantized;
    }
  }

  
// 0-5v = 0-4095
// 1v = 819
// 5 octaves = 60 notes
// step = 819/12

    // Midi note 60 = 4 095
    
    // Midi note 59 = 4 026,75
    // Midi note 58 = 3 958,5
    // Midi note 57 = 3 890,25
    // Midi note 56 = 3 822
    // Midi note 55 = 3 753,75
    // Midi note 54 = 3 685,5
    // Midi note 53 = 3 617,25
    // Midi note 52 = 3 549
    // Midi note 51 = 3 480,75
    // Midi note 50 = 3 412,5
    // Midi note 49 = 3 344,25
    // Midi note 48 = 3 276
    
    // Midi note 47 = 3 207,75
    // Midi note 46 = 3 139,5
    // Midi note 45 = 3 071,25
    // Midi note 44 = 3 003
    // Midi note 43 = 2 934,75
    // Midi note 42 = 2 866,5
    // Midi note 41 = 2 798,25
    // Midi note 40 = 2 730
    // Midi note 39 = 2 661,75
    // Midi note 38 = 2 593,5
    // Midi note 37 = 2 525,25
    // Midi note 36 = 2 457

    // Midi note 35 = 2 388,75
    // Midi note 34 = 2 320,5
    // Midi note 33 = 2 252,25
    // Midi note 32 = 2 184
    // Midi note 31 = 2 115,75
    // Midi note 30 = 2 047,5
    // Midi note 29 = 1 979,25
    // Midi note 28 = 1 911
    // Midi note 27 = 1 842,75
    // Midi note 26 = 1 774,5
    // Midi note 25 = 1 706,25
    // Midi note 24 = 1 638
    
    // Midi note 23 = 1 569,75
    // Midi note 22 = 1 501,5
    // Midi note 21 = 1 433,25
    // Midi note 20 = 1 365
    // Midi note 19 = 1 296,75
    // Midi note 18 = 1 228,5
    // Midi note 17 = 1 160,25
    // Midi note 16 = 1 092
    // Midi note 15 = 1 023,75
    // Midi note 14 = 955,5
    // Midi note 13 = 887,25
    // Midi note 12 = 819
    
    // Midi note 11 = 750,75
    // Midi note 10 = 682,5
    // Midi note 9 = 614,25
    // Midi note 8 = 546
    // Midi note 7 = 477,75
    // Midi note 6 = 409,5
    // Midi note 5 = 341,25
    // Midi note 4 = 273
    // Midi note 3 = 204,75
    // Midi note 2 = 136,5
    // Midi note 1 = 68,25
    // Midi note 0 = 0
}
#endif
