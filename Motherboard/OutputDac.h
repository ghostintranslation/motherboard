#ifndef OutputDac_h
#define OutputDac_h

#include "PhysicalOutput.h"

class OutputDac : public PhysicalOutput
{

public:
//  using APhysicalIO::APhysicalIO;
  OutputDac(int index, String name);

protected:
  String type = "OutputDac";
};

inline OutputDac::OutputDac(int index, String name):PhysicalOutput{index, name}{
  IO::registerOutput(this);
}

#define OutputDac MotherboardNamespace::OutputDac
#endif
