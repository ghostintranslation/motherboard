#ifndef M2_h
#define M2_h

#define ADC1_MULTIPLEXER_BIT1_PIN 2
#define ADC1_MULTIPLEXER_BIT2_PIN 3
#define ADC1_MULTIPLEXER_BIT3_PIN 4

#define ADC2_MULTIPLEXER_BIT1_PIN 5
#define ADC2_MULTIPLEXER_BIT2_PIN 6
#define ADC2_MULTIPLEXER_BIT3_PIN 10

#define REGISTERS_LATCH_PIN 9
#define SPI_CLOCK_PIN 13
#define SPI_MOSI_PIN 11
#define ABSOLUTE_ANALOG_MIN -32768
#define ABSOLUTE_ANALOG_MAX 32767
#define ANALOG_RANGE 65536

#include "Input.h"

class M2
{
public:
    static M2 *getInstance();

private:
    // Singleton
    static M2 *instance;
    M2();
};

// Singleton pre init
M2 *M2::instance = nullptr;

inline M2::M2()
{
    pinMode(REGISTERS_LATCH_PIN, OUTPUT);
    pinMode(SPI_CLOCK_PIN, OUTPUT);
    pinMode(SPI_MOSI_PIN, OUTPUT);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setSCK(SPI_CLOCK_PIN);
    SPI.setMOSI(SPI_MOSI_PIN);
    SPI.begin();
}

/**
 * Singleton instance
 */
inline M2 *ETC::getInstance()
{
    if (!instance)
        instance = new ETC;
    return instance;
}

M2 &M2 = *M2::getInstance();

#endif