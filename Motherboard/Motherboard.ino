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

#include "Motherboard.h"

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 2500)
    ; // wait for serial monitor

  // Motherboard
  Motherboard.init("Motherboard", 2,
                   {Potentiometer, Potentiometer,
                    Potentiometer, Button,
                    Potentiometer, Potentiometer},
                   {CvOut});

  Serial.println("Ready!");
}

void loop()
{
  Motherboard.update();
  //  for(int i=0; i<6; i++){
  //    Serial.print(Motherboard.getInputValue(i));
  //    Serial.print(" ");
  //  }
  //  Serial.println("");
  Motherboard.setLED(0, MotherboardNamespace::Led::Status::On, Motherboard.getInputValue(0));
}
