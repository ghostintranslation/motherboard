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
using PressDownCallback = void (*)(byte);
using LongPressDownCallback = void (*)(byte);
using PressUpCallback = void (*)(byte);
using LongPressUpCallback = void (*)(byte);
using ChangeCallback = void (*)(byte inputIndex, unsigned int value, int diff);

#include "Potentiometer.h"
#include "Button.h"

class IOManager{    
  public:
    // Init
    static IOManager *getInstance();
    static IOManager *init(byte columnNumber, std::initializer_list<Input> inputs, std::initializer_list<Output> outputs);
    
    // Update
    void update();

    byte getInputNumber();
    byte getOutputNumber();
    unsigned int getInputValue(byte index);
    void setOutput(byte index, byte value);
    void setLED(byte index, Status status, unsigned int brightness);
    unsigned int getAnalogMaxValue();
    unsigned int getAnalogMinValue();
    byte getMidiChannel();

    // Callbacks
    void setHandlePressDown(byte inputIndex, PressDownCallback fptr);
    void setHandleLongPressDown(byte inputIndex, LongPressDownCallback fptr);
    void setHandlePressUp(byte inputIndex, PressUpCallback fptr);
    void setHandleLongPressUp(byte inputIndex, LongPressUpCallback fptr);
    void setHandleChange(byte inputIndex, ChangeCallback fptr);
    
  private:
    // Singleton
    static IOManager * instance;
    IOManager();

    // Inputs/Outputs
    byte inputNumber;
    byte outputNumber;
    byte ledNumber;
    byte currentInputIndex;
    byte currentOutputIndex;
    Input *inputs;
    Output *outputs;
    Led *leds;
    byte analogResolution = 12;
    byte midiChannel;

    void iterateIO();
    void updateIO();
    void readWriteIO();

    // Refresh clock
    const unsigned int intervalRefresh = 2;
    elapsedMicros clockRefresh;
};


// Instance pre init
IOManager * IOManager::instance = nullptr;

/**
 * Constructor
 */
inline IOManager::IOManager(){
}

/**
 * Singleton instance
 */
inline IOManager * IOManager::getInstance() {
  if (!instance)
     instance = new IOManager;
  return instance;
}

inline IOManager * IOManager::init(byte columnNumber, std::initializer_list<Input> inputs, std::initializer_list<Output> outputs){
  Serial.println("init");

  byte inputNumber = inputs.size();
  getInstance()->inputNumber = inputNumber;
  
  // Init of the inputs
  getInstance()->inputs = new Input[inputNumber];
  
  byte i = 0;
  for(auto input : inputs){
    getInstance()->inputs[i] = input;
    i++;
  }

  // Init the outputs
  byte outputNumber = outputs.size();
  getInstance()->outputs = new Output[outputNumber];

  i = 0;
  for(auto output : outputs){
    getInstance()->outputs[i] = output;
    i++;
  }

  // Init of the leds
  this->ledNumber = columnNumber * 3
  getInstance()->leds = new Led[this->ledNumber];

  Serial.println("init2");
  
  // Pin modes
  pinMode(MIDI_CHANNEL_A_PIN, INPUT_PULLUP);
  pinMode(MIDI_CHANNEL_B_PIN, INPUT_PULLUP);
  pinMode(MIDI_CHANNEL_C_PIN, INPUT_PULLUP);
  pinMode(MIDI_CHANNEL_D_PIN, INPUT_PULLUP);
  pinMode(REGISTERS_LATCH_PIN, OUTPUT);
  pinMode(SPI_CLOCK_PIN, OUTPUT);
  pinMode(SPI_MOSI_PIN, OUTPUT);

  analogReadResolution(getInstance()->analogResolution);
   
  Serial.println("init3");
  return getInstance();
}

/**
 * Update
 */
inline void IOManager::update(){
  if (this->clockRefresh >= this->intervalRefresh) {
   // Iterate to the next input and output
   this->iterateIO();

   // Read the current input and set the current output
    this->readWriteIO();
    
   // Update all inputs and outputs
   this->updateIO();

   this->clockRefresh = 0;
  }
}

inline byte IOManager::getInputNumber(){
  return this->inputNumber;
}

inline byte IOManager::getOutputNumber(){
  return this->outputNumber;
}

/**
 * Iterate over the inputs
 */
inline void IOManager::iterateIO() {
    this->currentInputIndex++;
    while(typeid(inputs[this->currentInputIndex]) == "None" && this->currentInputIndex < this->inputNumber){
        this->currentInputIndex++;
    }
    this->currentInputIndex = this->currentInputIndex % this->inputNumber;

    this->currentOutputIndex++;
    while(typeid(this->outputs[this->currentOutputIndex]) == "None" && this->currentOutputIndex < this->inputNumber){
        this->currentOutputIndex++;
    }
    this->currentOutputIndex = this->currentOutputIndex % this->inputNumber;
}

