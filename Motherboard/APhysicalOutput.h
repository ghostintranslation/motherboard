#ifndef APhysicalOutput_h
#define APhysicalOutput_h

#include "APhysicalIO.h"

class APhysicalOutput : public APhysicalIO
{
public:
    //    virtual void write(); // TODO: NEED?
    APhysicalOutput(int index, String name);

    void update() override;

    String getType() override;
};

inline APhysicalOutput::APhysicalOutput(int index, String name):APhysicalIO{index, name}
{
  if(name == "ALed"){
    AIO::registerLed((ALed*)this);
  }else{
    AIO::registerOutput(this);
  }
}

inline void APhysicalOutput::update(){
  AIO::update();

  this->value = constrain(this->value, 0, 4095);
}

inline String APhysicalOutput::getType()
{
  return "APhysicalOutput";
}
//#define APhysicalOutput MotherboardNamespace::APhysicalOutput
#endif
