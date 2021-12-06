#ifndef AIO_h
#define AIO_h

#include "AudioStream.h"

// Forward declarations
class APhysicalInput;
class APhysicalOutput;
class ALed;

class AIO : public AudioStream
{
public:
        AIO(String name);
        
        virtual void update(void);

        String getName();
        
        String getType();
    
        float getValue();
        
        float getTarget();
        
        virtual void setTarget(float target);

        void setOnChange(ChangeCallback changeCallback);
    
        void setOnGateOpen(EdgeCallback gateOpenCallback);//onGateOn ?
    
        void setOnGateClose(EdgeCallback gateCloseCallback);//onGateOff ?
        
    virtual void print();
        
    // Registrar
    static void registerInput(APhysicalInput* input);
    static void registerOutput(APhysicalOutput* output);
    static void registerLed(ALed* led);
    static APhysicalInput** getInputs();
    static APhysicalOutput** getOutputs();
    static ALed** getLeds();
    static unsigned int inputsSize;
    static unsigned int outputsSize;
    static unsigned int ledsSize;
    
private:
    audio_block_t *inputQueueArray[1];
        
    static APhysicalInput** inputs;
    
    static APhysicalOutput** outputs;
    
    static ALed** leds;
        
protected:      
        String name = "";
        
        String type = "";
         
        // The previous value
        float previousValue = 0;
        
        // The actual value
        float value = 0;
    
        // The target value
        float target = 0;
        
        float thresholdOnChange = 0;
        
        // The smoothing factor
        float smoothing = 2000;

        float updateMillis = 1/AUDIO_SAMPLE_RATE*1000*128;

         bool isGateOpen = false;
         
         // Change callback function
         ChangeCallback changeCallback = nullptr;
      
         EdgeCallback gateOpenCallback = nullptr;
      
         EdgeCallback gateCloseCallback = nullptr;
          
        virtual void onValueChange();
        
         elapsedMillis debounceTime = 0;
         unsigned int debounceDelay = 100;
         
};


// Must initialize
unsigned int AIO::inputsSize = 0;
APhysicalInput** AIO::inputs = new APhysicalInput*[0];
    
unsigned int AIO::outputsSize = 0;
APhysicalOutput** AIO::outputs = new APhysicalOutput*[0];
    
unsigned int AIO::ledsSize = 0;
ALed** AIO::leds = new ALed*[0];


inline AIO::AIO(String name) : AudioStream(1, inputQueueArray)
{
  this->name = name;

  // Set to always active so they always update
  // and we can read their values and get callbacks even without patch cords connected
  this->active = true;
}

inline String AIO::getName()
{
    return this->name;
}

inline String AIO::getType()
{
    return this->type;
}

inline float AIO::getValue()
{
    return this->value;
}

inline float AIO::getTarget()
{
    return this->target;
}

inline void AIO::setTarget(float target)
{
    this->target = target;
}

inline void AIO::update()
{ 
    // Set the target from the input
    audio_block_t *block;
    uint32_t *p, *end;
 
    block = receiveReadOnly(0);
    if (block){
      this->setTarget(constrain(block->data[0], 0, 4095)); // TODO: instead of using one value and dropping the 127 others, drop 1
      release(block);
    }

    // Update the value to reach the target
    if (this->target != this->value)
    {
        if (this->smoothing == 1)
        {
            this->value = this->target;
        }
        else
        {
            this->value += (this->smoothing * (this->target - this->value) / 1024) / (100 / this->updateMillis);
        }
    }

    // Rounding the float to compare to 0, because otherwise it is actually never quite 0
    // and we don't need more than 2 decimal precision
    this->value = roundf(this->value * 100) / 100;

    // To eliminate noise
    if (this->target > 5 && this->value == 0)
    {
        this->value = 0;
    }

    // When value changes then call the callback.
    // Only carring about changes greater than 1.
    // This callback is defined in the Input class, and is empty in the Output class
    if (abs(this->value - this->previousValue) > this->thresholdOnChange)
    {
        this->onValueChange();
    }

    this->previousValue = this->value;

    block = allocate();
    if (block){
      p = (uint32_t *)(block->data);
      end = p + AUDIO_BLOCK_SAMPLES/2;
      do {
        *p++ = this->value;
        *p++ = this->value;
        *p++ = this->value;
        *p++ = this->value;
        *p++ = this->value;
        *p++ = this->value;
        *p++ = this->value;
        *p++ = this->value;
      } while (p < end);
      transmit(block, 0);
      release(block);
    }
}

inline void AIO::onValueChange(){
  if (this->changeCallback != nullptr)
  {
    this->changeCallback(this->name, this->value, this->value - this->previousValue);
  }

  // Debouncing
  if(this->debounceTime < this->debounceDelay){
    return;
  }

  if((this->value > 4095 / 1.5) && !this->isGateOpen){
    if (this->gateOpenCallback != nullptr){
      this->gateOpenCallback(this->name);
    }
    this->isGateOpen = true;
    this->debounceTime = 0;
  }else if((this->value < 4095 / 4) && this->isGateOpen){
    if (this->gateCloseCallback != nullptr){
      this->gateCloseCallback(this->name);
    }
    this->isGateOpen = false;
    this->debounceTime = 0;
  }
}

/**
 * Set the callback function to call when the value changes
 */
inline void AIO::setOnChange(ChangeCallback changeCallback)
{
    this->changeCallback = changeCallback;
}

/**
 * 
 */
inline void AIO::setOnGateOpen(EdgeCallback gateOpenCallback)
{
    this->gateOpenCallback = gateOpenCallback;
}

/**
 * 
 */
inline void AIO::setOnGateClose(EdgeCallback gateCloseCallback)
{
    this->gateCloseCallback = gateCloseCallback;
}

inline void AIO::registerInput(APhysicalInput* input){
  APhysicalInput** newArr = new APhysicalInput*[inputsSize + 1];
  std::copy(inputs, inputs + std::min(inputsSize, inputsSize + 1), newArr); // TODO: Not sure if this is better than the loop
//  for (int i = 0; i < inputsSize; i++)
//  {
//      newArr[i] = inputs[i];
//  }
  delete []inputs;
  inputs = newArr;
  newArr = NULL;

  inputs[inputsSize] = input;
  inputsSize++;
}

inline void AIO::registerOutput(APhysicalOutput* output){
  APhysicalOutput** newArr = new APhysicalOutput*[outputsSize + 1];
  std::copy(outputs, outputs + std::min(outputsSize, outputsSize + 1), newArr);
  delete[] outputs;
  outputs = newArr;

  outputs[outputsSize] = output;
  
  outputsSize++; 
}

inline void AIO::registerLed(ALed* led){
  ALed** newArr = new ALed*[ledsSize + 1];
  std::copy(leds, leds + std::min(ledsSize, ledsSize + 1), newArr);
  delete[] leds;
  leds = newArr;

  leds[ledsSize] = led;
  
  ledsSize++; 
}

inline APhysicalInput** AIO::getInputs(){
  return inputs;
};

inline APhysicalOutput** AIO::getOutputs(){
  return outputs;
};

inline ALed** AIO::getLeds(){
  return leds;
};

inline void AIO::print()
{
  Serial.print(this->name);
  Serial.print(": ");
  Serial.printf("%07.2f", this->value);
}

//#define AIO MotherboardNamespace::AIO

#endif
