/*
Simple example with 6 inputs controlling 6 leds.

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

#include <Audio.h>
#include "Motherboard/Input.h"
#include "Motherboard/OutputLed.h"
#include "Motherboard/OutputTrigger.h"
#include "Motherboard/OutputGate.h"
#include "MyQuantizer.h"

AudioOutputI2S i2s;
AudioSynthWaveformSineModulated sine;
MyQuantizer* quantizer;
Input* input1;
Input* input2;
Input* input3;
Input* input4;
Input* input5;
Input* input6;
OutputLed led1(0);
OutputLed led2(1);
OutputLed led3(2);
OutputLed led4(3);
OutputLed led5(4);

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(40);

  sine.frequency(100);
  sine.amplitude(1);

  quantizer = new MyQuantizer();

  input1 = new Input(0);
  input2 = new Input(1);
  input3 = new Input(2);
  input4 = new Input(3);
  input5 = new Input(4);
  input6 = new Input(5);


  led2.setStatus(OutputLed::Status::Blink);
  led3.setStatus(OutputLed::Status::BlinkFast);
  led4.setStatus(OutputLed::Status::Blink);
  led4.setSmoothing(0.9999);
  led5.setStatus(OutputLed::Status::BlinkFast);
  led5.setSmoothing(0.999);

  new AudioConnection(*input5, 0, sine, 0);
  // new AudioConnection(sine, 0, *quantizer, 0);
  // new AudioConnection(*quantizer, 0, *led1, 0);
  new AudioConnection(sine, 0, led1, 0);
  // new AudioConnection(*input2, 0, *led2, 0);
  // new AudioConnection(*input3, 0, *led3, 0);
  // new AudioConnection(*input4, 0, *led4, 0);
  // // new AudioConnection(*input5, 0, *led5, 0);
  // new AudioConnection(*input6, 0, *led6, 0);
}

void loop() {
}
