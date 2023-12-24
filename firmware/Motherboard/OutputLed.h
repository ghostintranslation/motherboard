#ifndef OutputLed_h
#define OutputLed_h

#include "Registrar.h"
#include "Output.h"

class OutputLed : public Output, public Registrar<OutputLed>
{
public:
    enum Status
    {
        Off,
        On,
        Blink,
        BlinkFast,
        BlinkOnce
    };

    OutputLed(int8_t index);
    void update(void);
    void setLowPassCoeff(float coeff);
    void setStatus(OutputLed::Status status);

private:
    audio_block_t *inputQueueArray[1];
    uint8_t index;
    uint8_t multiplier = 0;
    float lowPassCoeff = 0.001;

    Status status = Off;

    // Time counter for the blinking
    elapsedMillis blinkClock;
};

inline OutputLed::OutputLed(int8_t index) : Output(index)
{
}

inline void OutputLed::update(void)
{
    Output::update();

    switch (this->status)
    {
    case Blink:
        if (this->blinkClock % 400 < 200)
        {
            this->multiplier = 0;
        }
        else
        {
            this->multiplier = 1;
        }
        break;
    case BlinkFast:
        if (this->blinkClock % 200 < 100)
        {
            this->multiplier = 0;
        }
        else
        {
            this->multiplier = 1;
        }
        break;
    case BlinkOnce:
        if (this->blinkClock > 100)
        {
            this->multiplier = 0;
        }
        else
        {
            this->multiplier = 1;
        }
        break;

    case Off:
        this->multiplier = 0;
        break;

    default:
        this->multiplier = 1;
        break;
    }

    for (int i = 0; i < buffSize; i++)
    {
        this->queue[this->index][i] *= this->multiplier;
    }
}

inline void OutputLed::setStatus(OutputLed::Status status)
{
    this->status = status;

    if (status == BlinkOnce)
    {
        this->blinkClock = 0;
    }
}

inline void OutputLed::setLowPassCoeff(float coeff)
{
    if (coeff < 0.0f)
    {
        coeff = 0;
    }
    if (coeff > 1.0f)
    {
        coeff = 1;
    }
    this->lowPassCoeff = coeff;
}

#endif
