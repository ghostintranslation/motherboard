#ifndef InputRotary_h
#define InputRotary_h

#include "Registrar.h"
#include "Input.h"

/**
 * Rotary Encoder type of input
 * Pins should be connected this way:
 * - Out A -> 20k -> Input
 * - Out B -> 10k -> Input
 * - The pin in the middle to GND
 * - Switch pin 1 -> Input
 * - Switch pin 2 -> GND
 * - There should be a pull up of 2k on the Input
 */
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
    void onPush(void (*onPushCallback)());
    void onRelease(void (*onReleaseCallback)());
    void onClick(void (*onClickCallback)());
    void onLongClick(void (*onLongClickCallback)());

private:
    uint8_t index;
    int state = 0;
    int16_t thresholdA = 22000;
    int16_t thresholdB = 19000;
    int16_t thresholdC = 15000;
    int16_t thresholdD = -30000;
    uint16_t value;
    bool isPushed;
    elapsedMillis millisSincePushed;
    void (*onChangeCallback)(uint16_t value) = nullptr;
    void (*onIncreaseCallback)(uint16_t value) = nullptr;
    void (*onDecreaseCallback)(uint16_t value) = nullptr;
    void (*onPushCallback)() = nullptr;
    void (*onReleaseCallback)() = nullptr;
    void (*onClickCallback)() = nullptr;
    void (*onLongClickCallback)() = nullptr;
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
        // Serial.println(blockData[i]);
        switch (state)
        {
        case 0:
            if (blockData[i] < thresholdA && blockData[i] > thresholdB)
            {
                state = 1;
                // Serial.println(state);
            }
            else if (blockData[i] < thresholdB && blockData[i] > thresholdC)
            {
                state = -1;
                // Serial.println(state);
            }
            break;
        case 1:
            if (blockData[i] < thresholdC)
            {
                state = 2;
            }
            break;
        case -1:
            if (blockData[i] < thresholdC)
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

        if (!isPushed && blockData[i] < thresholdD)
        {
            isPushed = true;
            millisSincePushed = 0;
            if (this->onPushCallback)
            {
                this->onPushCallback();
            }
        }
        else if (isPushed && blockData[i] > thresholdD)
        {
            isPushed = false;
            if (this->onReleaseCallback)
            {
                this->onReleaseCallback();
            }
            if (millisSincePushed > 200)
            {
                if (this->onLongClickCallback)
                {
                    this->onLongClickCallback();
                }
            }
            else
            {
                if (this->onClickCallback)
                {
                    this->onClickCallback();
                }
            }
        }

        if (blockData[i] > thresholdA)
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

inline void InputRotary::onPush(void (*onPushCallback)())
{
    this->onPushCallback = onPushCallback;
}

inline void InputRotary::onRelease(void (*onReleaseCallback)())
{
    this->onReleaseCallback = onReleaseCallback;
}

inline void InputRotary::onClick(void (*onClickCallback)())
{
    this->onClickCallback = onClickCallback;
}

inline void InputRotary::onLongClick(void (*onLongClickCallback)())
{
    this->onLongClickCallback = onLongClickCallback;
}
#endif
