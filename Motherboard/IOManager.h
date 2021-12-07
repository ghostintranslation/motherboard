#ifndef IOManager_h
#define IOManager_h

// Teensy pins
#define MIDI_CHANNEL_A_PIN 2
#define MIDI_CHANNEL_B_PIN 3
#define MIDI_CHANNEL_C_PIN 4
#define MIDI_CHANNEL_D_PIN 5
#define REGISTERS_LATCH_PIN 9
#define SPI_CLOCK_PIN 13
#define SPI_MOSI_PIN 11
#define ANALOG_IN_1_PIN 14

// Callback types
//using TriggerDownCallback = void (*)(byte);
//using LongTriggerDownCallback = void (*)(byte);
//using TriggerUpCallback = void (*)(byte);
//using LongTriggerUpCallback = void (*)(byte);
using EdgeCallback = void (*)(String);
using ChangeCallback = void (*)(String name, float value, float diff);
using ChangeQuantizedCallback = void (*)(byte inputIndex, int value);


//#include "Led.h"
//#include "Input.h"
//#include "Output.h"
//#include "Button.h"
//#include "Trigger.h"
//#include "Gate.h"
//#include "Potentiometer.h"
//#include "RotaryEncoder.h"
//#include "TouchPad.h"
//#include "CvIn.h"
//#include "ToggleOnOn.h"
//#include "ToggleOnOffOn.h"
//#include "CvOut.h"
//#include "None.h"

#include "InputPotentiometer.h"
#include "OutputCV.h"
#include "Led.h"

class IOManager
{
public:

    // Init
    void init(byte columnNumber);

    // Update
    void update();

    // Debug
    void print();

    float getInputValue(byte index);
//    float getOutputValue(byte index);
//    void setOutputValue(byte index, unsigned int value);
    void setLED(byte index, Led::Status status, unsigned int brightness);
//    void setSmoothing(byte smoothing);
    unsigned int getAnalogMaxValue();
    unsigned int getAnalogMinValue();
    byte getMidiChannel();

    
    // Callbacks
//    void setHandleTriggerDown(byte inputIndex, TriggerDownCallback fptr);
//    void setHandleLongTriggerDown(byte inputIndex, LongTriggerDownCallback fptr);
//    void setHandleTriggerUp(byte inputIndex, TriggerUpCallback fptr);
//    void setHandleLongTriggerUp(byte inputIndex, LongTriggerUpCallback fptr);
//    void setHandleChange(byte inputIndex, ChangeCallback fptr);
//    void setHandleChangeQuantized(byte inputIndex, ChangeQuantizedCallback fptr);

private:
    // Only Motherboard can access this instance
    friend class Motherboard;
  
    // Singleton
    static IOManager *getInstance();
    
    // Singleton
    static IOManager *instance;
    IOManager();

    // Inputs/Outputs
    byte maxIoNumber = 0;
    byte currentInputIndex = 0;
    byte currentOutputIndex = 0;

    byte analogResolution = 12;
    byte midiChannel = 0;

    void iterateIO();
    void readWriteIO();

    
    static void handleMidiControlChange(byte channel, byte controlNumber, byte value);

    // Refresh clock
    const unsigned int intervalReadWrite = 4;
    elapsedMicros clockReadWrite;
    const unsigned int intervalIteration = 20000;
    elapsedMicros clockIteration;

    // PWM clock
    const float intervalPWM = 20000;
    elapsedMicros clockPWM;
};

// Instance pre init
IOManager *IOManager::instance = nullptr;

/**
 * Constructor
 */
inline IOManager::IOManager()
{
}

/**
 * Singleton instance
 */
inline IOManager *IOManager::getInstance()
{
    if (!instance)
        instance = new IOManager;
    return instance;
}

inline void IOManager::init(byte columnNumber)
{
    this->maxIoNumber = columnNumber * 3 + columnNumber;

    // Pin modes
    pinMode(MIDI_CHANNEL_A_PIN, INPUT_PULLUP);
    pinMode(MIDI_CHANNEL_B_PIN, INPUT_PULLUP);
    pinMode(MIDI_CHANNEL_C_PIN, INPUT_PULLUP);
    pinMode(MIDI_CHANNEL_D_PIN, INPUT_PULLUP);
    pinMode(REGISTERS_LATCH_PIN, OUTPUT);
    pinMode(SPI_CLOCK_PIN, OUTPUT);
    pinMode(SPI_MOSI_PIN, OUTPUT);

    analogReadResolution(getInstance()->analogResolution);

    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.begin();
}

/**
 * Update
 */
