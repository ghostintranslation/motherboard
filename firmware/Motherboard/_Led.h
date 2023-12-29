#ifndef Led_h
#define Led_h

#include "Registrar.h"
#include "AudioStream.h"

#define REGISTERS_LATCH_PIN 9
#define SPI_CLOCK_PIN 13
#define SPI_MOSI_PIN 11

class Led : public AudioStream, public Registrar<Led>
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

    Led(int8_t index);
    void update(void);
    void setLowPassCoeff(float coeff);
    void setStatus(Led::Status status);
    int16_t getValue();
    int8_t getIndex();

private:
    audio_block_t *inputQueueArray[1];
    uint8_t index;
    static const unsigned int ledsMax = 32;
    float value = INT16_MIN;
    int16_t multiplier = INT16_MIN;
    float lowPassCoeff = 0.001;
    static uint16_t timerCallbackCount;
    static void timerCallback();

    static uint8_t currentSampleIndex;

    Status status = Off;

    // Time counter for the blinking
    elapsedMillis blinkClock;

    static uint8_t ledsData;
    static const uint16_t resolution = 255; // 65536;
    static uint16_t ledsData2[AUDIO_BLOCK_SAMPLES][resolution];
    static void writeLeds();
};

uint8_t Led::ledsData = 0;
uint16_t Led::timerCallbackCount = 0;
uint8_t Led::currentSampleIndex = 0;
uint16_t Led::ledsData2[AUDIO_BLOCK_SAMPLES][resolution];

inline Led::Led(int8_t index) : AudioStream(1, inputQueueArray)
{
    this->index = index;
    this->active = true;

    pinMode(REGISTERS_LATCH_PIN, OUTPUT);
    pinMode(SPI_CLOCK_PIN, OUTPUT);
    pinMode(SPI_MOSI_PIN, OUTPUT);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setSCK(SPI_CLOCK_PIN);
    SPI.setMOSI(SPI_MOSI_PIN);
    SPI.begin();

    PIT_MCR = 0;                 // Enable PIT
    PIT_LDVAL0 = 135;            // 545;            // 135 = 352800Hz  135 = 1/352800
    PIT_TCTRL0 = PIT_TCTRL_TIE;  // PIT Interrupt enable for Timer0
    PIT_TCTRL0 |= PIT_TCTRL_TEN; // start Timer0
    attachInterruptVector(IRQ_PIT, timerCallback);
    NVIC_ENABLE_IRQ(IRQ_PIT);
}

inline void Led::update(void)
{
    // Receive input data
    audio_block_t *block;
    block = receiveReadOnly(0);

    if (block)
    {
        for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
        {
            int16_t newVal = block->data[i] * this->multiplier;
            this->value = (this->lowPassCoeff * newVal) + (1.0f - this->lowPassCoeff) * this->value;

            uint16_t offsetValue = (this->value + INT16_MAX) / 65535.0 * resolution + 0.5;
            for (int t = 0; t < resolution; ++t)
            {
                ledsData2[i][t] ^= (-(offsetValue > t) ^ ledsData2[i][t]) & (1 << (this->index));
            }
        }

        // currentSampleIndex = 0;
        // uint16_t offsetValue = 127; //(this->value + INT16_MAX) / 65535.0 * resolution + 0.5;
        // for (int t = 0; t < resolution; ++t)
        // {
        //     // set (pin % 8)th bit to (value > t)
        //     ledsData2[t] ^= (-(offsetValue > t) ^ ledsData2[t]) & (1 << (this->index));
        // }

        release(block);
    }

    switch (this->status)
    {
    case Blink:
        if (this->blinkClock % 400 < 200)
        {
            this->multiplier = INT16_MIN;
        }
        else
        {
            this->multiplier = 1;
        }
        break;
    case BlinkFast:
        if (this->blinkClock % 200 < 100)
        {
            this->multiplier = INT16_MIN;
        }
        else
        {
            this->multiplier = 1;
        }
        break;
    case BlinkOnce:
        if (this->blinkClock > 100)
        {
            this->multiplier = INT16_MIN;
        }
        else
        {
            this->multiplier = 1;
        }
        break;

    case Off:
        this->multiplier = INT16_MIN;
        break;

    default:
        this->multiplier = 1;
        break;
    }
}
// elapsedMicros counter;
inline void Led::timerCallback()
{
    timerCallbackCount++;
    timerCallbackCount = timerCallbackCount % resolution;

    if (timerCallbackCount == 0)
    {
        currentSampleIndex++;
        Serial.println(currentSampleIndex);

        if (currentSampleIndex >= AUDIO_BLOCK_SAMPLES)
        {
            currentSampleIndex = 0;
        }
        //     Serial.println(counter);
        //     counter = 0;
    }

    writeLeds();

    PIT_TFLG0 |= PIT_TFLG_TIF; // to enable interrupt again
}

inline int16_t Led::getValue()
{
    return this->value;
}

inline int8_t Led::getIndex()
{
    return this->index;
}

inline void Led::setStatus(Led::Status status)
{
    this->status = status;

    if (status == BlinkOnce)
    {
        this->blinkClock = 0;
    }
}

inline void Led::setLowPassCoeff(float coeff)
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

uint8_t bip = B11111111;

inline void Led::writeLeds()
{
    // Preparing the shift register data
    // uint8_t newLedsData = 0;

    // // Preparing the LEDs data
    // for (uint8_t i = 0; i < Led::getCount(); i++)
    // {
    //     if (Led::get(i)->getValue() == INT16_MIN)
    //     {
    //         continue;
    //     }
    //     else if (Led::get(i)->getValue() == INT16_MAX)
    //     {
    //         bitSet(newLedsData, Led::get(i)->getIndex());
    //     }
    //     // The value is a int16_t, offseting it to uint16_t in order to run this comparison
    //     else if (timerCallbackCount < (Led::get(i)->getValue() + INT16_MAX) >> 8)
    //     {
    //         bitSet(newLedsData, Led::get(i)->getIndex());
    //     }
    // }

    // // Reduce SPI use by not pushing data if not necessary
    // if (ledsData == newLedsData)
    // {
    //     return;
    // }

    // // Set the latch to low (activate the shift registers)
    // digitalWrite(REGISTERS_LATCH_PIN, LOW);

    // // Send the leds byte
    // SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    // SPI.transfer(newLedsData);
    // SPI.endTransaction();

    // // Set the latch to high (shift registers actually set their pins and stop listening)
    // digitalWrite(REGISTERS_LATCH_PIN, HIGH);

    // ledsData = newLedsData;

    // Set the latch to low (activate the shift registers)
    digitalWriteFast(REGISTERS_LATCH_PIN, LOW);

    // Send the leds byte
    SPI.beginTransaction(SPISettings(30000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(ledsData2[currentSampleIndex][timerCallbackCount]);
    // SPI.transfer(bip);
    SPI.endTransaction();

    bip ^= B11111111;

    // Set the latch to high (shift registers actually set their pins and stop listening)
    digitalWriteFast(REGISTERS_LATCH_PIN, HIGH);
}

#endif