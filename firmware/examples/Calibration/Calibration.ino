/*
Calibration helper.

On the Motherboard:
- connect a wire from 5v to IN 1
- connect a wire from 5v to IN 9

The inputs are patched to a sine oscillator each, outputting the signals
to the left and right channels. By sending 5v to the inputs the oscillator should
output a tone of 880Hz.

Turn the trim potentiometers to find where the tones get to their maximum.


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
// As of now the second ADC kicks in only if there is more than 8 inputs so we have to instantiate all of them
Input *input1;
Input *input2;
Input *input3;
Input *input4;
Input *input5;
Input *input6;
Input *input7;
Input *input8;

// Input of the second ADC
Input *input9;

// Sines to connect to the inputs
AudioSynthWaveformModulated *sine1;
AudioSynthWaveformModulated *sine2;

// The AudioOutputI2S is required for the audio library's clock to tick
AudioOutputI2S i2s;

void setup()
{
  // Audio connections require memory to work.
  AudioMemory(40);

  // Inputs of the first ADC
  input1 = new Input(0);
  input2 = new Input(1);
  input3 = new Input(2);
  input4 = new Input(3);
  input5 = new Input(4);
  input6 = new Input(5);
  input7 = new Input(6);
  input8 = new Input(7);
  // Input of the second ADC
  input9 = new Input(8);

  sine1 = new AudioSynthWaveformModulated();
  sine1->begin(WAVEFORM_SINE);
  sine1->frequencyModulation(5);
  sine1->frequency(27.5);
  sine1->amplitude(0.005);

  sine2 = new AudioSynthWaveformModulated();
  sine2->begin(WAVEFORM_SINE);
  sine2->frequencyModulation(5);
  sine2->frequency(27.5);
  sine2->amplitude(0.005);

  new AudioConnection(*input1, 0, *sine1, 0);
  new AudioConnection(*input9, 0, *sine2, 0);
  new AudioConnection(*sine1, 0, i2s, 0);
  new AudioConnection(*sine2, 0, i2s, 1);
}

void loop()
{
}