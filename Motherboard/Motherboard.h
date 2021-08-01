#ifndef Motherboard_h
#define Motherboard_h

#include <SPI.h>
#include <vector>

namespace MotherboardNamespace{
  
#include "ioManager.h"

/** 
 * Motherboard
 * v2.0.0
 */
class Motherboard
{

private:
  static Motherboard *instance;
  Motherboard();

  IOManager* ioManager;

public:
  static Motherboard *getInstance();
  void init(String deviceName, int columnNumber, std::vector<Input *> inputs, std::vector<Output *> outputs);
  void update();

  void setLED(byte index, Led::Status status, unsigned int brightness = 4095);
  void toggleLED(byte index);
  void resetAllLED();
  void initSequence();
  float getInputValue(byte index);
  int getAnalogMaxValue();
  int getAnalogMinValue();
  byte getMidiChannel();

  // Callbacks
  void setHandlePressDown(byte inputIndex, PressDownCallback fptr);
  void setHandleLongPressDown(byte inputIndex, LongPressDownCallback fptr);
  void setHandlePressUp(byte inputIndex, PressUpCallback fptr);
  void setHandleLongPressUp(byte inputIndex, LongPressUpCallback fptr);
  void setHandleChange(byte inputIndex, ChangeCallback fptr);
};

// Instance pre init
Motherboard *Motherboard::instance = nullptr;

/**
 * Constructor
 */
inline Motherboard::Motherboard()
{
}

/**
 * Singleton instance
 */
inline Motherboard *Motherboard::getInstance()
{
  if (!instance)
    instance = new Motherboard;
  return instance;
}

/**
 * Init
 */
inline void Motherboard::init(String deviceName, int columnNumber, std::vector<Input *> inputs, std::vector<Output *> outputs)
{
  this->ioManager = IOManager::getInstance();
  ioManager->init(columnNumber, inputs, outputs);
  //  ioManager->getMidiChannel();
  //
  //  // Init sequence
  //  this->initSequence();
}

/**
 * Update
 */
inline void Motherboard::update()
{
  ioManager->update();

  // Debug
  // if (this->clockDebug >= 100) {
  //   //    this->printInputs();
  //   //    this->printLeds();
  //   this->clockDebug = 0;
  // }
}

/**
 * Set a led status
 */
inline void Motherboard::setLED(byte index, Led::Status status, unsigned int brightness)
{
  ioManager->setLED(index, status, brightness);
}

/**
 * Toggle one LED
 */
inline void Motherboard::toggleLED(byte index)
{
  //   ioManager->toggleLed(index);
}

/**
 * Reset all LEDs
 */
inline void Motherboard::resetAllLED()
{
  //   ioManager->resetAllLED(index);
}

/**
 * Get input value
 * @param byte index The index of the input
 */
inline float Motherboard::getInputValue(byte index)
{
  return ioManager->getInputValue(index);
}

/**
 * Get max analog value according to resolution
 */
inline int Motherboard::getAnalogMinValue()
{
  return ioManager->getAnalogMinValue();
}

/**
 * Get max analog value according to resolution
 */
inline int Motherboard::getAnalogMaxValue()
{
    return ioManager->getAnalogMaxValue();
}

inline byte Motherboard::getMidiChannel()
{
  return ioManager->getMidiChannel();
}

/**
 * Set handle press down on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
inline void Motherboard::setHandlePressDown(byte inputIndex, PressDownCallback fptr)
{
  ioManager->setHandlePressDown(inputIndex, fptr);
}

/**
 * Set handle press up on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
inline void Motherboard::setHandlePressUp(byte inputIndex, PressUpCallback fptr)
{
  ioManager->setHandlePressUp(inputIndex, fptr);
}

/**
 * Set handle long press down on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
inline void Motherboard::setHandleLongPressDown(byte inputIndex, LongPressDownCallback fptr)
{
  ioManager->setHandleLongPressDown(inputIndex, fptr);
}

/**
 * Set handle long press up on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
inline void Motherboard::setHandleLongPressUp(byte inputIndex, LongPressUpCallback fptr)
{
  ioManager->setHandleLongPressUp(inputIndex, fptr);
}

/**
 * Set handle change
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex, float value, int diffToPrevious)
 */
inline void Motherboard::setHandleChange(byte inputIndex, ChangeCallback fptr)
{
  ioManager->setHandleChange(inputIndex, fptr);
}

/**
 * LEDs init sequence
 */
inline void Motherboard::initSequence()
{
}

}
// Instanciating
MotherboardNamespace::Motherboard &Motherboard = *MotherboardNamespace::Motherboard::getInstance();

#endif
