#ifndef MidiNoteInput_h
#define MidiNoteInput_h

#include "Setting.h"
#include "MidiInput.h"

class MidiNoteInput : public MidiInput, public Registrar<MidiNoteInput>
{
public:
    MidiNoteInput(Setting *setting);
    static void handleMidiNoteOn(byte channel, byte control, byte velocity);
    static void handleMidiNoteOff(byte channel, byte control, byte velocity);

private:
    // TODO: bool isNoteOn;
    // TODO: add callbacks
};

inline MidiNoteInput::MidiNoteInput(Setting *setting = nullptr) : MidiInput(setting)
{
    MIDI.setHandleNoteOn(handleMidiNoteOn);
    MIDI.setHandleNoteOff(handleMidiNoteOff);
    usbMIDI.setHandleNoteOn(handleMidiNoteOn);
    usbMIDI.setHandleNoteOff(handleMidiNoteOff);
}

inline void MidiNoteInput::handleMidiNoteOn(byte channel, byte note, byte velocity)
{
    for (unsigned int i = 0; i < MidiNoteInput::getCount(); i++)
    {
        MidiNoteInput *midiInput = MidiNoteInput::get(i);
        if (midiInput->setting != nullptr)
        {
            if ((!isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getValue() == note) || (isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getDefaultValue() == note))
            {
                midiInput->setValue(velocity);
            }
        }
    }
}

inline void MidiNoteInput::handleMidiNoteOff(byte channel, byte note, byte velocity)
{
}

#endif