/**
 * Send data to the chips to set the LEDs, the DAC, and read the inputs
 */
inline void IOManager::readWriteIO(){
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

  byte currentOutputDacIndex = this->currentOutputIndex / 2;
  byte currentOutputDacChannel = this->currentOutputIndex % 2;

    // Set the latch to low (activate the shift registers)
    digitalWrite(REGISTERS_LATCH_PIN, LOW);

    // Preparing the shift register data
    unsigned long shiftRegistersData;
    if(this->inputs[this->currentInputIndex]->needsGround()){
        shiftRegistersData = 0x80;
    }else{
        shiftRegistersData = 0x00;
    }
    shiftRegistersData = shiftRegistersData | this->currentInputIndex << 4 | 0x0F ^ 0x01 << currentOutputDacIndex;
    
    int ledsData = 0;
  
    // Preparing the LEDs data
    for(auto led : this->leds){
      if(led->getValue() == 0){
        continue;
      }else if(led->getValue() == 4095){
        bitSet(ledsData, led->getIndex());
      }else if((float)clockPWM/intervalPWM < (float)led->getValue()/4095){
        bitSet(ledsData, led->getIndex());
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
    // TODO: ONLY DO THAT IF OUTPUT IS NOT "NONE"
    if(currentOutputDacChannel==0){
        // Channel A
        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
        SPI.transfer16(0x1000 | this->outputs[this->currentOutputIndex]->getValue());
        SPI.endTransaction();
    }else{
        // Channel B
        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
        SPI.transfer16(0x9000 | this->outputs[this->currentOutputIndex]->getValue());
        SPI.endTransaction();
    }
    
    // Set the latch to low (now shift registers are listening)
    digitalWrite(REGISTERS_LATCH_PIN, LOW);

    // Send the same data to the shift registers except the DAC selection bit goes to high (the DAC will actually set its pin)
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(shiftRegistersData ^ 0x01 << currentOutputDacIndex );
    SPI.transfer(ledsData);
    SPI.endTransaction();

    // Set the latch to high  (shift registers actually set their pins and stop listening)
    digitalWrite(REGISTERS_LATCH_PIN, HIGH);
    
    // Read the current input
    this->inputs[this->currentInputIndex]->read();
}

inline void IOManager::updateIO(){
    for(auto input : this->inputs){
        input->update(this->intervalRefresh);
    }
    for(auto output : this->outputs){
        output->update(this->intervalRefresh);
    }
    for(auto led : this->leds){
        leds->update(this->intervalRefresh);
    }
}

/**
 * Handle press down on a button
 */
inline void IOManager::setHandlePressDown(byte index, PressDownCallback fptr){
    this->inputs[index]->setPressDownCallback(fptr);
}

/**
 * Handle press up on a button
 */
inline void IOManager::setHandlePressUp(byte index, PressUpCallback fptr){
    this->inputs[index]->setPressUpCallback(fptr);
}

/**
 * Handle long press down on a button
 */
inline void IOManager::setHandleLongPressDown(byte index, LongPressDownCallback fptr){
    this->inputs[index]->setLongPressDownCallback(fptr);
}

/**
 * Handle long press up on a button
 */
inline void IOManager::setHandleLongPressUp(byte index, LongPressUpCallback fptr){
    this->inputs[index]->setLongPressUpCallback(fptr);
}

/**
 * Handle potentiometer
 */
inline void IOManager::setHandleChange(byte index, ChangeCallback fptr){
    this->inputs[index]->setChangeCallback(fptr);
}


inline void IOManager::setOutput(byte index, unsigned int value){
    this->outputs[index]->setTarget(fptr);
}

inline void IOManager::setLED(byte index, Status status, unsigned int brightness = 4095){
    this->leds[index]->setTarget(brightness);
    this->leds[index]->setStatus(status);
}

/**
 * Get input value
 * @param byte index The index of the input
 */
inline int IOManager::getInputValue(byte index){
   return this->inputs[index]->getValue();
 }


inline byte IOManager::getMidiChannel(){ 
  return this->midiChannel;
}

/**
 * Get min analog value according to resolution
 */
inline unsigned int IOManager::getAnalogMinValue(){
  return 0;
}

/**
 * Get max analog value according to resolution
 */
inline unsigned int IOManager::getAnalogMaxValue(){
  return (1 << this->analogResolution) - 1;
}


// Instanciating
IOManager & IOManager = *IOManager::getInstance();
#endif