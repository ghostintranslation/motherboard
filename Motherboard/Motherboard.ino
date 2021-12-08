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
//InputPotentiometer pot1(0, "pot1");
//InputPotentiometer pot2(1, "pot2");
//InputPotentiometer pot3(2, "pot3");
//InputPotentiometer pot4(3, "pot4");
//InputPotentiometer pot5(4, "pot5");
//InputPotentiometer pot6(5, "pot6");
//OutputDac dac1(0, "dac1");
//Led led1(0);
//Led led2(1);
//AudioSynthWaveformModulated sine;
//AudioMixer4 mixer;
//AudioConnection     patchCord1(pot1, 0, mixer, 0);
//AudioConnection     patchCord2(pot2, 0, mixer, 1);
//AudioConnection     patchCord3(pot3, 0, mixer, 2);
//AudioConnection     patchCord4(pot4, 0, mixer, 3);
//AudioConnection     patchCord5(mixer, 0, dac1, 0);
//AudioConnection     patchCord6(mixer, 0, led1, 0);
//AudioConnection     patchCord7(sine, 0, led2, 0);

//AudioSynthWaveformModulated sine;
//AudioConnection     patchCord1(pot1, 0, amp, 0);
//AudioConnection     patchCord2(amp, 0, sine, 0);
//AudioConnection     patchCord3(pot2, 0, sine, 1);
//AudioConnection     patchCord3(sine, 0, dac1, 0);
//AudioConnection     patchCord4(sine, 0, i2s1, 0);
//AudioConnection     patchCord5(sine, 0, i2s1, 1);
//AudioControlSGTL5000     audioboard;

//AudioSynthWaveform       waveform1;      //xy=171,84
//AudioSynthWaveform       waveform2;      //xy=178,148
//AudioOutputI2S           i2s1;           //xy=360,98
//AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
//AudioConnection          patchCord2(waveform2, 0, i2s1, 1);
//AudioControlSGTL5000     audioboard;     //xy=239,232
AudioSynthWaveformModulated* sine;
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
  
//  TouchPad* pad1 = new TouchPad();
//  TouchPad* pad2 = new TouchPad();
//  TouchPad* pad3 = new TouchPad();

//Button* but1 = new Button();

  
  sine = new AudioSynthWaveformModulated();
  sine->begin(WAVEFORM_SINE);
  sine->frequency(0.5);
  sine->amplitude(0.1);
  
  Led* led1 = new Led(0);
//  led1->setMidiCC(0); // CC number, the channel is the board one
  Led* led2 = new Led(1);
//  led2->setMidiCC(0); // CC number, the channel is the board one

//  OutputCV* outputCV1 = new OutputCV(0, "outputCV1");
//  outputCV1->setMidiCC(2); // CC number, the channel is the board one
//  outputCV1->setMidiMode(MidiMode::Multiply);
  
//  OutputGate* outputGate1 = new OutputGate(0, "outputGate1");
//  outputGate1->setMidiCC(3); // CC number, the channel is the board one
//  outputGate1->setMidiMode(MidiMode::Multiply);

//  OutputTrigger* outputTrigger1 = new OutputTrigger(0, "outputTrigger1");
//  outputTrigger1->setMidiCC(1); // CC number, the channel is the board one
  
  InputPotentiometer* pot1 = new InputPotentiometer(0, "pot1");
pot1->setType("Gate");
//  pot1->setMidiCC(1); // CC number, the channel is the board one
//  pot1->setMidiMode(MidiMode::Multiply);

  InputPotentiometer* pot2 = new InputPotentiometer(1, "pot2");
pot2->setType("Trigger");
//  pot2->setOnChange(onChangePot2);

OutputJack* outputJack1 = new OutputJack(0, "outputJack1");
OutputJack* outputJack2 = new OutputJack(1, "outputJack1");

  AudioConnection* patchCord1 = new AudioConnection(*pot1, 0, *outputJack1, 0);
  AudioConnection* patchCord2 = new AudioConnection(*pot2, 0, *outputJack2, 0);
  AudioConnection* patchCord3 = new AudioConnection(*outputJack1, 0, *led1, 0);
  AudioConnection* patchCord4 = new AudioConnection(*outputJack2, 0, *led2, 0);

//  TODO: Virtual IOs
//  InputMidiNote* inputMidiNote = new InputMidiNote(0); // 0 = channel
//  inputMidiNote->onNoteOn(callback);
//  inputMidiNote->onNoteOff(callback);
//  inputMidiNote->setOnChange(callback);

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

elapsedMillis timeOut;
int outValue = 0;
int outValue2 = 0;


const unsigned int intervalRefresh = 4;
elapsedMicros clockRefresh;

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


//  if(timeOut%50 == 0){
//    outValue++;
//    outValue = outValue % 4095;
//    out->setTarget(outValue);
//    out3->setTarget(outValue);
//    out5->setTarget(outValue);
//    out7->setTarget(outValue);
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
  sine->frequency(map(value,0,4095,0,20));
//Serial.println("onChangePot2");
}
void onChangePot3(byte inputIndex, float value, float diffToPrevious){
//  float mapped = map(value,0,4095,0,1000);
//  waveform2.frequency(mapped);
//  Motherboard.setLED(1, MotherboardNamespace::Led::Status::On, value);
}
void onChangePot4(byte inputIndex, float value, float diffToPrevious){
//  float mapped = map(value,0,4095,0,0.05);
//  Serial.println(mapped);
//  waveform2.amplitude(mapped);
}

void onVelocityChange(byte inputIndex, float value, float diffToPrevious){
//  float gainValue = map(value, 0, 4095, 0, 1);
//  getInstance()->voices[0]->getOutput()->gain(1, gainValue);
//  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, value);
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
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(note);
  Serial.print(" ");
  Serial.println(velocity);
}
