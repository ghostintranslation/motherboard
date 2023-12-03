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
#include "Motherboard/Led.h"

AudioOutputI2S i2s;
Input* input1;
Input* input2;
Input* input3;
Input* input4;
Input* input5;
Input* input6;
Led* led1;
Led* led2;
Led* led3;
Led* led4;
Led* led5;
Led* led6;

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(40);

  input1 = new Input(0);
  input2 = new Input(1);
  input3 = new Input(2);
  input4 = new Input(3);
  input5 = new Input(4);
  input6 = new Input(5);

  led1 = new Led(0);
  led1->setStatus(Led::Status::On);
  led2 = new Led(1);
  led2->setStatus(Led::Status::On);
  led3 = new Led(2);
  led3->setStatus(Led::Status::On);
  led4 = new Led(3);
  led4->setStatus(Led::Status::On);
  led5 = new Led(4);
  led5->setStatus(Led::Status::On);
  led6 = new Led(5);
  led6->setStatus(Led::Status::On);

  new AudioConnection(*input1, 0, *led1, 0);
  new AudioConnection(*input2, 0, *led2, 0);
  new AudioConnection(*input3, 0, *led3, 0);
  new AudioConnection(*input4, 0, *led4, 0);
  new AudioConnection(*input5, 0, *led5, 0);
  new AudioConnection(*input6, 0, *led6, 0);
}

void loop() {
}
