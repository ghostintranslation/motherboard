#ifndef io_h
#define io_h
class IO {
  public:
    virtual void onValueChange();

    void update(unsigned int updateMillis);
    byte getIndex();
    void setIndex(byte index);
    float getValue();
    int getTarget();
    void setTarget(int target);
    int getSmoothing();
    void setSmoothing(byte smoothing);

  private:
    // The index starting at 0
    byte index;

    // The previous value
    float previousValue;

    // The actual value
    float value;

    // The target value
    int target;

    // The smoothing factor
    float smoothing;
};

inline byte IO::getIndex(){
  return this->index;
}

inline void IO::setIndex(byte index){
    this->index = index;
}

inline float IO::getValue(){
  return this->value;
}

inline int IO::getTarget(){
  return this->target;
}

inline int IO::setTarget(int target){
  this->target = target;
}

inline unsigned int IO::getSmoothing(){
  return this->smoothing;
}

inline int IO::setSmoothing(byte smoothing){
  this->smoothing = map((float)smoothing, 0, 100, 1, 0.05);
}

inline void IO::update(unsigned int updateMillis){
  if(this->target != this->value){
    if(this->smoothing == 0){
      this->value = this->target;
    }else{
      this->value += (this->smoothing * (this->target - this->value) / 1024)  / (100 / (float)updateMillis);
    }
  }

    // Rounding the float to compare to 0, because otherwise it is actually never quite 0
    // and we don't need more than 2 decimal precision
    if(roundf(this->value * 100) / 100 == 0){
        this->value = 0;
    }

    // When value changes then call the callback.
    // Only carring about changes greater than 1.
    // This callback is defined in the Input class, and is empty in the Output class
    if(abs(this->value - this->previousValue) > 1){
        this->onValueChange();
    }
}
#endif