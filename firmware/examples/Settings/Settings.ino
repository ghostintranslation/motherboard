/*
Simple example with configurable MIDI input that controls an led.

MIDI CC 1 is the default. It can be changed using the web interface.

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
// Potentiometer input on index 0
Input *input1;
// Inputs on indexes 0 and 1
MidiCCInput *midiInput1;
MidiCCInput *midiInput2;
// Setting for the MIDI CC value
Setting *midiCCSetting1;
Setting *midiCCSetting2;
// The AudioOutputI2S is required for the audio library's clock to tick
AudioOutputI2S i2s1;

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(40);

  midiCCSetting1 = new Setting("midi-cc-led-1", "MIDI CC led 1", 1, 1, 127, 1);  
  midiCCSetting2 = new Setting("midi-cc-led-2", "MIDI CC led 2", 2, 1, 127, 1); 
  midiInput1 = new MidiCCInput(midiCCSetting1);
  midiInput2 = new MidiCCInput(midiCCSetting2);
  led1 = new OutputLed(0);
  led2 = new OutputLed(1);
  input1 = new Input(0);
  input1->setMidiInput(midiInput1);

  // Connecting input1's output to the frequency input of sine1
  new AudioConnection(*input1, 0, *led1, 0);
  new AudioConnection(*midiInput2, 0, *led2, 0);
}

void loop() {
}
