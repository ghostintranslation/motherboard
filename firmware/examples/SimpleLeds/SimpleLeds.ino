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
#include "Motherboard/OutputLed.h"


OutputLed led1(0);
OutputLed led2(1);
OutputLed led3(2);
OutputLed led4(3);
AudioSynthWaveformModulated sine1;
AudioSynthWaveformModulated sine2;
AudioSynthWaveformModulated sine3;
AudioSynthWaveformModulated sine4;
AudioConnection patch1(sine1, 0, led1, 0);
AudioConnection patch2(sine2, 0, led2, 0);
AudioConnection patch3(sine3, 0, led3, 0);
AudioConnection patch4(sine4, 0, led4, 0);

// #include <Audio.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>

#include <synth_simple_drum.h>

// #include <Audio.h>
// #include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum2;          //xy=399,244
AudioSynthSimpleDrum     drum3;          //xy=424,310
AudioSynthSimpleDrum     drum1;          //xy=431,197
AudioSynthSimpleDrum     drum4;          //xy=464,374
AudioMixer4              mixer1;         //xy=737,265
AudioOutputI2S           i2s1;           //xy=979,214
AudioConnection          patchCord1(drum2, 0, mixer1, 1);
AudioConnection          patchCord2(drum3, 0, mixer1, 2);
AudioConnection          patchCord3(drum1, 0, mixer1, 0);
AudioConnection          patchCord4(drum4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=930,518
// GUItool: end automatically generated code

static uint32_t next;

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(200);

  sine1.begin(WAVEFORM_SINE);
  sine1.frequency(0.5);
  sine1.amplitude(1);
  sine2.begin(WAVEFORM_SINE);
  sine2.frequency(1);
  sine2.amplitude(1);
  sine3.begin(WAVEFORM_SINE);
  sine3.frequency(2);
  sine3.amplitude(1);
  sine4.begin(WAVEFORM_SINE);
  sine4.frequency(3);
  sine4.amplitude(1);

  next = millis() + 1000;

  AudioNoInterrupts();

  drum1.frequency(60);
  drum1.length(1500);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.55);

  drum2.frequency(60);
  drum2.length(300);
  drum2.secondMix(0.0);
  drum2.pitchMod(1.0);

  drum3.frequency(550);
  drum3.length(400);
  drum3.secondMix(1.0);
  drum3.pitchMod(0.5);

  drum4.frequency(1200);
  drum4.length(150);
  drum4.secondMix(0.0);
  drum4.pitchMod(0.0);


  sgtl5000_1.enable();
  sgtl5000_1.volume(1);

  AudioInterrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

  static uint32_t num = 0;

  if(millis() == next)
  {
    next = millis() + 1000;

    switch(num % 4)
    {
      case 0:
        drum1.noteOn();
        break;
      case 1:
        drum2.noteOn();
        break;
      case 2:
        drum3.noteOn();
        break;
      case 3:
        drum4.noteOn();
        break;
    }
    num++;

    Serial.print("Diagnostics: ");
    Serial.print(AudioProcessorUsageMax());
    Serial.print(" ");
    Serial.println(AudioMemoryUsageMax());
    AudioProcessorUsageMaxReset();
  }
}
