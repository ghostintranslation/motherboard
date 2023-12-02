# THIS IS A WIP

# MOTHERBOARD

![GitHub version](https://img.shields.io/github/v/release/ghostintranslation/motherboard.svg?include_prereleases)

**[PCBs AND MODULES AVAILABLE HERE](https://ghostintranslation.bandcamp.com/merch/)**

## Features

* 8 analog inputs (12 bits, 5v, 44.1kHz)
* 8 leds outputs
* 2 audio channels over I2S or 8 channels over TDM
* I2C to expand with analog outs and more
* MIDI in and out
* 10 pin Eurorack either way power
* Through hole and SMD footprints for most of the chips
* Simple programming

## Introduction

Wether analog or digital, often modules are designed from scratch as one-offs creations. This means going through the same challenges and solving them in slightly different ways. It’s possible to do better and not reinvent the wheel every time. 

By separating the functional circuitry from the user interface circuitry, a pattern will emerge and make it possible to reuse at least a few things. 

With analog it is more difficult to achieve as the circuit is very much dependent on the  inputs and outputs, but it would still be possible to have a generic user interface connected to a specific functional circuit. 

With digital this gets interesting. The functional circuit can be completely generic and be reused for many modules, simply reading inputs and generating outputs, it doesn’t need to be designed around the final module. The low level code can be generic and reused too, while the high level code would be specific to each module. 

## Hardware

- Photos of Motherboard with labels
- Schematics

### Bill Of Materials

### Teensy

In order to run any sketch on the Teensy you have to install the Arduino IDE. Follow the instructions from the official page, section "Arduino 2.0.x Software Development": https://www.pjrc.com/teensy/td_download.html

1. Then open Motherboard.ino located in the firmaware/example folder of this repo.
2. In the Tools -> USB Type menu, choose Serial.
3. **Make sure the Motherboard is not powered with external power when pluggin Teensy to a computer!**
3. Plug the Teensy to your computer with a micro USB cable.
4. Then just click the arrow button to upload the code

## Firmware

## How to 
## TODO
- Add schematics in the repo
- Create a .ino sketch example
- See if possible to create an expander that attaches to the back
- Actually implement the MIDI Input
- Final check of the MIDI hardware
- Rewire the op amp to be powered by 5v, footprint for mcp6292
- Can Tx detect If MIDI or CLK type of signals
- How to Teletype I2C compatibility
- Web editor to edit settings and maybe visualize data


# About me
You can find me on Bandcamp, Instagram, Youtube and my own site:

https://ghostintranslation.bandcamp.com/

https://www.instagram.com/ghostintranslation/

https://www.youtube.com/ghostintranslation

https://www.ghostintranslation.com/

# Support
To support my work:<br>
https://www.patreon.com/ghostintranslation
https://www.paypal.com/paypalme/ghostintranslation
https://www.buymeacoffee.com/ghostintranslation

# License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details