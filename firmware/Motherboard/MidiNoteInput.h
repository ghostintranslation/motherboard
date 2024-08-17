#ifndef MidiNoteInput_h
#define MidiNoteInput_h

#include "Setting.h"
#include "MidiInput.h"

// TODO: add callbacks

class MidiNoteInput : public MidiInput, public Registrar<MidiNoteInput>
{
public:
    MidiNoteInput(Setting *noteSetting);
    MidiNoteInput(byte note);
    void setMin(byte min);
    void setMax(byte max);
    byte getMin();
    byte getMax();
    byte getNote();
    static void handleMidiNoteOn(byte channel, byte control, byte velocity);
    static void handleMidiNoteOff(byte channel, byte control, byte velocity);

private:
    byte min = 0;
    byte max = 127;
    byte note = 0;
};

inline MidiNoteInput::MidiNoteInput(Setting *setting = nullptr) : MidiInput(setting)
{
    MIDI.setHandleNoteOn(handleMidiNoteOn);
    MIDI.setHandleNoteOff(handleMidiNoteOff);
    usbMIDI.setHandleNoteOn(handleMidiNoteOn);
    usbMIDI.setHandleNoteOff(handleMidiNoteOff);

    // TODO: this->note = noteSetting->getValue() ?
    // Could make things easier but would require a reboot after settings change
}

inline MidiNoteInput::MidiNoteInput(byte note) : MidiInput(nullptr)
{
    MIDI.setHandleNoteOn(handleMidiNoteOn);
    MIDI.setHandleNoteOff(handleMidiNoteOff);
    usbMIDI.setHandleNoteOn(handleMidiNoteOn);
    usbMIDI.setHandleNoteOff(handleMidiNoteOff);

    this->note = note;
}

inline void MidiNoteInput::setMin(byte min)
{
    // Min has to be max - 1;
    if (min > 126)
    {
        min = 126;
    }

    this->min = min;
}

inline void MidiNoteInput::setMax(byte max)
{

    if (max > 127)
    {
        max = 127;
    }

    this->max = max;
}

inline byte MidiNoteInput::getMin()
{
    return this->min;
}

inline byte MidiNoteInput::getMax()
{
    return this->max;
}

inline byte MidiNoteInput::getNote()
{
    return this->note;
}


inline void MidiNoteInput::handleMidiNoteOn(byte channel, byte note, byte velocity)
{
    for (unsigned int i = 0; i < MidiNoteInput::getCount(); i++)
    {
        MidiNoteInput *midiInput = MidiNoteInput::get(i);
        byte mappedValue = map(velocity, 0, 127, midiInput->getMin(), midiInput->getMax());

        if (midiInput->setting != nullptr)
        {
            if ((!isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getValue() == note) || (isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getDefaultValue() == note))
            {
                midiInput->setValue(mappedValue);
            }
        }else if (note == midiInput->getNote())
        {
            midiInput->setValue(mappedValue);
        }
    }
}

inline void MidiNoteInput::handleMidiNoteOff(byte channel, byte note, byte velocity)
{
    MidiNoteInput::handleMidiNoteOn(channel, note, 0);
}

#endif