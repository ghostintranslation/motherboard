/*
 __    _  _____   ___      ___ _  _     __    _ ______ _    
/__|_|/ \(_  |     | |\|    | |_)|_||\|(_ |  |_| |  | / \|\|
\_|| |\_/__) |    _|_| |    | | \| || |__)|__| | | _|_\_/| |

MOTHERBOARD
1.0.0

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

#include "Motherboard.h"

// Motherboard
Motherboard * device;
  
void setup() {
  Serial.begin(115200);
  
  while (!Serial && millis() < 2500); // wait for serial monitor

  // Motherboard
  device = Motherboard::init(
    "Motherboard",
    {
      Potentiometer, Potentiometer,
      Potentiometer, Potentiometer,
      Potentiometer, Potentiometer
    }
  );
  
  // Potentiometer callback
  device->setHandlePotentiometerChange(0, onPotentiometerChange);

  // MIDI callback
  device->setHandleMidiControlChange(0, 0,  "Parameter",   onMidiControlChange);

  Serial.println("Ready!");
}

void loop() {
  device->update();
}

/**
 * Potentiometer Change
 */
void onPotentiometerChange(byte inputIndex, float value, int diffToPrevious){
  Serial.print("onPotentiometerChange: ");
  Serial.print(inputIndex);
  Serial.print(" ");
  Serial.print(value);
  Serial.print(" ");
  Serial.print(diffToPrevious);
  Serial.println("");
}

/**
 * Midi Control Change callback
 */
void onMidiControlChange(byte channel, byte control, byte value) {
  Serial.print("onMidiControlChange: ");
  Serial.print(channel);
  Serial.print(" ");
  Serial.print(control);
  Serial.print(" ");
  Serial.print(value);
  Serial.println("");
}
