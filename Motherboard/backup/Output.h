#ifndef output_h
#define output_h

#include "PhysicalIO.h"

class Output : public PhysicalIO
{
public:
    //    virtual void write(); // TODO: NEED?
    Output(int index, String name);
    
};

inline Output::Output(int index, String name):PhysicalIO{index, name}
{
//  if(this->getType() != "Led"){
//    IO::registerOutput(this);
//  }
}

#endif
