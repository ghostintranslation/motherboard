#ifndef MidiInput_h
#define MidiInput_h

#include <cmath>
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // MIDI library init

using MidiControlChangeCallbackFunction = void (*)(int16_t);

class MidiInput : public AudioStream, public Registrar<MidiInput>
{
public:
    MidiInput(Setting *setting);
    void update(void);
    byte getValue();
    void setValue(byte value);
    static void handleMidiControlChange(byte channel, byte control, byte value);

private:
    // static IntervalTimer myTimer;
    static void readMidi();
    Setting *setting = nullptr;
    byte value = 0;
};

// IntervalTimer MidiInput::myTimer;

inline MidiInput::MidiInput(Setting *setting = nullptr) : AudioStream(0, NULL)
{

    if (setting != nullptr)
    {
        this->setting = setting;
    }

    // MIDI init
    if (MidiInput::getCount() == 1)
    {
        Serial.println("MidiInput::getCount() == 1");
        MIDI.setHandleControlChange(handleMidiControlChange);
        MIDI.begin();
        usbMIDI.setHandleControlChange(handleMidiControlChange);
        Serial1.begin(31250, SERIAL_8N1_RXINV);

        // myTimer.begin(readMidi, 1); // read midi every 1us
    }
}

inline void MidiInput::readMidi()
{
    // Serial.println("readMidi");
    // MIDI.read();
    // usbMIDI.read();
}

inline void MidiInput::handleMidiControlChange(byte channel, byte control, byte value)
{
    for (unsigned int i = 0; i < MidiInput::getCount(); i++)
    {
        MidiInput *midiInput = MidiInput::get(i);
        if (midiInput->setting != nullptr)
        {
            if ((!isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getValue() == control) || (isnan(midiInput->setting->getValue()) && (byte)midiInput->setting->getDefaultValue() == control))
            {
                Serial.println(value);
                midiInput->setValue(value);
            }
        }
    }
}

inline byte MidiInput::getValue()
{
    return this->value;
}

inline void MidiInput::setValue(byte value)
{
    this->value = value;
}

inline void MidiInput::update(void)
{
    audio_block_t *block;

    // allocate the audio blocks to transmit
    block = allocate();

    if (block)
    {
        int16_t mappedValue = map(this->value, 0, 127, INT16_MIN, INT16_MAX);
        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
        {
            block->data[i] = mappedValue;
        }

        transmit(block, 0);
        release(block);
    }
}

#endif