/*
Simple calibration helper.

On the Motherboard:
- connect a wire from 5v to IN 1
- connect a wire from 5v to IN 2.
- connect a LED on OUT 1 and a resistor to GND
- connect a LED on OUT 2 and a resistor to GND

The inputs are receiving the max voltage, and the LEDs will switch ON only when the max voltage is read.
To calibrate, adjust the rear trim potentiometers so that each LED switch on steadily.

Support my work:
https://patreon.com/ghostintranslation
https://www.buymeacoffee.com/ghostintranslation

Music and Merch:
https://ghostintranslation.bandcamp.com/

Social:
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/c/ghostintranslation

Code:
https://github.com/ghostintranslation

My website:
https://www.ghostintranslation.com/
*/

#include "Audio.h"
#include "Motherboard/Motherboard.h"

// Inputs of the first ADC
// Currently the second ADC is used only if there is more than 8 inputs
InputGate *input1;
InputGate *input2;
InputGate *input3;
InputGate *input4;
InputGate *input5;
InputGate *input6;
InputGate *input7;
InputGate *input8;
// Input of the second ADC
InputGate *input9;
// Led
OutputLed *led1;
OutputLed *led2;

// The AudioOutputI2S is required for the audio library's clock to tick
AudioOutputI2S i2s;

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ; // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(40);

  // Inputs of the first ADC
  input1 = new InputGate(0);
  input1->setThresholds(INT16_MAX, INT16_MAX - 1); // The signal will be high only when the max is reached
  input2 = new InputGate(1);
  input3 = new InputGate(2);
  input4 = new InputGate(3);
  input5 = new InputGate(4);
  input6 = new InputGate(5);
  input7 = new InputGate(6);
  input8 = new InputGate(7);
  // Input of the second ADC
  input9 = new InputGate(8);
  input9->setThresholds(INT16_MAX, INT16_MAX - 1); // The signal will be high only when the max is reached

  // LED on index 0 and 1
  led1 = new OutputLed(0);
  led1->setSmoothing(0.0);
  led2 = new OutputLed(1);
  led2->setSmoothing(0.0);

  new AudioConnection(*input1, 0, *led1, 0);
  new AudioConnection(*input9, 0, *led2, 0);
}

void loop()
{
}