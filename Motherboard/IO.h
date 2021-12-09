#ifndef IO_h
#define IO_h

#include "AudioStream.h"
#include "IOTypeGate.h"
#include "IOTypeTrigger.h"
#include "IOTypeCV.h"

// Forward declarations
class PhysicalInput;
class PhysicalOutput;
class Led;

enum MidiMode
{
  Either,
  Multiply,
  Add
};

//enum IOType
//{
//  Gate,
//  Trigger,
//  CV
//};


class IO : public AudioStream
{
public:
        IO(String name);
        
        virtual void update(void);

        void setCalibrate(bool calibrate);
        
        unsigned int getMin();
        
        unsigned int getMax();
        
        void setMin(unsigned int min);
        
        void setMax(unsigned int max);

        String getName();
        
        virtual String getClassName(){return "";};
    
        float getValue();

        virtual void setValue(float value);
        
        float getTarget();
        
        virtual void setTarget(float target);
        
        void updateTarget();

        void setType(String type);
  
        // Callbacks
        void setOnChange(ChangeCallback changeCallback);
    
        void setOnGateOpen(EdgeCallback gateOpenCallback);//onGateOn ?
    
        void setOnGateClose(EdgeCallback gateCloseCallback);//onGateOff ?

        // MIDI
        void setMidiCC(byte controlNumber);

        int getMidiCC();

        virtual void onMidiCC(unsigned int value);

        void setMidiMode(MidiMode mode);
        
    // Registrar
    static void registerInput(PhysicalInput* input);
    static void registerOutput(PhysicalOutput* output);
    static void registerLed(Led* led);
    static PhysicalInput** getInputs();
    static PhysicalOutput** getOutputs();
    static Led** getLeds();
    static unsigned int inputsSize;
    static unsigned int outputsSize;
    static unsigned int ledsSize;

    // Debug
    virtual void print();
    
private:
    audio_block_t *inputQueueArray[1];

    bool calibrate = false;
    
    IOType* ioType;
    
    // Regitrar
    static PhysicalInput** inputs;
    
    static PhysicalOutput** outputs;
    
    static Led** leds;
        
protected:      
        String name = "";

        unsigned int min = 0;
        
        unsigned int max = 4095;
        
        // The previous value
        float previousValue = 0;

        float previousStreamValue = 0;
        
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

         // MIDI
         int midiControlNumber = -1;

         unsigned int midiValue = 0;

         MidiMode midiMode = Either;
};


// Must initialize
unsigned int IO::inputsSize = 0;
PhysicalInput** IO::inputs = new PhysicalInput*[0];
    
unsigned int IO::outputsSize = 0;
PhysicalOutput** IO::outputs = new PhysicalOutput*[0];
    
unsigned int IO::ledsSize = 0;
Led** IO::leds = new Led*[0];


inline IO::IO(String name) : AudioStream(1, inputQueueArray)
{
  this->name = name;

  // Set to always active so they always update
  // and we can read their values and get callbacks even without patch cords connected
  this->active = true;

  this->ioType = new IOTypeCV();
}

inline String IO::getName()
{
    return this->name;
}

inline float IO::getValue()
{
    return this->value;
}

inline void IO::setValue(float value){
  this->value = this->ioType->processValue(value);
}

inline float IO::getTarget()
{
    return this->target;
}

inline void IO::setTarget(float target)
{
  if(this->calibrate){
    this->target = target;
    return;
  }
  
  this->target = this->ioType->processTarget(target);
  this->updateTarget();
}

inline void IO::updateTarget(){
  if(this->midiControlNumber > -1){
    switch(this->midiMode){
      case Multiply:
        this->target = this->target * map((float)this->midiValue,0,4095,0,1);
      break;

      case Add:
        this->target = constrain(this->target + this->midiValue,0,4095);
      break;
      
      case Either:
      default:
//      Serial.println(this->target);
//      Serial.println(this->midiValue);
//      Serial.println("");
//        this->target = max(this->target, this->midiValue);
      break;
    }
  }else{
    this->target = this->target;
  }
}

