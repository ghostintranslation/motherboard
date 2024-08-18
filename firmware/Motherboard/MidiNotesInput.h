#ifndef MidiNotesInput_h
#define MidiNotesInput_h

#include "Setting.h"
#include "MidiInput.h"

// TODO: add callbacks

class MidiNotesInput : public MidiInput, public Registrar<MidiNotesInput>
{
public:
    MidiNotesInput();

private:
};

inline MidiNotesInput::MidiNotesInput() : MidiInput(nullptr)
{
}

#endif