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
#include "Motherboard/InputRotary.h"
// #include "Motherboard/InputGate.h"
// #include "Motherboard/InputTrigger.h"
#include "Motherboard/OutputLed.h"
// #include "Motherboard/OutputTrigger.h"
// #include "Motherboard/OutputGate.h"
// #include "MyQuantizer.h"

AudioOutputI2S i2s;
// AudioSynthWaveformSineModulated sine1;
// AudioSynthWaveformSineModulated sine2;
// AudioSynthWaveformSineModulated sine3;
// AudioSynthWaveformSineModulated sine4;
// AudioSynthWaveformSineModulated sine5;
// AudioSynthWaveformSineModulated sine6;
// AudioSynthWaveformSineModulated sine7;
// AudioSynthWaveformSineModulated sine8;
// AudioSynthWaveformSineModulated sine9;
// AudioSynthWaveformSineModulated sine10;
// AudioSynthWaveformSineModulated sine11;
// AudioSynthWaveformSineModulated sine12;
// AudioSynthWaveformSineModulated sine13;
// AudioSynthWaveformSineModulated sine14;
// AudioSynthWaveformSineModulated sine15;
// AudioSynthWaveformSineModulated sine16;
// MyQuantizer* quantizer;
Input input1(0);
Input input2(1);
// Input input3(2);
// Input input4(3);
// Input input5(4);
// Input input6(5);
// Input input7(6);
// Input input8(7);
// Input input9(8);
// Input input10(9);
// Input input11(10);
// Input input12(11);
// Input input13(12);
// Input input14(13);
// Input input15(14);
// Input input16(15);
OutputLed led1(0);
OutputLed led2(1);
// OutputLed led3(2);
// OutputLed led4(3);
// OutputLed led5(4);
// OutputLed led6(5);
// OutputLed led7(6);
// OutputLed led8(7);
// OutputLed led9(8);
// OutputLed led10(9);
// OutputLed led11(10);
// OutputLed led12(11);
// OutputLed led13(12);
// OutputLed led14(13);
// OutputLed led15(14);
// OutputLed led16(15);

void setup() {
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ;  // wait for serial monitor

  // Audio connections require memory to work.
  AudioMemory(400);


// input1.onChange(onButtonChange);
// input1.onIncrease(onIncrease);
// input1.onDecrease(onDecrease);
// input1.onPush(onPush);
// input1.onRelease(onRelease);
// input1.onClick(onClick);
// input1.onLongClick(onLongClick);

  // sine1.frequency(50);
  // sine1.amplitude(1);
  // sine1.phase(0);

  // sine2.frequency(50);
  // sine2.amplitude(1);
  // // sine2.phase(22);

  // sine3.frequency(50);
  // sine3.amplitude(1);
  // // sine3.phase(45);

  // sine4.frequency(50);
  // sine4.amplitude(1);
  // // sine4.phase(67);

  // sine5.frequency(50);
  // sine5.amplitude(1);
  // // sine5.phase(90);

  // sine6.frequency(50);
  // sine6.amplitude(1);
  // // sine6.phase(112);

  // sine7.frequency(50);
  // sine7.amplitude(1);
  // // sine7.phase(135);

  // sine8.frequency(50);
  // sine8.amplitude(1);
  // // sine8.phase(157);

  // sine9.frequency(50);
  // sine9.amplitude(1);
  // // sine9.phase(180);

  // sine10.frequency(50);
  // sine10.amplitude(1);
  // // sine10.phase(202);

  // sine11.frequency(50);
  // sine11.amplitude(1);
  // // sine11.phase(225);

  // sine12.frequency(50);
  // sine12.amplitude(1);
  // // sine12.phase(247);

  // sine13.frequency(50);
  // sine13.amplitude(1);
  // // sine13.phase(270);

  // sine14.frequency(50);
  // sine14.amplitude(1);
  // // sine14.phase(292);

  // sine15.frequency(50);
  // sine15.amplitude(1);
  // // sine15.phase(315);

  // sine16.frequency(50);
  // sine16.amplitude(1);
  // sine16.phase(337);

  // quantizer = new MyQuantizer();

  new AudioConnection(input1, 0, led1, 0);
  new AudioConnection(input2, 0, led2, 0);

  // new AudioConnection(input1, 0, sine1, 0);
  // new AudioConnection(input2, 0, sine2, 0);
  // new AudioConnection(input3, 0, sine3, 0);
  // new AudioConnection(input4, 0, sine4, 0);
  // new AudioConnection(input5, 0, sine5, 0);
  // new AudioConnection(input6, 0, sine6, 0);
  // new AudioConnection(input7, 0, sine7, 0);
  // new AudioConnection(input8, 0, sine8, 0);
  // new AudioConnection(input9, 0, sine9, 0);
  // new AudioConnection(input10, 0, sine10, 0);
  // new AudioConnection(input11, 0, sine11, 0);
  // new AudioConnection(input12, 0, sine12, 0);
  // new AudioConnection(input13, 0, sine13, 0);
  // new AudioConnection(input14, 0, sine14, 0);
  // new AudioConnection(input15, 0, sine15, 0);
  // new AudioConnection(input16, 0, sine16, 0);
  // new AudioConnection(sine1, 0, led1, 0);
  // new AudioConnection(sine2, 0, led2, 0);
  // new AudioConnection(sine3, 0, led3, 0);
  // new AudioConnection(sine4, 0, led4, 0);
  // new AudioConnection(sine5, 0, led5, 0);
  // new AudioConnection(sine6, 0, led6, 0);
  // new AudioConnection(sine7, 0, led7, 0);
  // new AudioConnection(sine8, 0, led8, 0);
  // new AudioConnection(sine9, 0, led9, 0);
  // new AudioConnection(sine10, 0, led10, 0);
  // new AudioConnection(sine11, 0, led11, 0);
  // new AudioConnection(sine12, 0, led12, 0);
  // new AudioConnection(sine13, 0, led13, 0);
  // new AudioConnection(sine14, 0, led14, 0);
  // new AudioConnection(sine15, 0, led15, 0);
  // new AudioConnection(sine16, 0, led16, 0);
  // new AudioConnection(input16, 0, led16, 0);
}

void loop() {
}

// void onButtonChange(uint16_t value){
//   Serial.println(value);
// }
// void onIncrease(uint16_t value){
//   Serial.println("Increase");
//   Serial.println(value);
// }
// void onDecrease(uint16_t value){
//   Serial.println("Decrease");
//   Serial.println(value);
// }
// void onPush(){
//   Serial.println("onPush");
// }
// void onRelease(){
//   Serial.println("onRelease");
// }
// void onClick(){
//   Serial.println("onClick");
// }
// void onLongClick(){
//   Serial.println("onLongClick");
// }