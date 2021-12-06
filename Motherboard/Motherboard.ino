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
//APotentiometer pot1(0, "pot1");
//APotentiometer pot2(1, "pot2");
//APotentiometer pot3(2, "pot3");
//APotentiometer pot4(3, "pot4");
//APotentiometer pot5(4, "pot5");
//APotentiometer pot6(5, "pot6");
//ADac dac1(0, "dac1");
//ALed led1(0);
//ALed led2(1);
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
  
  ALed* led1 = new ALed(0);
//  led1->setOnGateOpen(onGateOpen);
//  led1->setOnGateClose(onGateClose);

  ALed* led2 = new ALed(1);
//  led2->setStatus(ALed::Status::BlinkFast);

  ADac* dac1 = new ADac(0, "dac1");
//  dac1->setOnChange(onChangePot2);
  
  APotentiometer* pot1 = new APotentiometer(0, "pot1");
//  pot1->setOnGateOpen(onGateOpen);
//  pot1->setOnGateClose(onGateClose);

  APotentiometer* pot2 = new APotentiometer(1, "pot2");
  pot2->setOnChange(onChangePot2);
//  pot2->setOnGateOpen(onGateOpen);
//  pot2->setOnGateClose(onGateClose);

  AudioConnection* patchCord1 = new AudioConnection(*sine, 0, *dac1, 0);
  AudioConnection* patchCord2 = new AudioConnection(*sine, 0, *led1, 0);
  AudioConnection* patchCord3 = new AudioConnection(*pot2, 0, *led2, 0);



  // SOLUTION 1
//  Potentiometer* pot1 = new Potentiometer("paramName1");
//  pot1->setMidi(1, 20);
//  pot1->setOnChange(onChangePot1);
//  
//  MidiInput* midiIn1 = new MidiInput("paramName2", 1, 21);
//  midiIn1->setOnChange(onChangeMidiIn1);

  // SOLUTION 2
//  b1 = new Block("b1");
//  b2 = new Block("b2");
//  b3 = new Block("b3");
//  b1->addOutput(b2);
//  b1->addOutput(b3);
//  b2->addInput(b1);
//  b3->addInput(b1);


   
//  pot1 = new APotentiometer(0, "pot1");
//  pot2 = new APotentiometer(1, "pot2");
//  pot1->setOnChange(onChangePot1);
  
//  Potentiometer* pot0 = new Potentiometer(0, "pot0");
//  pot0->setOnChange(onChangePot0);
  
//  Potentiometer* pot1 = new Potentiometer(1, "pot1");

//  Potentiometer* pot2 = new Potentiometer(2, "pot2");
  
//  VirtualInput* virtualInput1 = new VirtualInput(); // Virtual inputs allow to benefits from smoothing and interract in a multiple inputs parameter
//  virtualInput1->setValue(10);
//  
//  Parameter* param0 = new Parameter("param0");
//  param0->setMidi(1, 21);
//  param0->addInput(pot0);
//  param0->setOnChange(onChangeParam1);
//  
//  Parameter* param2 = new Parameter("param2");
//  param2->setMidi(1, 21);
//  param2->addInput(pot2);
//  param2->setOnChange(onChangeParam2);
//  
//  Parameter* param1 = new Parameter("param1");
//  param1->setMidi(1, 25); // MIDI would override the value
//  param1->setValue(10); // This would override the value
//  param1->addInput(pot0);
//  param1->addInput(virtualInput1);
//  param1->setOnChange(onChangeParam1);
//  param1->setInputsRelation("SUM"); // LAST, SUM, SUB, AVG // should be able to pass a function for custom relation

  // SOLUTION 3
//  Potentiometer* pot1 = new Potentiometer();
//  MidiInput* midiIn1 = new MidiInput();
//  Parameter* param1 = new Parameter("paramName1");
//  param1->setOnChange(onChangeParam1);
//  IOConnection* patchCord1 = new IOConnection(pot1, param1);
//  IOConnection* patchCord2 = new IOConnection(midiIn1, param1);
  
  
  // SOLUTION 4
//  Motherboard.registerParameter("paramName", CONTROL, MIDI_CHANNEL, MIDI_CC);



//  ToggleOnOffOn* tog = new ToggleOnOffOn();
  
//  Potentiometer* pot5 = new Potentiometer();
// pot5->setOnChange(onChange);
// pot5->setSmoothing(50);
  
//  Potentiometer* pot6 = new Potentiometer();

  
//  CvIn* in7 = new CvIn();
//  in7->setOnChangeQuantized(onChangeQuantized);
//  in7->setSmoothing(0);
//
//  CvIn* in8 = new CvIn();
//  in8->setOnChange(onVelocityChange);
//  in8->setOnGateOpen(onGateOpen);
//  in8->setOnGateClose(onGateClose);
//  in8->setSmoothing(0);

//  out = new CvOut();
//  out2 = new CvOut();
//  out3 = new CvOut();
//  out4 = new CvOut();
//  out5 = new CvOut();
//  out6 = new CvOut();
//  out7 = new CvOut();
//  out8 = new CvOut();

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
