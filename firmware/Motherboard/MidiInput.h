#ifndef MidiInput_h
#define MidiInput_h

#include "AudioStream.h"
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // MIDI library init

using MidiControlChangeCallbackFunction = void (*)(int16_t);

class MidiInput : public AudioStream
{
public:
    MidiInput();
    void update(void);

private:
    static IntervalTimer myTimer;
    static void handleMidiControlChange(byte channel, byte control, byte value);
};

IntervalTimer MidiInput::myTimer;

inline MidiInput::MidiInput()
    : AudioStream(0, NULL)
{

    // MIDI init
    MIDI.setHandleControlChange(handleMidiControlChange);
    MIDI.begin();
    // usbMIDI.setHandleControlChange(handleMidiControlChange);

    Serial1.begin(31250, SERIAL_8N1_RXINV);

    myTimer.begin(readMidi, 1); // read midi every 1us
}

inline void MidiInput::update(void)
{
}

inline void MidiInput::readMidi()
{
    MIDI.read();
}

#endif