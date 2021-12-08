#ifndef OutputJack_h
#define OutputJack_h

#include "PhysicalOutput.h"

class OutputJack : public PhysicalOutput
{

public:
  OutputJack(int index, String name);

protected:
  String className = "OutputJack";
};

inline OutputJack::OutputJack(int index, String name):PhysicalOutput{index, name}{
  IO::registerOutput(this);
}

#define OutputJack MotherboardNamespace::OutputJack
#endif
