#ifndef APhysicalOutput_h
#define APhysicalOutput_h

#include "APhysicalIO.h"

class APhysicalOutput : public APhysicalIO
{
public:
    //    virtual void write(); // TODO: NEED?
    APhysicalOutput(int index, String name);

    void update() override;
};

inline APhysicalOutput::APhysicalOutput(int index, String name):APhysicalIO{index, name}
{
}

inline void APhysicalOutput::update(){
  AIO::update();
}

//#define APhysicalOutput MotherboardNamespace::APhysicalOutput
#endif
