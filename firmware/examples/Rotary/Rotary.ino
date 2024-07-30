/*
Simple example with one rotary encoder controlling an LED and an oscillator.

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
#include "Motherboard/InputRotary.h"
#include "Motherboard/OutputLed.h"

// Input of type Rotary
InputRotary *input1;
// Led
OutputLed *led1;
// A sine oscillator
AudioSynthWaveformDc *dc1;
//
AudioSynthWaveformModulated *sine1;
// The AudioOutputI2S is required for the audio library's clock to tick
AudioOutputI2S i2s;

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(40);

  // Input on index 0
  input1 = new InputRotary(0);
  input1->onChange(changeCallback);
  input1->onPush(pushCallback);
  input1->onRelease(releaseCallback);

  // LED on index 0
  led1 = new OutputLed(0);

  // Setting up the DC object
  dc1 = new AudioSynthWaveformDc();
  dc1->amplitude(-1.0);

  // Setting up the sine1 oscillator
  sine1 = new AudioSynthWaveformModulated();
  sine1->begin(WAVEFORM_SINE);
  sine1->frequency(20);
  sine1->amplitude(1);

  // Connecting dc1's output to the led1's input
  new AudioConnection(*dc1, 0, *led1, 0);
  // Connecting sine1's output to the audio DAC's input
  new AudioConnection(*sine1, 0, i2s, 0);
}

void loop() {
}

/**
 * Gets called when turning the rotary.
 * Sets a DC object's value which is connected to the LED, so it changes the LED's brightness,
 * and sets the sine's frequency, which can be heard on the audio output.
 */
void changeCallback(uint16_t value) {
  float dcAmplitude = map((float)constrain(value, 0, 100), 0, 100, -1.0, 1.0);
  dc1->amplitude(dcAmplitude);
  uint16_t sineFrequency = map((float)constrain(value, 0, 100), 0, 100, 20, 2000);
  sine1->frequency(sineFrequency);
}

/**
 * Gets called when pushing on the rotary's button.
 * Sets the LED to blink.
 */
void pushCallback() {
  led1->setStatus(OutputLed::Status::Blink);
}

/**
 * Gets called when releasing the rotary's button
 * Sets the LED back to default, meaning it uses the value passed to its input.
 */
void releaseCallback() {
  led1->setStatus(OutputLed::Status::Default);
}