inline void IO::setType(String type){
  if(type == "Gate"){
    this->ioType = new IOTypeGate();
  }else if(type == "Trigger"){
    this->ioType = new IOTypeTrigger();
  }else{
    this->ioType = new IOTypeCV();
  }
}

inline void IO::setMidiMode(MidiMode mode){
  this->midiMode = mode;
}

inline void IO::update()
{ 
  if(this->calibrate){
    return;
  }
  
    // Maybe alter the smoothing
    this->smoothing = this->ioType->processSmoothing(this->smoothing);
    
    // Set the target from the input
    audio_block_t *block;
    uint32_t *p, *end;
    unsigned int streamValue = 0;
 
    block = receiveReadOnly(0);
    if (block){
      streamValue = constrain(block->data[0], 0, 4095);
      if(this->previousStreamValue != streamValue){
        this->setTarget(streamValue); // TODO: instead of using one value and dropping the 127 others, drop 1
        this->previousStreamValue  = streamValue;
      }
      release(block);
    }

    this->target = this->ioType->processTargetBeforeValueUpdate(this->target);
    
    // Update the value to reach the target
    if (this->target != this->value)
    {
        if (this->smoothing == 1)
        {
            this->setValue(this->target);
        }
        else
        {
            this->setValue(this->value + (this->smoothing * (this->target - this->value) / 1024) / (100 / this->updateMillis));
        }
    }

    // Rounding the float to compare to 0, because otherwise it is actually never quite 0
    // and we don't need more than 2 decimal precision
    this->setValue(roundf(this->value * 100) / 100);

    // To eliminate noise
    if (this->target > 5 && this->value == 0)
    {
        this->setValue(0);
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

inline void IO::onValueChange(){
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
inline void IO::setOnChange(ChangeCallback changeCallback)
{
    this->changeCallback = changeCallback;
}

/**
 * 
 */
inline void IO::setOnGateOpen(EdgeCallback gateOpenCallback)
{
    this->gateOpenCallback = gateOpenCallback;
}

/**
 * 
 */
inline void IO::setOnGateClose(EdgeCallback gateCloseCallback)
{
    this->gateCloseCallback = gateCloseCallback;
}

inline void IO::registerInput(PhysicalInput* input){
  PhysicalInput** newArr = new PhysicalInput*[inputsSize + 1];
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

inline void IO::registerOutput(PhysicalOutput* output){
  PhysicalOutput** newArr = new PhysicalOutput*[outputsSize + 1];
  std::copy(outputs, outputs + std::min(outputsSize, outputsSize + 1), newArr);
  delete[] outputs;
  outputs = newArr;

  outputs[outputsSize] = output;
  
  outputsSize++; 
}

inline void IO::registerLed(Led* led){
  Led** newArr = new Led*[ledsSize + 1];
  std::copy(leds, leds + std::min(ledsSize, ledsSize + 1), newArr);
  delete[] leds;
  leds = newArr;

  leds[ledsSize] = led;
  
  ledsSize++; 
}

inline PhysicalInput** IO::getInputs(){
  return inputs;
};

inline PhysicalOutput** IO::getOutputs(){
  return outputs;
};

inline Led** IO::getLeds(){
  return leds;
};

inline void IO::setMidiCC(byte controlNumber){
  this->midiControlNumber = controlNumber;
}

inline int IO::getMidiCC(){
  return this->midiControlNumber;
}

inline void IO::onMidiCC(unsigned int value){
  this->midiValue = this->ioType->processMidiCC(value);
  
  if(this->midiMode == Either){
    this->setTarget(this->midiValue);
  }else{
    this->updateTarget();
  }
}

inline void IO::setCalibrate(bool calibrate){
  this->calibrate = calibrate;
}

inline void IO::setMin(unsigned int min){
  this->min = min;
}

inline void IO::setMax(unsigned int max){
  this->max = max;
}

inline unsigned int IO::getMin(){
  return this->min;
}

inline unsigned int IO::getMax(){
  return this->max;
}

inline void IO::print()
{
  Serial.print(this->name);
  Serial.print(": ");
  Serial.printf("%07.2f", this->value);
}

#define MidiMode MotherboardNamespace::MidiMode

#endif
