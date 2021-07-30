#ifndef cv_h
#define cv_h

#include "Output.h"

class CV : public Output
{
public:
  String getType() override;
};

inline String CV::getType()
{
  return "CV";
}

// From now on "Potentiometer" will be replaced by "new Potentiometer()"
#define CV new CV()
#endif
