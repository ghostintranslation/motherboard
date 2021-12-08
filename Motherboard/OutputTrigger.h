#ifndef OutputTrigger_h
#define OutputTrigger_h

#include "PhysicalOutput.h"

class OutputTrigger : public PhysicalOutput
{

public:
  OutputTrigger(int index, String name);

  void setValue(float value) override;
  void trigger();
  void setTarget(float target) override;
  void onMidiCC(unsigned int value);
  void update() override;

private:
  elapsedMillis triggerClock;
  bool hasTriggered = false;
  
protected:
  String type = "OutputTrigger";
};

inline OutputTrigger::OutputTrigger(int index, String name):PhysicalOutput{index, name}{
  IO::registerOutput(this);
}

inline void OutputTrigger::setValue(float value){
  float newVal = 0;

  if(value > 4096/2){
    newVal = 4095;
  }else{
    newVal = 0;
  }
  
  PhysicalOutput::setValue(newVal);
}

inline void OutputTrigger::setTarget(float target){
  if(target > 4096/2 && !this->hasTriggered){
    this->trigger();
  }else if(target < 4095/2 && this->hasTriggered){
    this->hasTriggered = false;
  }
}

inline void OutputTrigger::onMidiCC(unsigned int value){
  this->setTarget(value);
}

inline void OutputTrigger::trigger(){
  this->triggerClock = 0;
  this->hasTriggered = true;
}

inline void OutputTrigger::update(){
  this->smoothing = 1; // It's a trigger, there is only 2 states, no in between.

  if(this->hasTriggered && this->triggerClock < 50){
    this->target = 4095;
  }else{
    this->target = 0;
  }
  
  PhysicalOutput::update();
}

#define OutputTrigger MotherboardNamespace::OutputTrigger
#endif
