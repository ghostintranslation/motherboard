#ifndef Button_h
#define Button_h

#include "Input.h"

class Button : public Input
{
public:
    void read() override;

    String getType() override;
    
    // On value change callback
    void onValueChange() override;
  
    void setOnPressStart(EdgeCallback pressStartCallback);
    
    void setOnLongPressStart(EdgeCallback longPressStartCallback);
    
    void setOnPressEnd(EdgeCallback pressEndCallback);
    
    void setOnLongPressEnd(EdgeCallback longPressEndCallback);

private:
    // Press start callback
    EdgeCallback pressStartCallback;
    
    // Long press start callback
    EdgeCallback longPressStartCallback;
    
    // Press end callback
    EdgeCallback pressEndCallback;
    
    // Long press end callback
    EdgeCallback longPressEndCallback;

    // How long is a long press in ms
    unsigned int longPressDuration = 200;
    
    // Has the long press start callback been fired already?
    bool longPressStartFired;
    
    // Time counter for long vs short press
    elapsedMillis pressTime;
};


inline void Button::read()
{
    // Debouncing
    if(this->debounceTime < this->debounceDelay){
      return;
    }

    pinMode(this->pin, INPUT_PULLDOWN);
//    int newReading = digitalRead(this->pin);
    int newReading = map(constrain(analogRead(this->pin), 0, 1000), 0, 1000, 0, 1);
    

    // In the case of a digital input, there is no in-between values, so the target and the actual value are equal
    this->target = newReading;
    this->value = newReading;
}

inline String Button::getType()
{
  return "Button";
}

inline void Button::setOnPressStart(EdgeCallback pressStartCallback){
  this->pressStartCallback = pressStartCallback;
}

inline void Button::setOnLongPressStart(EdgeCallback longPressStartCallback){
  this->longPressStartCallback = longPressStartCallback;
}

inline void Button::setOnPressEnd(EdgeCallback pressEndCallback){
  this->pressEndCallback = pressEndCallback;
}

inline void Button::setOnLongPressEnd(EdgeCallback longPressEndCallback){
  this->longPressEndCallback = longPressEndCallback;
}

/**
 * On value change call the callback.
 * This function is called in the IO class.
 */
inline void Button::onValueChange()
{
  Input::onValueChange();

  if (this->pressStartCallback != nullptr ||
      this->pressEndCallback != nullptr ||
      this->longPressStartCallback != nullptr ||
      this->longPressEndCallback != nullptr){
      this->debounceTime = 0;

      // If it's pressed
      if (this->value && !this->previousValue)
      {
          // Start the counter of that input
          this->pressTime = 0;
          this->longPressStartFired = false;

          // If there is a short press down callback on that input, and there is no Long Press down
          if (this->longPressStartCallback == nullptr &&
              this->pressStartCallback != nullptr)
          {
              this->pressStartCallback(this->index);
          }
      }

      // If it stayed pressed for some time and Long Press Start callback hasn't been fired yet
      if (this->value && this->previousValue)
      {
          if (this->pressTime >= this->longPressDuration && !this->longPressStartFired)
          {

              if (this->longPressStartCallback != nullptr)
              {
                  // Fire the callback
                  this->longPressStartCallback(this->index);
                  this->longPressStartFired = true;
              }
          }
      }

      // If it's released
      if (!this->value && this->previousValue)
      {
          // How long was it pressed
          if (this->pressTime < this->longPressDuration)
          {
              // Short press

              // If there is a short press callback on that input
              if (this->pressEndCallback != nullptr)
              {
                  this->pressEndCallback(this->index);
              }
          }
          else
          {
              // Long press

              // If there is a long press callback on that input
              if (this->longPressEndCallback != nullptr)
              {
                  this->longPressEndCallback(this->index);
              }
              else if (this->pressEndCallback != nullptr)
              {
                  // If the input was pressed for a long time but there is only a short press callback
                  // the short press callback should still be called
                  this->pressEndCallback(this->index);
              }
          }
      }
  }
  
}

// From now on "Button" will be replaced by "new Button()"
//#define Button new MotherboardNamespace::Button()
#define Button MotherboardNamespace::Button

#endif
