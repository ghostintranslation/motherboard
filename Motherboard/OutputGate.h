#ifndef OutputGate_h
#define OutputGate_h

#include "PhysicalOutput.h"

class OutputGate : public PhysicalOutput
{

public:
//  using APhysicalIO::APhysicalIO;
  OutputGate(int index, String name);

  void setValue(float value) override;
  void setValue(bool value);
  void setTarget(float target) override;
  void onMidiCC(unsigned int value);
  void update() override;

protected:
  String type = "OutputGate";
};

inline OutputGate::OutputGate(int index, String name):PhysicalOutput{index, name}{
  IO::registerOutput(this);
}

inline void OutputGate::setValue(float value){
  float newVal = 0;
  if(value < 4096/2){
    newVal = 0;
  }else{
    newVal = 4095;
  }

  PhysicalOutput::setValue(newVal);
}

inline void OutputGate::setValue(bool value){
  if(value){
    this->setValue((float)4095);
  }else{
    this->setValue((float)0);
  }
}

inline void OutputGate::setTarget(float target){
  float newTarget = 0;
  if(target < 4096/2){
    newTarget = 0;
  }else{
    newTarget = 4095;
  }
  
  PhysicalOutput::setTarget(newTarget);
}

inline void OutputGate::onMidiCC(unsigned int value){
  unsigned int newVal = 0;
  if(value < 4096/2){
    newVal = 0;
  }else{
    newVal = 4095;
  }
  
  PhysicalOutput::onMidiCC(newVal);
}

inline void OutputGate::update(){
  this->smoothing = 1; // It's a gate, there is only 2 states, no in between.
  PhysicalOutput::update();

}

#define OutputGate MotherboardNamespace::OutputGate
#endif
