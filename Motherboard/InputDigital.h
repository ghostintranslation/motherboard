//#ifndef InputDigital_h
//#define InputDigital_h
//
//#include "Input.h"
//
//class InputDigital : public Input
//{
//public:
//    void read() override;
//    bool needsGround() override;
//    String getType() override;
//
//protected:
//    void setTriggerDownCallback(TriggerDownCallback TriggerDownCallback);
//    void setLongTriggerDownCallback(LongTriggerDownCallback longTriggerDownCallback);
//    void setTriggerUpCallback(TriggerUpCallback TriggerUpCallback);
//    void setLongTriggerUpCallback(LongTriggerUpCallback longTriggerUpCallback);
//
//private:
//    // Time counter for long vs short press
//    elapsedMillis pressTime;
//    
//    elapsedMillis debounceTime;
//    unsigned int debounceDelay = 5;
//
//    // Has the long press down callback been fired already?
//    bool longPressDownFired;
//
//    // Callbacks functions
//    TriggerDownCallback triggerDownCallback;
//    LongTriggerDownCallback longTriggerDownCallback;
//    TriggerUpCallback triggerUpCallback;
//    LongTriggerUpCallback longTriggerUpCallback;
//};
//
//inline void InputDigital::read()
//{
//    // Debouncing
//    if(this->debounceTime < this->debounceDelay){
//      return;
//    }
//
//    pinMode(this->pin, INPUT_PULLUP);
//    int newReading = !digitalRead(this->pin);
//
//    if (this->triggerDownCallback != nullptr ||
//        this->triggerUpCallback != nullptr ||
//        this->longTriggerDownCallback != nullptr ||
//        this->longTriggerUpCallback != nullptr){
//        this->debounceTime = 0;
//
//        // Inverted logic, 0 = InputDigital pushed
//        // If previous value is not pushed and now is pushed
//        // So if it's pushed
//        if (this->value && !newReading)
//        {
//            // Start the counter of that input
//            this->pressTime = 0;
//            this->longPressDownFired = false;
//
//            // If there is a short press down callback on that input, and there is no Long Press down
//            if (this->longTriggerDownCallback == nullptr &&
//                this->triggerDownCallback != nullptr)
//            {
//                this->triggerDownCallback(this->index);
//            }
//        }
//
//        // If it stayed pressed for 200ms and Long Press Down callback hasn't been fired yet
//        if (!this->value && !newReading)
//        {
//            if (this->pressTime >= 200 && !this->longPressDownFired)
//            {
//
//                if (this->longTriggerDownCallback != nullptr)
//                {
//                    // Fire the callback
//                    this->longTriggerDownCallback(this->index);
//                    this->longPressDownFired = true;
//                }
//            }
//        }
//
//        // If it's released
//        if (!this->value && newReading)
//        {
//            // How long was it pressed
//            if (this->pressTime < 200)
//            {
//                // Short press
//
//                // If there is a short press callback on that input
//                if (this->triggerUpCallback != nullptr)
//                {
//                    this->triggerUpCallback(this->index);
//                }
//            }
//            else
//            {
//                // Long press
//
//                // If there is a long press callback on that input
//                if (this->longTriggerUpCallback != nullptr)
//                {
//                    this->longTriggerUpCallback(this->index);
//                }
//                else if (this->triggerUpCallback != nullptr)
//                {
//                    // If the input was pressed for a long time but there is only a short press callback
//                    // the short press callback should still be called
//                    this->triggerUpCallback(this->index);
//                }
//            }
//        }
//    }
//
//    // In the case of a digital input, there is no in-between values, so the target and the actual value are equal
//    this->target = newReading;
//    this->value = newReading;
//}
//
//inline bool InputDigital::needsGround()
//{
//    return false;
//}
//
///**
// * Set the press down callback
// */
//inline void InputDigital::setTriggerDownCallback(TriggerDownCallback fptr)
//{
//    this->triggerDownCallback = fptr;
//}
//
///**
// * Set the long press down callback
// */
//inline void InputDigital::setLongTriggerDownCallback(LongTriggerDownCallback fptr)
//{
//    this->longTriggerDownCallback = fptr;
//}
//
///**
// * Set the press up callback
// */
//inline void InputDigital::setTriggerUpCallback(TriggerUpCallback fptr)
//{
//    this->triggerUpCallback = fptr;
//}
//
///**
// * Set the long press up callback
// */
//inline void InputDigital::setLongTriggerUpCallback(LongTriggerUpCallback fptr)
//{
//    this->longTriggerUpCallback = fptr;
//}
//
//inline String InputDigital::getType()
//{
//    return "InputDigital";
//}
//
//#endif
