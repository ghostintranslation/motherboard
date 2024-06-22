# THIS IS A WIP

# MOTHERBOARD

![GitHub version](https://img.shields.io/github/v/release/ghostintranslation/motherboard.svg?include_prereleases)

## Features

* 16 analog inputs (12 bits, 5v, 44.1kHz)
* 16 "PWM" outputs (Binary Code Modulation)
* 2 audio channels over I2S or 8 channels over TDM
* I2C to expand with analog outputs and more
* MIDI in and out
* 10 pin Eurorack either way power
* Through hole and SMD for most footprints
* Teensy 4.0

## Introduction

Wether analog or digital, often modules are designed from scratch as one-offs creations. This means going through the same challenges and solving them in slightly different ways. It’s possible to do better and not reinvent the wheel every time. 

By separating the functional circuitry from the user interface circuitry, a pattern will emerge and make it possible to reuse at least a few things. 

With analog it is more difficult to achieve as the circuit is very much dependent on the inputs and outputs, but it would still be possible to have a generic user interface connected to a specific functional circuit. 

With digital this gets interesting. The functional circuit can be completely generic and be reused for many modules, simply reading inputs and generating outputs, it doesn’t need to be designed around the final module. The low level code can be generic and reused too, while the high level code would be specific to each module.

## Hardware

### Schematics

#### Audio-Midi expander

https://www.falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWECAc0BsBOFCAsBmSTXMAdiOUmRAOQFMBaMMAKACUQGAmXK3FTlxQD+UcCC5wo0TOGhcxMBCwCGndLhAatm7QKRUw8eBOi5MFzAnwIjXawiRdoSI8ZYB3dZtENtVSE8dMW1-FgBzTjAhcEwFJhjJAQCWACdvGgEEkWTweDTg7TBcXU1DBED07Nj46IFisrz3KrrwEqiYzHQxN3yvP00UKgG2sqCRho769sCAY0FeTKmlgNhjSw3Njaj5QlJbUhKCTHwUCBg4ViqePiyb8BjDd0juRdbXwxmoAvtu7V-wOgAk1ApEAWAgRIEN0kooWPNJkU4sFVsYiFsMRZTPguFxokCEPtcI5IPZpJcWAAPCQnB4KfDEB5ITQ8EAAQQArgATACWAHsADoAZz5HIALuxll0osjpcDFgZFC4WGRNCMGKJtBr6iDVoRJGjDZAkGw6EKeUKxSoAHazOjjLW+LUQ75eLU44La12ezVquVBLVyzj+rzDIMjQL9Z3dCPe6MouNql3uhSRmrBAFhakMfCEECkBaaIw0CSaADCAGUCCUqVFmFEbOAKg2FCzNBWedbwgAbOjCoUcgAOg+7AE9hSpufyJwBbQcsPkhRq8LEGNZgaFkqipkD4BdtBMERzUC6QdBkiAKYq7lhAA

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
- Can Tx detect If MIDI or CLK type of signals
- How to Teletype I2C compatibility
- Web editor to edit settings and maybe visualize data
- Explore replacement of Teensy for direct integration to the board by STM32F7/Pi Pico/Linux chip/FPGA chip/...


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