/*
 __    _  _____   ___      ___ _  _     __    _ ______ _    
/__|_|/ \(_  |     | |\|    | |_)|_||\|(_ |  |_| |  | / \|\|
\_|| |\_/__) |    _|_| |    | | \| || |__)|__| | | _|_\_/| |

MOTHERBOARD
2.0.0

Support my work:
https://www.paypal.com/paypalme/ghostintranslation

https://www.ghostintranslation.com/
https://ghostintranslation.bandcamp.com/
https://www.instagram.com/ghostintranslation/
https://www.youtube.com/channel/UCcyUTGTM-hGLIz4194Inxyw
https://github.com/ghostintranslation
*/

/**
 * This is an example sketch
 */


#include <Audio.h>
#include "Motherboard.h"

AudioOutputI2S           i2s1; 
//AudioSynthWaveformModulated* sine;

IO* io;

void setup()
{
  Serial.begin(115200);

  // Wait for serial monitor
  while (!Serial && millis() < 2500); 

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
//  audioboard.enable();
//  audioboard.volume(0.05); // caution: very loud - use oscilloscope only!
  
  
//  sine = new AudioSynthWaveformModulated();
//  sine->begin(WAVEFORM_SINE);
//  sine->frequency(0.5);
//  sine->amplitude(0.1);
  
  Led* led1 = new Led(0);
//  led1->setMidiCC(0); // CC number, the channel is the board one
//  Led* led2 = new Led(1);
//  led2->setMidiCC(0); // CC number, the channel is the board one


//  InputMidiNote* inputMidiNote1 = new InputMidiNote("inputMidiNote1", 1);
//  inputMidiNote1->setHandleMidiNoteOn(noteOnCallback);
//  inputMidiNote1->setHandleMidiNoteOff(noteOffCallback);
  


  InputPotentiometer* inputPot1= new InputPotentiometer(0, "inputPot1");
  
  InputJack* inputJack1 = new InputJack(6, "inputJack1");
  inputJack1->setType("Quantized");
  
//  AudioConnection* patchCord3 = new AudioConnection(*inputJack1, 0, *led1, 0);
//  AudioConnection* patchCord5 = new AudioConnection(*pot1, 0, *outputJack1, 0);
//  AudioConnection* patchCord4 = new AudioConnection(*inputJack1, 0, *led2, 0);

//  TODO: Virtual IOs
//  InputMidiCC* inputMidiCC = new InputMidiCC(0); // 0 = channel, the channel is the board one
//  inputMidiCC->setOnChange(callback);

//  OutputMidiNote* outputMidiNote = new OutputMidiNote(0);
//  outputMidiNote->setOnChange(callback);
//  outputMidiNote->sendNoteOn(note, velocity);
//  outputMidiNote->sendNoteOff(note, velocity);

//  OutputMidiCC* outputMidiCC = new OutputMidiCC(0);
//  outputMidiCC->setOnChange(callback);
//  outputMidiCC->set(value);



  Motherboard.setDebug(true);
  Motherboard.init("Motherboard", 2);


//  Motherboard.setHandleMidiNoteOn(noteOnCallback);
}

//elapsedMillis timeOut;
//int outValue = 0;
//int outValue2 = 0;
//
//
//const unsigned int intervalRefresh = 4;
//elapsedMicros clockRefresh;

void loop()
{

//  if (clockRefresh >= intervalRefresh){
//    b1->update(intervalRefresh);
//    b2->update(intervalRefresh);
//    b3->update(intervalRefresh);
//  }

//  Serial.print(pot1->getValue());
//  Serial.print(" ");
//  Serial.println(customStream1.getValue());
  Motherboard.update();

//  if(timeOut%100 == 0){
//    outValue++;
//    outValue = outValue % 4095;
//    outputJack2->setTarget(outValue);
//  }
//
//  if(timeOut%150 == 0){
//    outValue2++;
//    outValue2 = outValue2 % 4095;
//    out2->setTarget(outValue2);
//    out4->setTarget(outValue2);
//    out6->setTarget(outValue2);
//    out8->setTarget(outValue2);
//  }
}

void onChangePot1(byte inputIndex, float value, float diffToPrevious){
//    Serial.print(inputIndex);
//    Serial.print(" ");
//    Serial.println(value);
//  if(diffToPrevious>1){
//    Serial.print(0);
//    Serial.print(",");
//    Serial.print(4095);
//    Serial.print(",");
//    Serial.println(value);
//  }
//  int quantizedValue = map(value, 0, 4095, 0, 47);
//  
//  Serial.print("onChange ");
//  Serial.print(inputIndex);
//  Serial.print(" ");
//  Serial.print(value);
//  Serial.print(" ");
//  Serial.println(quantizedValue);

//  float mapped = map(value,0,4095,0,1000);
//  waveform1.frequency(mapped);

//  customStream1.setTarget(value);

//  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, value);
}

void onChangePot2(String name, float value, float diffToPrevious){
//  waveform1.amplitude(map(value,0,4095,0,0.05));
//amp.gain(map(value,0,4095,0,2));
//  sine->frequency(map(value,0,4095,0,20));
//Serial.println("onChangePot2");
}

void onChangeQuantized(byte inputIndex, int note){
  Serial.print("onChangeQuantized ");
  Serial.println(note);
}

void onGateOpen(String name){
  Serial.print("onGateOpen ");
  Serial.println(name);

//  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, 4095);
}

void onGateClose(String name){
  Serial.print("onGateClose ");
  Serial.println(name);
  
//  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, 0);
}

void noteOnCallback(byte channel, byte note, byte velocity){
  Serial.print("noteOnCallback: ");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(note);
  Serial.print(" ");
  Serial.println(velocity);
}
void noteOffCallback(byte channel, byte note, byte velocity){
  Serial.print("noteOffCallback: ");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(note);
  Serial.print(" ");
  Serial.println(velocity);
}
