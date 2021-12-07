#ifndef OutputCV_h
#define OutputCV_h

#include "PhysicalOutput.h"

class OutputCV : public PhysicalOutput
{

public:
//  using APhysicalIO::APhysicalIO;
  OutputCV(int index, String name);

//  void setTarget(float target) override;

protected:
  String type = "OutputCV";
};

inline OutputCV::OutputCV(int index, String name):PhysicalOutput{index, name}{
  IO::registerOutput(this);
}

//inline void OutputCV::setTarget(float target){
//  if(this->midiControlNumber > -1){
//    this->target = (target * map((float)this->midiValue,0,4095,0,1));
//  }else{
//    this->target = target;
//  }
//}

#define OutputCV MotherboardNamespace::OutputCV
#endif
