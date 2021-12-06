#ifndef APhysicalInput_h
#define APhysicalInput_h

#include "APhysicalIO.h"

class APhysicalInput : public APhysicalIO
{
public:
    APhysicalInput(int index, String name);
    
    // Depending on the type of APhysicalInput, the way to read it will difer
    virtual void read() = 0;

    // Depending on the type of APhysicalInput, it could need to connect directly to Teensy
    // skipping the opamp and resistor divider, like the TouchPads
    virtual bool isDirectToTeensy() = 0;
    
    void setAnalogMinValue(unsigned int analogMinValue);
    
    void setAnalogMaxValue(unsigned int analogMaxValue);

    static void onMidiControlChange(byte channel, byte controlNumber, byte value);
    void setMidiControlChange(int controlNumber);
    int getMidiControlNumber();
    void setMidiTarget(byte channel, byte controlNumber, byte value);

protected:
    // The pin on which the APhysicalInput can be read
    byte pin = 0;

    unsigned int analogMaxValue = 4095;
    
    unsigned int analogMinValue = 0;

    int midiControlNumber = -1;
};

inline APhysicalInput::APhysicalInput(int index, String name):APhysicalIO{index, name}
{
  this->pin = ANALOG_IN_1_PIN; // TODO: DO IT ACCORDING TO INDEX
  AIO::registerInput(this);
}


inline void APhysicalInput::setAnalogMinValue(unsigned int analogMinValue){
  this->analogMinValue = analogMinValue;
}
    
inline void APhysicalInput::setAnalogMaxValue(unsigned int analogMaxValue){
  this->analogMaxValue = analogMaxValue;
}

inline void APhysicalInput::onMidiControlChange(byte channel, byte controlNumber, byte value){
//    int target = map(value, 0,127, 0, 4095);
//    setTarget(target);
}

// TODO: Something like that
// We should be able to have multiple callbacks on one control number
// We should have a function to specify the range too, so the setTarget is set with a mapped value
inline void APhysicalInput::setMidiControlChange(int controlNumber){
  this->midiControlNumber = controlNumber;
}

inline int APhysicalInput::getMidiControlNumber(){
  return this->midiControlNumber;
}

inline void APhysicalInput::setMidiTarget(byte channel, byte controlNumber, byte value){
  
}

//#define APhysicalInput MotherboardNamespace::APhysicalInput
#endif
