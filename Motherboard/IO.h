#ifndef IO_h
#define IO_h

#include "AudioStream.h"
#include "IOTypeGate.h"
#include "IOTypeTrigger.h"
#include "IOTypeCV.h"
#include "IOState.h"

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
        float getSmoothing();
        void setSmoothing(float smoothing);
        IOType* getType();
        audio_block_t* receiveStream();
        audio_block_t* allocateStream();
        void transmitStream(audio_block_t* block);
        void releaseStream(audio_block_t* block);
        float getPreviousStreamValue();
        void setPreviousStreamValue(float previousStreamValue);
        unsigned int getUpdateMillis();
        float getPreviousValue();
        void setPreviousValue(float previousValue);
        float getThresholdOnChange();
        void setThresholdOnChange(float thresholdOnChange);
  
        // Callbacks  
        virtual void onValueChange();
        void setOnChange(ChangeCallback changeCallback);
        void setOnGateOpen(EdgeCallback gateOpenCallback);//onGateOn ?
        void setOnGateClose(EdgeCallback gateCloseCallback);//onGateOff ?

        // MIDI
        void setMidiCC(byte controlNumber);
        int getMidiCC();
        virtual void onMidiCC(unsigned int value);
        void setMidiMode(MidiMode mode);

        // State
        void transitionTo(IOState* state);
        
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
          
        
         elapsedMillis debounceTime = 0;
         unsigned int debounceDelay = 100;

         // MIDI
         int midiControlNumber = -1;

         unsigned int midiValue = 0;

         MidiMode midiMode = Either;

         // State
         IOState* state;
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

  this->state = nullptr;
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
//  if(this->calibrate){
//    this->target = target;
//    return;
//  }
  
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
  if(this->state != nullptr){
    this->state->update();
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

inline void IO::transitionTo(IOState* state){
  if (this->state != nullptr){
      delete this->state;
  }
  this->state = state;
  this->state->setIO(this);
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


inline float IO::getSmoothing(){
  return this->smoothing;
}

inline void IO::setSmoothing(float smoothing){
  this->smoothing = smoothing;
}

inline IOType* IO::getType(){
  return this->ioType;
}

inline audio_block_t* IO::receiveStream(){
  return this->receiveReadOnly(0);
}

inline audio_block_t* IO::allocateStream(){
  return this->allocate();
}

inline void IO::transmitStream(audio_block_t* block){
  return this->transmit(block, 0);
}

inline void IO::releaseStream(audio_block_t* block){
  return this->release(block);
}

inline float IO::getPreviousValue(){
  return this->previousValue;
}

inline void IO::setPreviousValue(float previousValue){
  this->previousValue = previousValue;
}

inline float IO::getPreviousStreamValue(){
  return this->previousStreamValue;
}

inline void IO::setPreviousStreamValue(float previousStreamValue){
  this->previousStreamValue = previousStreamValue;
}

inline unsigned int IO::getUpdateMillis(){
  return this->updateMillis;
}

inline float IO::getThresholdOnChange(){
  return this->thresholdOnChange;
}

inline void IO::setThresholdOnChange(float thresholdOnChange){
  this->thresholdOnChange = thresholdOnChange;
}

inline void IO::print()
{
  Serial.print(this->name);
  Serial.print(": ");
  Serial.printf("%07.2f", this->value);
}

#include "IOStateDefault.h"
#include "IOStateCalibrate.h"

#define MidiMode MotherboardNamespace::MidiMode

#endif
