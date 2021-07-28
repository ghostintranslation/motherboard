#include "Input.h"

class Potentiometer : public Input {
  public:
    void read() override;
    bool needsGround() override;
};

inline void Potentiometer::read(){
//   Serial.print("Potentiometer: ");
//   Serial.println(this->needsGround());

    pinMode(inputPin, INPUT);
    unsigned int val = analogRead(inputPin);
    val = map(constrain(val,8,4095), 8, 4095, 0, 4095); // Potentiometers

    this->setTarget(val);
}

inline void Potentiometer::needsGround(){
      return true;
}

// From now on "Potentiometer" will be replaced by "new Potentiometer()"
#define Potentiometer new Potentiometer()