#ifndef PhysicalOutput_h
#define PhysicalOutput_h

#include "PhysicalIO.h"

class PhysicalOutput : public PhysicalIO
{
public:
    //    virtual void write(); // TODO: NEED?
    PhysicalOutput(int index, String name);

    void update() override;
};

inline PhysicalOutput::PhysicalOutput(int index, String name):PhysicalIO{index, name}
{
}

inline void PhysicalOutput::update(){
  IO::update();
}

//#define PhysicalOutput MotherboardNamespace::PhysicalOutput
#endif
