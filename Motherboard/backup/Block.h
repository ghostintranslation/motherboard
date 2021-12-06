#ifndef Block_h
#define Block_h

#include <vector>

class Block
{
public:
    enum Relation
    {
      AVG,
      SUM
    };
    
    Block(String name);
    
    String getName();
    
    float getValue();
    
    float getTarget();
    
    void setTarget(int target);
    
    void updateTarget();
    
    void update(unsigned int updateMillis);
    
    void setSmoothing(byte smoothing);
    
    virtual void onValueChange() {}

    void addInput(Block* input);
    
    void addOutput(Block* output);

protected:
    std::vector<Block*> inputs;
    
    std::vector<Block*> outputs;
    
    String name = "";
    
    // The previous value
    float previousValue = 0;
    
    // The actual value
    float value = 0;

    // The target value
    float target = 0;
    
    float thresholdOnChange = 0;
    
    // The smoothing factor
    float smoothing = 5;

    Relation inputsRelation = AVG;
};


inline Block::Block(String name)
{
  this->name = name;
  this->inputs = std::vector<Block*>{};
  this->outputs = std::vector<Block*>{};
}

inline String Block::getName()
{
    return this->name;
}

inline float Block::getValue()
{
    return this->value;
}

inline float Block::getTarget()
{
    return this->target;
}

inline void Block::setTarget(int target)
{
    this->target = target;
}

/**
 * Update target according to all inputs
 */
inline void Block::updateTarget()
{
    float target = 0;

    switch(this->inputsRelation){
      case AVG:
      default:
        for(Block* b : this->inputs){
          target += b->getValue();
        }
    
        target = target / this->inputs.size();
      break;
      
      case SUM:
        for(Block* b : this->inputs){
          target += b->getValue();
        }
      break;
    };

    this->target = target;
}


inline void Block::setSmoothing(byte smoothing)
{
    this->smoothing = map((float)smoothing, 0, 100, 1, 0.05);
}

inline void Block::update(unsigned int updateMillis)
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
    // and we don't need more than 2 decimal precisBlockn
    if (abs(this->target) > 5 && roundf(this->value * 100) / 100 == 0)
    {
        this->value = 0;
    }

    // When value changes then call the callback.
    // Only carring about changes greater than 1.
    // This callback is defined in the Input class, and is empty in the Output class
    if (abs(this->value - this->previousValue) > this->thresholdOnChange)
    {
        // To be defined by derived classes
        this->onValueChange();

        // Tells the connected blocks to reassess their target because their is a new value
        for(Block* b : this->outputs){
          b->updateTarget();
        }
    }

    this->previousValue = this->value;
}


inline void Block::addInput(Block* input){
  inputs.push_back(input);
}

inline void Block::addOutput(Block* output){
  outputs.push_back(output);
}

#endif
