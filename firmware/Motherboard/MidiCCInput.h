#ifndef MidiCCInput_h
#define MidiCCInput_h

#include "Setting.h"
#include "MidiInput.h"

// TODO: add callbacks

class MidiCCInput : public MidiInput, public Registrar<MidiCCInput>
{
public:
    MidiCCInput(Setting *ccSetting);
    MidiCCInput(byte control);
    void setMin(byte min);
    void setMax(byte max);
    byte getMin();
    byte getMax();
    byte getControl();
    static void handleMidiControlChange(byte channel, byte control, byte value);

private:
    byte min = 0;
    byte max = 127;
    byte control = 0;
};

inline MidiCCInput::MidiCCInput(Setting *ccSetting) : MidiInput(ccSetting)
{
    MIDI.setHandleControlChange(handleMidiControlChange);
    usbMIDI.setHandleControlChange(handleMidiControlChange);
}

inline MidiCCInput::MidiCCInput(byte control) : MidiInput(nullptr)
{
    MIDI.setHandleControlChange(handleMidiControlChange);
    usbMIDI.setHandleControlChange(handleMidiControlChange);

    this->control = control;
}

inline void MidiCCInput::setMin(byte min)
{
    // Min has to be max - 1;
    if (min > 126)
    {
        min = 126;
    }

    this->min = min;
}

inline void MidiCCInput::setMax(byte max)
{

    if (max > 127)
    {
        max = 127;
    }

    this->max = max;
}

inline byte MidiCCInput::getMin()
{
    return this->min;
}

inline byte MidiCCInput::getMax()
{
    return this->max;
}

inline byte MidiCCInput::getControl()
{
    return this->control;
}

inline void MidiCCInput::handleMidiControlChange(byte channel, byte control, byte value)
{

    for (unsigned int i = 0; i < MidiCCInput::getCount(); i++)
    {
        MidiCCInput *midiInput = MidiCCInput::get(i);
        if (midiInput->setting != nullptr)
        {
            if ((!isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getValue() == control) || (isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getDefaultValue() == control))
            {
                byte mappedValue = map(value, 0, 127, midiInput->getMin(), midiInput->getMax());
                midiInput->setValue(mappedValue);
            }
        }
        else if (control == midiInput->getControl())
        {
            byte mappedValue = map(value, 0, 127, midiInput->getMin(), midiInput->getMax());
            midiInput->setValue(mappedValue);
        }
    }
}

#endif