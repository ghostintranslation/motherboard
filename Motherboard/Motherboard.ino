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

//#include <Audio.h>
#include "Motherboard.h"

CvOut* out;
CvOut* out2;
CvOut* out3;
CvOut* out4;
CvOut* out5;
CvOut* out6;
CvOut* out7;
CvOut* out8;


//AudioSynthWaveform       waveform1;      //xy=171,84
//AudioSynthWaveform       waveform2;      //xy=178,148
//AudioOutputI2S           i2s1;           //xy=360,98
//AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
//AudioConnection          patchCord2(waveform2, 0, i2s1, 1);
//AudioControlSGTL5000     audioboard;     //xy=239,232

void setup()
{
  Serial.begin(115200);

  // Wait for serial monitor
  while (!Serial && millis() < 2500); 

   // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
//  AudioMemory(20);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
//  audioboard.enable();
//  audioboard.volume(0.5); // caution: very loud - use oscilloscope only!
  
//  TouchPad* pad1 = new TouchPad();
//  TouchPad* pad2 = new TouchPad();
//  TouchPad* pad3 = new TouchPad();

//Button* but1 = new Button();
  
  Potentiometer* pot1 = new Potentiometer();
  pot1->setOnChange(onChangePot1);
  Potentiometer* pot2 = new Potentiometer();
  pot2->setOnChange(onChangePot2);
  Potentiometer* pot3 = new Potentiometer();
  pot3->setOnChange(onChangePot3);
  Potentiometer* pot4 = new Potentiometer();
  pot4->setOnChange(onChangePot4);
  Potentiometer* pot5 = new Potentiometer();
  Potentiometer* pot6 = new Potentiometer();
  Potentiometer* pot7 = new Potentiometer();
  Potentiometer* pot8 = new Potentiometer();

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

  out = new CvOut();
  out2 = new CvOut();
  out3 = new CvOut();
  out4 = new CvOut();
  out5 = new CvOut();
  out6 = new CvOut();
  out7 = new CvOut();
  out8 = new CvOut();

//  Motherboard.setDebug(true);
  Motherboard.init("Motherboard", 2,
                   {
                    pot1, pot2,
                    pot3, pot4,
                    pot5, pot6,
                    None, None,
                   },
                   {
                    None, None,
                    None, None,
                    None, None,
                    None, None
                   });


  Motherboard.setHandleMidiNoteOn(noteOnCallback);
  
  Serial.println("Ready!");
}

elapsedMillis timeOut;
int outValue = 0;
int outValue2 = 0;

void loop()
{
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

  float mapped = map(value,0,4095,0,1000);
  waveform1.frequency(mapped);
  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, value);
}

void onChangePot2(byte inputIndex, float value, float diffToPrevious){
  waveform1.amplitude(map(value,0,4095,0,0.05));
}
void onChangePot3(byte inputIndex, float value, float diffToPrevious){
  float mapped = map(value,0,4095,0,1000);
  waveform2.frequency(mapped);
  Motherboard.setLED(1, MotherboardNamespace::Led::Status::On, value);
}
void onChangePot4(byte inputIndex, float value, float diffToPrevious){
  float mapped = map(value,0,4095,0,0.05);
  Serial.println(mapped);
  waveform2.amplitude(mapped);
}

void onVelocityChange(byte inputIndex, float value, float diffToPrevious){
//  float gainValue = map(value, 0, 4095, 0, 1);
//  getInstance()->voices[0]->getOutput()->gain(1, gainValue);
  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, value);
}

void onChangeQuantized(byte inputIndex, int note){
  Serial.print("onChangeQuantized ");
  Serial.println(note);
}

void onGateOpen(byte inputIndex){
  Serial.print("onGateOpen ");
  Serial.println(inputIndex);

  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, 4095);
}

void onGateClose(byte inputIndex){
  Serial.print("onGateClose ");
  Serial.println(inputIndex);
  
  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, 0);
}

void noteOnCallback(byte channel, byte note, byte velocity){
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(note);
  Serial.print(" ");
  Serial.println(velocity);
}
