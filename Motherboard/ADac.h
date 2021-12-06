#ifndef ADac_h
#define ADac_h

#include "APhysicalOutput.h"

class ADac : public APhysicalOutput
{

public:
//  using APhysicalIO::APhysicalIO;
  ADac(int index, String name);

protected:
  String type = "ADac";
};

inline ADac::ADac(int index, String name):APhysicalOutput{index, name}{
  AIO::registerOutput(this);
}

#define ADac MotherboardNamespace::ADac
#endif
