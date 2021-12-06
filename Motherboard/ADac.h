#ifndef ADac_h
#define ADac_h

#include "APhysicalOutput.h"

class ADac : public APhysicalOutput
{

public:
  using APhysicalOutput::APhysicalOutput;

  String getType() override;
};


inline String ADac::getType()
{
  return "ADac";
}

#define ADac MotherboardNamespace::ADac
#endif
