#ifndef InputMidiNote_h
#define InputMidiNote_h

#include "IO.h"

class InputMidiNote : public VirtualInput
{
public:
    InputMidiNote(String name, byte channel);
    byte getChannel();

protected:
    // The MIDI channel starting at 0
    byte channel = 0;
};

inline InputMidiNote::InputMidiNote(String name, byte channel):IO{name}
{
  this->channel = channel;
}

inline unsigned int InputMidiNote::getChannel()
{
    return this->channel;
}

#endif