inline void IOManager::update()
{
    if (this->clockIteration >= this->intervalIteration)
    {
        // Iterate to the next input and output
        this->iterateIO();

        this->clockIteration = 0;
    }
    
    if (this->clockReadWrite >= this->intervalReadWrite)
    {
        // Read the current input and set the current output
        this->readWriteIO();

        this->clockReadWrite = 0;
    }

    if (clockPWM > intervalPWM)
    {
        clockPWM = 0;
    }
}

/**
 * Iterate over the inputs
 */
inline void IOManager::iterateIO()
{  
  if(IO::inputsSize > 0){
    do{
      // Iterate to the next or Iterate back to the first one
      this->currentInputIndex++;
      this->currentInputIndex = this->currentInputIndex % IO::inputsSize;

    // If current input Position is greater than the allowed max
    // then iterate again. This way we skip useless Inputs.
    }while(IO::getInputs()[this->currentInputIndex]->getIndex() > this->maxIoNumber);
  }

  if(IO::outputsSize > 0){
    do{
      // Iterate to the next or Iterate back to the first one
      this->currentOutputIndex++;
      this->currentOutputIndex = this->currentOutputIndex % IO::outputsSize;

    // If current input Position is greater than the allowed max
    // then iterate again. This way we skip useless Inputs.
    }while(IO::getOutputs()[this->currentOutputIndex]->getIndex() > this->maxIoNumber);
  }
}

/**
 * Send data to the chips to set the LEDs, the DAC, and read the inputs
 */
inline void IOManager::readWriteIO()
{
  /**
   * The data transfer goes like this:
   * - Set the latch to low (activate the shift registers)
   * - Send the byte to set the MUX and select the DAC
   * - Send the leds byte to complete the shift registers chain
   * - Set the latch to high (shift registers actually set their pins and stop listening)
   * - Send the selected DAC data
   * - Set the latch to low (now shift registers are listening)
   * - Send the same data to the shift register except the DAC selection bit goes to high (the DAC will actually set its pin)
   * - Send the leds byte to complete the shift registers chain
   * - Set the latch to high  (shift registers actually set their pins and stop listening)
   */
   
    unsigned int currentInputPosition = IO::inputsSize > this->currentInputIndex ? IO::getInputs()[this->currentInputIndex]->getIndex() : 0;
    
    unsigned int currentOutputPosition = IO::outputsSize > this->currentOutputIndex ? IO::getOutputs()[this->currentOutputIndex]->getIndex() : 0;

    byte currentOutputDacIndex = currentOutputPosition / 2;
    byte currentOutputDacChannel = currentOutputPosition % 2;

    // Set the latch to low (activate the shift registers)
    digitalWrite(REGISTERS_LATCH_PIN, LOW);

    // Preparing the shift register data
    unsigned long shiftRegistersData =  0x00;
    if (IO::inputsSize > this->currentInputIndex && IO::getInputs()[this->currentInputIndex]->isDirectToTeensy())
    {
        shiftRegistersData = 0x80;
    }
    
    shiftRegistersData = shiftRegistersData | currentInputPosition << 4 | (0x0F ^ 0x01 << currentOutputDacIndex);

    int ledsData = 0;

    // Preparing the LEDs data
    if(IO::ledsSize > 0){
      for(unsigned int i = 0; i<IO::ledsSize; i++){
        if (IO::getLeds()[i]->getValue() == 0)
        {
            continue;
        }
        else if (IO::getLeds()[i]->getValue() == 4095)
        {
            bitSet(ledsData, IO::getLeds()[i]->getIndex());
        }
        else if ((float)this->clockPWM / this->intervalPWM < (float)IO::getLeds()[i]->getValue() / 4095)
        {
            bitSet(ledsData, IO::getLeds()[i]->getIndex());
        }
      }
    }
    
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    // Send the byte to set the MUX and select the DAC
    SPI.transfer(shiftRegistersData);
    // Send the leds byte to complete the shift registers chain
    SPI.transfer(ledsData);
    SPI.endTransaction();

    // Set the latch to high (shift registers actually set their pins and stop listening)
    digitalWrite(REGISTERS_LATCH_PIN, HIGH);

    // Send the selected DAC data
    if(IO::outputsSize > 0){
      if (currentOutputDacChannel == 0)
      {
          // Channel A
          SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
          SPI.transfer16(0x1000 | constrain((int)IO::getOutputs()[this->currentOutputIndex]->getValue(), 0, 4095));
          SPI.endTransaction();
      }
      else
      {
          // Channel B
          SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
          SPI.transfer16(0x9000 | (int)IO::getOutputs()[this->currentOutputIndex]->getValue());
          SPI.endTransaction();
      }
    }

    // Set the latch to low (now shift registers are listening)
    digitalWrite(REGISTERS_LATCH_PIN, LOW);

    // Send the same data to the shift registers except the DAC selection bit goes to high (the DAC will actually set its pin)
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(shiftRegistersData ^ 0x01 << currentOutputDacIndex);
    SPI.transfer(ledsData);
    SPI.endTransaction();

    // Set the latch to high  (shift registers actually set their pins and stop listening)
    digitalWrite(REGISTERS_LATCH_PIN, HIGH);

    // Read the current input
    if(IO::inputsSize > this->currentInputIndex){
      IO::getInputs()[this->currentInputIndex]->read();
    }
}


