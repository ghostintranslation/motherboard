/*
Simple example with inputs controlling leds.

One input controls an oscilator which in turns control an LED, and another input controls and LED directly.

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

// Leds on indexes 0 and 1
OutputLed *led1;
OutputLed *led2;
// Inputs on indexes 0 and 1
Input *input1;
Input *input2;
// A sine oscillator
AudioSynthWaveformModulated *sine1;
// The AudioOutputI2S is required for the audio library's clock to tick
AudioOutputI2S i2s1;

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(40);

  input1 = new Input(0);
  input2 = new Input(1);
  led1 = new OutputLed(0);
  led2 = new OutputLed(1);
  // Setting up the sine1 oscillator
  sine1 = new AudioSynthWaveformModulated();
  sine1->begin(WAVEFORM_SINE);
  sine1->frequency(0.5);
  sine1->amplitude(1);

  // Connecting input1's output to the frequency input of sine1
  new AudioConnection(*input1, 0, *sine1, 0);
  //Connecting sine1's output to led1's input
  new AudioConnection(*sine1, 0, *led1, 0);
  // Connecting input2's output to led2's input
  new AudioConnection(*input2, 0, *led2, 0);
}

void loop() {
}
