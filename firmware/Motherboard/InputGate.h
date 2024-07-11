#ifndef InputGate_h
#define InputGate_h

#include "Registrar.h"
#include "Input.h"

class InputGate : public Input, public Registrar<InputGate>
{
public:
    InputGate(int8_t index);
    void update(void);
    int16_t *&updateBefore(int16_t *&blockData);
    void setThresholds(int16_t openThreshold, int16_t closeThreshold);

private:
    uint8_t index;
    int16_t openThreshold = 100;
    int16_t closeThreshold = -100;
};

inline InputGate::InputGate(int8_t index) : Input(index)
{
}

inline void InputGate::update(void)
{
    Input::update();
}

inline int16_t *&InputGate::updateBefore(int16_t *&blockData)
{
    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
        if (blockData[i] > openThreshold)
        {
            blockData[i] = INT16_MAX;
        }
        else if (blockData[i] < closeThreshold)
        {
            blockData[i] = INT16_MIN;
        }
    }

    return blockData;
}

inline void InputGate::setThresholds(int16_t openThreshold, int16_t closeThreshold)
{
    this->openThreshold = openThreshold;
    this->closeThreshold = closeThreshold;
}

#endif
