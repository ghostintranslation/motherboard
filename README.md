# MOTHERBOARD

![GitHub version](https://img.shields.io/github/v/release/ghostintranslation/motherboard.svg?include_prereleases)

## MOdular Teensy Hybrid Eurorack Board

MOTHERBOARD is a Teensy 4.0 modular platform made to facilitate the creation of various modules ranging from synthesizers to sequencers. 

MOTHERBOARD follows a column design of 3 inputs per column. Currently 3 physical variants exist:
* MOTHERBOARD6: 2 columns, 6 inputs
* MOTHERBOARD9: 3 columns, 9 inputs
* MOTHERBOARD12: 4 columns, 12 inputs

What makes it interesting is how the inputs are stacked to allow for any combination of pushbuttons, potentiometers, encoders. The footprints of those 3 types of components are stacked together so that each input can be any of those, and only one pcb can be used for many different modules. 

<img src="input-traces.png" width="200px"/>

MOTHERBOARD's physical format is following Eurorack standard, but it doesn't use CV/Gates and patch cables and has rather MIDI and audio jacks in the back. It is oriented for live performance more than experimentation.

MOTHERBOARD comes with 2 boards, one on which the inputs and leds are soldered to (A), and one on which the Teensy and other components are soldered to (B). Both boards are attached together by multiple pin headers.

You can get the PCBs here:<br/>
https://ghostintranslation.bandcamp.com/merch/

<img src="motherboard6.jpg" width="200px"/> <img src="motherboard9.jpg" width="200px"/> <img src="motherboard12.jpg" width="200px"/>

## Schematics

Due to the use of the audio board, the available pins are very limited. Looking at the Teensy audio board page (https://www.pjrc.com/store/teensy3_audio.html) we can see only pins 0, 1, 2, 3, 4, 5, 9, 14, 16, 17, 22 are available. Also looking at Teensy pins (https://www.pjrc.com/store/teensy40.html), only 14, 16 and 17 from this subset are analog inputs.

So the use of multiplexers is required to be able to read pushbuttons, potentiometers, encoders or to lit leds. In addition, a matrix design is used for the encoders to reduce the number of inputs required as each of them has 3 inputs.

With this design, pin 22 will switch from input to output very fast to lit the leds and read the inputs.

### MOTHERBOARD6
<p>
<img src="motherboard6a-schematics.png" width="200px"/> <img src="motherboard6b-schematics.png" width="200px"/>
</p>

### MOTHERBOARD9
<p>
<img src="motherboard9a-schematics.png" width="200px"/> <img src="motherboard9b-schematics.png" width="200px"/>
</p>

### MOTHERBOARD12
<p>
<img src="motherboard12a-schematics.png" width="200px"/> <img src="motherboard12b-schematics.png" width="200px"/>
</p>

### Notes

Dependng on the type of inputs used, not all multiplexers may be required. 

- IC1 = Mux for potentiometers
- IC2 = Mux for LEDs
- IC3 = Mux for encoders
- IC4 = Mux for encoder's switches and pushbuttons
- IC5 = Main mux, always required
- IC6 = Mux for midi channel dipswitch

A few examples:

If you only use potentiometers, you won't need IC3 and IC4. Or if you don't have any led you won't need IC2. Or if you don't want to use a dipswitch to select the midi channel, you won't need IC6.

## Bill Of Materials

Here is the list of components you will need:

```
1 MOTHERBOARD6A pcb
1 MOTHERBOARD6B pcb
1 Teensy 4.0
1 Teensy audio board (optional)
1 5 pins male header
1 5 pins female headers
5 14 pins male header
5 14 pins female header
2 14 pins long female header (see note)
2 3.5mm jack connectors
1 Resistor ~ 22ohm
1 4 positions dipswitch (optional)
x CD4051 multiplexers
x DIP16 IC sockets (optional)
x LEDs
x Vertical linear 10k potentiometers
x Vertical rotary encoders
x D6 pushbuttons
```

Note: long female headers are used to stack the audio board to Teensy and connect Teensy to the MOTHERBOARD

Here is a list of useful links to get these parts: https://github.com/ghostintranslation/parts

## Teensy

In order to run any sketch on the Teensy you have to install Arduino and the Teensyduino add-on.
Follow the instructions from the official page:
https://www.pjrc.com/teensy/teensyduino.html

1. Then open `Motherboard.ino` located in the `Motherboard` folder of this repo.
2. In the Tools -> USB Type menu, choose `Serial + midi`.
3. Plug the Teensy to your computer with a micro USB cable. (It's ok if the Teensy is on the module)
4. Then just click the arrow button to upload the code

## MIDI

The MIDI input and output jacks are directly connected to the Teensy serial input and output. That means there is no protection against voltage or current. It is primarily ment to connect 2 of these modules, or 2 Teensy together. If you want to connect something else to it make sure to provide a maximum of `3.3v` and `250mA`.

## How to use

Copy the `Motherboard.h` file in your project. Then just include it and start using it.

### Init

Motherboard is a singleton, there can be only one instance. This is how to initialize it:

```
Motherboard * motherboard;

void setup() {
  motherboard = Motherboard::init(
    "Motherboard",
    {
      Potentiometer, Potentiometer,
      Potentiometer, Potentiometer,
      Potentiometer, Potentiometer
    }
  );
}
```
The first parameter is the name of the module. This will show in the MIDI web editor.

The second parameter is the list of inputs. Potentiometer, Button, RotaryEncoder are valid values.

Then you have to call the Motherboard's `update`:
```
void loop() {
  motherboard->update();
}
```

### LEDs

LEDs are controlled by setting their status according to:

- 0 = Off
- 1 = Solid
- 2 = Slow flashing
- 3 = Fast flashing
- 4 = Solid for 50 milliseconds
- 5 = Solid low brightness

#### Set one LED:
```
void setLED(byte ledIndex, byte ledStatus);
```

#### Set multiple LEDs:
The first parameter called `binary` is a number that will be represented in binary with the least significant bit on the left. Ex: 9 = 100100 => LEDs of indexes 0 and 3 will be lit.
```
void setAllLED(unsigned int binary, byte ledStatus);
```

#### Toggle an LEDs:
```
void toggleLED(byte index);
```

#### Reset all LEDs:
```
void resetAllLED();
```

### Inputs

#### Get one input value:
- In the case of a potentiometer it will return between 0 and the max analog resolution which is 1023 by default.
- In the case of a button it will return 1 for push 0 for release.
- In the case of a rotary it will return the number of rotations since the last get.
```
int getInput(byte index);
```

#### Get the switch value of an encoder:

Because an encoder is like 2 controls, the rotary and the switch, we need this function in addition to `getInput`.
```
int getEncoderSwitch(byte index);
```

#### Get the maximum possible value of the potentiometers:
The value depends of the ADC resolution, which is 10 by default and can be set to 12 or 8.

With a resolution of 10 bits, the maximum value is 1023.
```
int getAnalogMaxValue();
```

#### Get the minimum possible value of the potentiometers:
It should always be 0, but if your potentiometers are not that accurate they could return bigger than 0 as a minimum value. You could then change the return value of that function to match the real minimum. This will ensure that `getInput` always returns a minimum of 0 and a maximum of 1023 that corresponds to the actual min and max. 
```
int getAnalogMinValue();
```

#### Get the midi channel
This is set by the dipswitch and read only once when powered on. If no dipswtich is soldered then the channel will be 1.
```
byte getMidiChannel();
```

### Callbacks
Callbacks are a very good way of handling inputs. Using them instead of reading `getInput` in the loop will make your code easier to read and maintain.

#### setHandlePressDown

Callback will be called on a press of a button

```
/**
 * Handle press down on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
void setHandlePressDown(byte inputIndex, PressDownCallback fptr);
```

#### setHandleLongPressDown

Callback will be called on a long press of a button

```
/**
 * Handle long press down on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
void setHandleLongPressDown(byte inputIndex, LongPressDownCallback fptr);
```

#### setHandlePressUp

Callback will be called at release of a button.

```
/**
 * Handle press up on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
void setHandlePressUp(byte inputIndex, PressUpCallback fptr);
```

#### setHandleLongPressUp

Callback will be called at release after a long press of a button.

```
/**
 * Handle long press up on a button
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex)
 */
void setHandleLongPressUp(byte inputIndex, LongPressUpCallback fptr);
```

#### setHandlePotentiometerChange

Callback will be called on a change of a potentiometer.

```
/**
 * Handle potentiometer change
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex, float value, int diffToPrevious)
 */
void setHandlePotentiometerChange(byte inputIndex, PotentiometerChangeCallback fptr);
```

#### setHandleRotaryChange

Callback will be called on a rotation of a rotary encoder.

```
/**
 * Handle rotary change
 * @param inputIndex The index of the input
 * @param fptr The callback function, void (*)(byte inputIndex, bool value)
 */
void setHandleRotaryChange(byte inputIndex, RotaryChangeCallback fptr);
```

### MIDI callbacks
Callbacks also exist for some MIDI messages.

Note, setting a callback on channel 0 means it is going to be triggered on messages addressed to the channel set by the dipswitch on the board. Otherwise channel is from 1 to 16.

#### setHandleMidiNoteOn

Callback triggered on MIDI note on message.

```
/**
 * Handle MIDI note on
 * @param fptr The callback function, void (*)(byte channel, byte note, byte velocity)
 */
void setHandleMidiNoteOn(MidiNoteOnCallback fptr);
```

#### setHandleMidiNoteOff

Callback triggered on MIDI note off message.

```
/**
 * Handle MIDI note off
 * @param fptr The callback function, void (*)(byte channel, byte note, byte velocity)
 */
void setHandleMidiNoteOff(MidiNoteOffCallback fptr);
```

#### setHandleGlobalMidiControlChange

Callback triggered on any MIDI Control Change messages, not depending on the channel nor the control number.

```
/**
 * Handle MIDI global control change
 * @param fptr The callback function, void (*)(byte channel, byte control, byte value)
 */
void setHandleGlobalMidiControlChange(GlobalMidiControlChangeCallback fptr);
```

#### setHandleMidiControlChange

Callback triggered on MIDI Control Change messages addressed to the channel set on the board.

```
/**
 * Handle MIDI control change
 * @param control The control number
 * @param controlName The name of the control
 * @param fptr The callback function, void (*)(byte channel, byte control, byte value)
 */
void setHandleMidiControlChange(byte control, String controlName, MidiControlChangeCallbackFunction fptr);
```

#### setHandleMidiControlChange

Callback triggered on MIDI Control Change messages.

```
/**
 * Set handle MIDI control change
 * @param channe; The channel number
 * @param control The control number
 * @param controlName The name of the control
 * @param fptr The callback function, void (*)(byte channel, byte control, byte value)
 */
void setHandleMidiControlChange(byte midiChannel, byte control, String controlName, MidiControlChangeCallbackFunction fptr);
```

#### setHandleMidiSysEx

Callback triggered on MIDI SysEx messages.

```
/**
 * Set handle MIDI SysEx
 * @param fptr The callback function, void (*)(const uint8_t* data, uint16_t length, bool last)
 */
void setHandleMidiSysEx(MidiSysExCallback fptr);
```

## Design rules

Here are the dimensions for any module size. Every column is following the same rules. So the size of a module depends on the number of column. 2 columns = 2x20mm = 40mm, 3 columns = 3x20 = 60mm ...

<img src="design-rules.png" width="200px"/>

# About me
You can find me on Bandcamp, Instagram, Youtube and my own site:

https://ghostintranslation.bandcamp.com/

https://www.instagram.com/ghostintranslation/

https://www.youtube.com/channel/UCcyUTGTM-hGLIz4194Inxyw

https://www.ghostintranslation.com/

# Support
To support my work:<br>
https://www.paypal.com/paypalme/ghostintranslation

# License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details