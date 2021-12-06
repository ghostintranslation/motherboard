#ifndef PhysicalIO_h
#define PhysicalIO_h

#include "IO.h"

class PhysicalIO : public IO
{
public:
    PhysicalIO(int index, String name);
    byte getIndex();

protected:
    // The index starting at 0
    byte index = 0;
    
};

inline PhysicalIO::PhysicalIO(int index, String name):IO{name}
{
  this->index = index;
}

inline byte PhysicalIO::getIndex()
{
    return this->index;
}

#endif
