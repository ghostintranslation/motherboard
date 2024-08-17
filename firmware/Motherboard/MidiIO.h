#ifndef MidiIO_h
#define MidiIO_h

#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // MIDI library init

class MidiIO
{
public:
    MidiIO(Setting *setting);
    void update(void);
    byte getValue();
    void setValue(byte value);
    // int16_t *getBlockData();
    // TODO: setChannelSetting

protected:
    Setting *setting = nullptr;
    byte value = 0;
    int16_t mappedValue = 0; // TODO: rename
    int16_t blockData[AUDIO_BLOCK_SAMPLES] = {0};
};

inline MidiIO::MidiIO(Setting *setting = nullptr)
{
    this->setting = setting;

    // MIDI init
    MIDI.begin();
    Serial1.begin(31250, SERIAL_8N1_RXINV);
}

inline void MidiIO::update(void)
{
    usbMIDI.read();
    MIDI.read();
}

inline byte MidiIO::getValue()
{
    return this->value;
}

inline void MidiIO::setValue(byte value)
{
    this->value = value;
}

// inline int16_t *MidiIO::getBlockData()
// {
//     return this->blockData;
// }
#endif