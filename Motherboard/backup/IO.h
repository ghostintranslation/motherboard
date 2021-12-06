#ifndef io_h
#define io_h

#include <vector>

class Input;
class Output;

class IO
{
public:
    IO(String name);
    
    virtual void onValueChange() {}

    virtual String getType() = 0;

    // Debug
    virtual void print();
    
    void update(unsigned int updateMillis);

    float getValue();
    int getTarget();
    void setTarget(int target);
    unsigned int getSmoothing();
    void setSmoothing(byte smoothing);

    // Registrar
    static void registerInput(Input* input);
    static void registerOutput(Output* output);
    static std::vector<Input*> getInputs();
    static std::vector<Output*> getOutputs();

private:
    static std::vector<Input*> inputs;
    static std::vector<Output*> outputs;

protected:
    // The previous value
    float previousValue = 0;

    // The actual value
    float value = 0;

    // The target value
    int target = 0;

    // The smoothing factor
    float smoothing = 5;

    float thresholdOnChange = 0;

    String name = "";
};

// Must initialize
std::vector<Input*> IO::inputs = std::vector<Input*>{};
std::vector<Output*> IO::outputs = std::vector<Output*>{};

inline IO::IO(String name){
  this->name = name;
}

inline float IO::getValue()
{
    return this->value;
}

inline int IO::getTarget()
{
    return this->target;
}

inline void IO::setTarget(int target)
{
    this->target = target;
}

inline unsigned int IO::getSmoothing()
{
    return this->smoothing; 
}

inline void IO::setSmoothing(byte smoothing)
{
    this->smoothing = map((float)smoothing, 0, 100, 1, 0.05);
}

inline void IO::update(unsigned int updateMillis)
{
    if (this->target != this->value)
    {
        if (this->smoothing == 1)
        {
            this->value = this->target;
        }
        else
        {
            this->value += (this->smoothing * (this->target - this->value) / 1024) / (100 / (float)updateMillis);
        }
    }

    // Rounding the float to compare to 0, because otherwise it is actually never quite 0
    // and we don't need more than 2 decimal precision
    if (abs(this->target) > 5 && roundf(this->value * 100) / 100 == 0)
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
}

inline void IO::registerInput(Input* input){
  inputs.push_back(input);
}

inline void IO::registerOutput(Output* output){
  outputs.push_back(output);
}

inline std::vector<Input*> IO::getInputs(){
  return inputs;
};

inline std::vector<Output*> IO::getOutputs(){
  return outputs;
};


inline void IO::print()
{
  Serial.print(this->name);
  Serial.print(": ");
  Serial.print(this->value);
}

#endif
