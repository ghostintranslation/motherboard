#ifndef InputRotary_h
#define InputRotary_h

#include "Registrar.h"
#include "Input.h"

class InputRotary : public Input, public Registrar<InputRotary>
{
public:
    InputRotary(int8_t index);
    void update(void);
    int16_t *&updateBefore(int16_t *&blockData);
    // void setThresholds(int16_t onThreshold, int16_t offThreshold);
    void onChange(void (*onChangeCallback)(uint16_t value));
    void onIncrease(void (*onIncreaseCallback)(uint16_t value));
    void onDecrease(void (*onDecreaseCallback)(uint16_t value));

private:
    uint8_t index;
    int state = 0;
    int16_t thresholdStart = 0;
    int16_t thresholdA = -20000;
    int16_t thresholdB = -2000;
    uint16_t value;
    void (*onChangeCallback)(uint16_t value) = nullptr;
    void (*onIncreaseCallback)(uint16_t value) = nullptr;
    void (*onDecreaseCallback)(uint16_t value) = nullptr;
};

inline InputRotary::InputRotary(int8_t index) : Input(index)
{
    this->setLowPassCoeff(1);
}

inline void InputRotary::update(void)
{
    Input::update();
}

inline int16_t *&InputRotary::updateBefore(int16_t *&blockData)
{

    for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
        switch (state)
        {
        case 0:
            if (blockData[i] < thresholdA)
            {
                state = 1;
            }
            else if (blockData[i] < thresholdB && blockData[i] > thresholdA)
            {
                state = -1;
            }
            break;
        case 1:
            if (blockData[i] < thresholdB && blockData[i] > thresholdA)
            {
                state = 2;
            }
            break;
        case -1:
            if (blockData[i] < thresholdA)
            {
                state = -2;
            }
            break;
        case 2:
            if (value < UINT16_MAX)
            {
                value++;
            }

            state = 0;

            if (this->onChangeCallback)
            {
                this->onChangeCallback(value);
            }
            if (this->onIncreaseCallback)
            {
                this->onIncreaseCallback(value);
            }
            break;
        case -2:
            if (value > 0)
            {
                value--;
            }

            state = 0;

            if (this->onChangeCallback)
            {
                this->onChangeCallback(value);
            }
            if (this->onDecreaseCallback)
            {
                this->onDecreaseCallback(value);
            }
            break;
        }
        if (blockData[i] > thresholdStart)
        {
            state = 0;
        }
    }

    return blockData;
}

// inline void InputRotary::setThresholds(int16_t onThreshold, int16_t offThreshold)
// {
//     this->onThreshold = onThreshold;
//     this->offThreshold = offThreshold;
// }

inline void InputRotary::onChange(void (*onChangeCallback)(uint16_t value))
{
    this->onChangeCallback = onChangeCallback;
}

inline void InputRotary::onIncrease(void (*onIncreaseCallback)(uint16_t value))
{
    this->onIncreaseCallback = onIncreaseCallback;
}

inline void InputRotary::onDecrease(void (*onDecreaseCallback)(uint16_t value))
{
    this->onDecreaseCallback = onDecreaseCallback;
}

#endif
