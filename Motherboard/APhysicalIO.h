#ifndef APhysicalIO_h
#define APhysicalIO_h

#include "AIO.h"

class APhysicalIO : public AIO
{
public:
    APhysicalIO(unsigned int index, String name);
    unsigned int getIndex();

protected:
    // The index starting at 0
    unsigned int index = 0;
    
};

inline APhysicalIO::APhysicalIO(unsigned int index, String name):AIO{name}
{
  this->index = index;
}

inline unsigned int APhysicalIO::getIndex()
{
    return this->index;
}

//#define APhysicalIO MotherboardNamespace::APhysicalIO

#endif
