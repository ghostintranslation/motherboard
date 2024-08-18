#ifndef MidiIO_h
#define MidiIO_h

class MidiIO
{
public:
    MidiIO(Setting *setting);
    byte getValue();
    void setValue(byte value);
    int16_t *getBlockData();
    Setting *getSetting();
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
}

inline byte MidiIO::getValue()
{
    return this->value;
}

inline void MidiIO::setValue(byte value)
{
    this->value = value;
}

inline int16_t *MidiIO::getBlockData()
{
    return this->blockData;
}

inline Setting *MidiIO::getSetting()
{
    return this->setting;
}
#endif