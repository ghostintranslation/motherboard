#ifndef MidiInput_h
#define MidiInput_h

#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // MIDI library init

using MidiControlChangeCallbackFunction = void (*)(int16_t);

// TODO: Add a change callback
class MidiInput : public AudioStream
{
public:
    MidiInput(Setting *setting);
    void update(void);
    byte getValue();
    void setValue(byte value);
    int16_t *getBlockData();
    // TODO: setChannelSetting

protected:
    Setting *setting = nullptr;
    byte value = 0;
    int16_t mappedValue = 0;
    int16_t blockData[AUDIO_BLOCK_SAMPLES] = {0};
};

inline MidiInput::MidiInput(Setting *setting = nullptr) : AudioStream(0, NULL)
{
    this->active = true;

    if (setting != nullptr)
    {
        this->setting = setting;
    }

    // MIDI init
    MIDI.begin();
    Serial1.begin(31250, SERIAL_8N1_RXINV);
}

inline void MidiInput::update(void)
{
    usbMIDI.read();

    audio_block_t *block;

    // allocate the audio blocks to transmit
    block = allocate();

    if (block)
    {
        int16_t newMappedValue = map(this->value, 0, 127, INT16_MIN, INT16_MAX);
        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
        {
            this->mappedValue = (float)this->mappedValue * 0.9 + (float)newMappedValue * 0.1;
            block->data[i] = this->mappedValue;
            this->blockData[i] = this->mappedValue;
        }

        transmit(block, 0);
        release(block);
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

inline int16_t *MidiInput::getBlockData()
{
    return this->blockData;
}
#endif