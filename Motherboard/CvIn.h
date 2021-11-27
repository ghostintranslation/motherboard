#ifndef CvIn_h
#define CvIn_h

#include "InputAnalog.h"

class CvIn : public InputAnalog
{
public:
  void read() override;
  bool needsGround() override;
  String getType() override;
  
  // On value change callback
  void onValueChange() override;

  // Takes only 2 values, 4 or 5
  void setMaxVoltage(byte maxVoltage);

  void setOnChangeQuantized(ChangeQuantizedCallback ftpr);
  
private:
  byte maxVoltage = 4;

  unsigned long readings = 0;

  elapsedMillis debounceQuantizedCallbackTime;
  unsigned int debounceQuantizedCallbackDelay = 5;
  
  // The previous quantizedvalue
  unsigned int previousValueQuantized = 0;

  ChangeQuantizedCallback changeQuantizedCallback;
};

inline void CvIn::read()
{
  pinMode(this->pin, INPUT);
  int val = analogRead(this->pin);

// TODO: METTRE AU PROPRE, supprimer this->readings
//  unsigned int bits = 4;
//  const unsigned int count = powf(bits, 4);
//  for (int n=0; n < count; n++) {
//    this->readings += analogRead(this->pin);
//  }
//  unsigned long val = this->readings / (unsigned int)powf(bits, 2);
//  val = val >> 4;
// this->readings = 0;

  switch(this->maxVoltage){
    case 5:
        val = constrain(val,0,4095);
    break;
    case 4:
    default:
        val = map(constrain(val, 0, 3440), 0, 3440, 0 , 4095);
    break;
    
  }

  
//val=val>>4;
//    unsigned long valueQuantized = map(val, 0, 4095, 0, 47);
//  Serial.println(valueQuantized);
  // TODO: ADD ANALOG MIN-MAX TO INPUT
  this->setTarget(val);
}


inline void CvIn::setOnChangeQuantized(ChangeQuantizedCallback fptr)
{
    this->changeQuantizedCallback = fptr;
}


inline void CvIn::onValueChange(){
  
  if (this->changeQuantizedCallback != nullptr)
  {
    // Debouncing
//    if(this->debounceQuantizedCallbackTime > this->debounceQuantizedCallbackDelay){
//
//      this->debounceQuantizedCallbackTime = 0;
      
      unsigned int valueQuantized = map((unsigned int)this->value, 0, 4095, 0, 47);
      
      if(valueQuantized != this->previousValueQuantized){
        this->changeQuantizedCallback(this->index, valueQuantized);
        this->previousValueQuantized = valueQuantized;
      }
//    }
  }
  Input::onValueChange(); // TODO: TESTER EN LE METTANT APRES changeQuantizedCallback
}

inline void CvIn::setMaxVoltage(byte maxVoltage){
  this->maxVoltage = maxVoltage;
}

inline bool CvIn::needsGround()
{
  return true;
}

inline String CvIn::getType()
{
  return "CvIn";
}


// From now on "CvIn" will be replaced by "new CvIn()"
#define CvIn MotherboardNamespace::CvIn

#endif