//inline void IOManager::setSmoothing(byte smoothing){
//  for(APhysicalInput* i : IO::getInputs()){
//    i->setSmoothing(smoothing);
//  }
//  for(APhysicalOutput* o : IO::getOutputs()){
//    o->setSmoothing(smoothing);
//  }
//}


///**
// * Handle press down on a button
// */
//inline void IOManager::setHandleTriggerDown(byte index, TriggerDownCallback fptr)
//{
//    this->inputs[index]->setTriggerDownCallback(fptr);
//}
//
///**
// * Handle press up on a button
// */
//inline void IOManager::setHandleTriggerUp(byte index, TriggerUpCallback fptr)
//{
//    this->inputs[index]->setTriggerUpCallback(fptr);
//}
//
///**
// * Handle long trigger down
// */
//inline void IOManager::setHandleLongTriggerDown(byte index, LongTriggerDownCallback fptr)
//{
//    this->inputs[index]->setLongTriggerDownCallback(fptr);
//}
//
///**
// * Handle long trigger up
// */
//inline void IOManager::setHandleLongTriggerUp(byte index, LongTriggerUpCallback fptr)
//{
//    this->inputs[index]->setLongTriggerUpCallback(fptr);
//}
//
///**
// * Handle change
// */
//inline void IOManager::setHandleChange(byte index, ChangeCallback fptr)
//{
//    this->inputs[index]->setChangeCallback(fptr);
//}
//
///**
// * Handle potentiometer
// */
//inline void IOManager::setHandleChangeQuantized(byte index, ChangeQuantizedCallback fptr)
//{
//    this->inputs[index]->setChangeQuantizedCallback(fptr);
//}

//inline void IOManager::setOutputValue(byte index, unsigned int value)
//{
//    IO::getOutputs()[index]->setTarget(value);
//}

//inline void IOManager::setLED(byte index, Led::Status status, unsigned int brightness = 4095)
//{
//    this->leds[index]->set(status, brightness);
//}

/**
 * Get input value
 * @param byte index The index of the input
 */
inline float IOManager::getInputValue(byte index)
{
//  if(IO::getInputs().size() > 0 && index < IO::getInputs().size()){
//    return IO::getInputs()[index]->getValue();
//  }else{
    return 0;
//  }
}

/**
 * Get output value
 * @param byte index The index of the output
 */
//inline float IOManager::getOutputValue(byte index)
//{
//  if(IO::getOutputs().size() > 0 && index < IO::getOutputs().size()){
//    return IO::getOutputs()[index]->getValue();
//  }else{
//    return 0;
//  }
//}

inline byte IOManager::getMidiChannel()
{
    return this->midiChannel;
}

/**
 * Get min analog value according to resolution
 */
inline unsigned int IOManager::getAnalogMinValue()
{
    return 0;
}

/**
 * Get max analog value according to resolution
 */
inline unsigned int IOManager::getAnalogMaxValue()
{
    return (1 << this->analogResolution) - 1;
}

inline void IOManager::handleMidiControlChange(byte channel, byte controlNumber, byte value){
  //TODO: use the channel
  
//    for(APhysicalInput* i : IO::getInputs()){
//        if(i->getMidiControlNumber() == controlNumber){
//          int target = map(value, 0, 127, 0, 4095);
//          i->setTarget(target);
//        }
//    }
}


inline void IOManager::print(){
  Serial.print("Inputs: ");
  for(unsigned int i = 0; i< IO::inputsSize; i++){
    IO::getInputs()[i]->print();
    Serial.print(" / ");
  }
  Serial.println("");

  Serial.print("Outputs: ");
  for(unsigned int i = 0; i< IO::outputsSize; i++){
    IO::getOutputs()[i]->print();
    Serial.print(" / ");
  }

  Serial.println("");
  
  Serial.print("Leds: ");
  for(unsigned int i = 0; i< IO::ledsSize; i++){
    IO::getLeds()[i]->print();
    Serial.print(" / ");
  }
  
  Serial.println("");
}

#endif
