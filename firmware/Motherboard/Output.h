#ifndef Output_h
#define Output_h

#include "AudioStream.h"
#include <SPI.h>

#define REGISTERS_LATCH_PIN 9
#define SPI_CLOCK_PIN 8
#define SPI_MOSI_PIN 11

class Output : public AudioStream
{
public:
    Output(int8_t index);
    void update(void);
    virtual int16_t *&updateBefore(int16_t *&blockData) { return blockData; };
    static void timerCallback();
    void setSmoothing(float smoothing);

protected:
    audio_block_t *inputQueueArray[1];
    uint8_t index;
    static uint16_t error[8];
    static uint8_t previousBits;
    static const uint16_t resolution = 256;
    static const uint32_t pwmSampleRate = (786000 / resolution);
    static const uint16_t buffSize = (1.0 / AUDIO_SAMPLE_RATE * AUDIO_BLOCK_SAMPLES) / (1.0 / pwmSampleRate) + 0.5;
    static const uint16_t audioSampleRateToPwmSampleRateRatio = ((uint16_t)AUDIO_SAMPLE_RATE / pwmSampleRate);
    static uint16_t queue[8][buffSize * 2];
    static uint16_t head[8];
    float smoothing = 0;
    double smoothingPrevValue = 0;
};

// PeriodicTimer* Output::t1;
uint8_t Output::previousBits = 0;
uint16_t Output::error[8] = {0};
uint16_t Output::head[8] = {0};
uint16_t Output::queue[8][buffSize * 2] = {{0}};

inline Output::Output(int8_t index)
    : AudioStream(1, inputQueueArray)
{
    this->index = index;
    this->active = true;

    // SPI
    pinMode(REGISTERS_LATCH_PIN, OUTPUT);
    // pinMode(SPI_CLOCK_PIN, OUTPUT);
    // pinMode(SPI_MOSI_PIN, OUTPUT);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    // SPI.setSCK(SPI_CLOCK_PIN);
    // SPI.setMOSI(SPI_MOSI_PIN);
    // SPI.setMISO(8);
    SPI.begin();

    // Timer
    PIT_MCR = 0;                 // Enable PIT
    PIT_LDVAL0 = 60;             // 60=786000Hz 70=675000Hz // 135 = 352800Hz
    PIT_TCTRL0 = PIT_TCTRL_TIE;  // PIT Interrupt enable for Timer0
    PIT_TCTRL0 |= PIT_TCTRL_TEN; // start Timer0
    attachInterruptVector(IRQ_PIT, timerCallback);
    NVIC_ENABLE_IRQ(IRQ_PIT);
}

inline void Output::update(void)
{
    // Receive input data
    audio_block_t *block;
    block = receiveReadOnly(0);

    int16_t *blockDataPointer;
    int16_t blockData[AUDIO_BLOCK_SAMPLES]{0};
    blockDataPointer = blockData;

    if (block)
    {
        for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
        {
            blockDataPointer[i] = block->data[i];
        }

        release(block);
    }

    // Allows for derived class to alter the block before being processed here
    blockDataPointer = this->updateBefore(blockDataPointer);

    uint8_t sampleIndex = head[this->index];

    for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {

        this->smoothingPrevValue = (double)blockDataPointer[i] * (1.0f - this->smoothing) + this->smoothingPrevValue * this->smoothing;
        blockDataPointer[i] = this->smoothingPrevValue;

        if (audioSampleRateToPwmSampleRateRatio <= 1 || i % audioSampleRateToPwmSampleRateRatio == 0)
        {

            if (++sampleIndex >= buffSize)
            {
                sampleIndex = 0;
            }

            uint16_t offsetValue = (blockDataPointer[i] + INT16_MAX) / 65535.0 * resolution + 0.5;
            queue[this->index][sampleIndex] = offsetValue;
        }
    }
}

inline void Output::timerCallback()
{

    uint8_t bits = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (head[i] >= buffSize)
        {
            head[i] = 0;
        }

        error[i] += queue[i][head[i]++]; // integrate error (SIGMA)

        if (error[i] >= resolution) // if error >= max value (DELTA)
        {
            bits = bits | 1 << i;
            error[i] -= resolution;
        }
    }
    // Serial.println(bits);

    // if (previousBits != bits)
    // {
    // digitalWriteFast(13, bits & 1);

    SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

    // Set the latch to low (activate the shift registers)
    digitalWriteFast(REGISTERS_LATCH_PIN, LOW);

    // // Send the data
    SPI.transfer(bits);

    // Set the latch to high (shift registers actually set their pins and stop listening)
    digitalWriteFast(REGISTERS_LATCH_PIN, HIGH);

    SPI.endTransaction();

    previousBits = bits;
    // }

    PIT_TFLG0 |= PIT_TFLG_TIF; // to enable interrupt again
}

/**
 * @brief Set the smoothing coefficient
 *
 * @param smoothing Between 0 and 1;
 */
inline void Output::setSmoothing(float smoothing)
{
    if (smoothing < 0)
    {
        smoothing = 0;
    }
    else if (smoothing >= 1)
    {
        smoothing = 0.999999;
    }

    // if (smoothing == 0)
    // {
    //     this->smoothing = 0;
    // }
    // else
    // {
    //     this->smoothing = log10(9.99 + smoothing * 0.01);
    // }
    this->smoothing = smoothing;
}

#endif
