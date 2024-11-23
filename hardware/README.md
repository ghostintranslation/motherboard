# Hardware

The PCB is made so that it is possible to use either through hole or SMD for most components.

## Bill Of Materials

**Motherboard**

```
4 x 10k resistor
2 x 2.7k resistor
3 x 2k resistor
2 x 1k resistor
1 x 157ohm resistor
2 x 10k trim potentiometer
4 x BAT43 diode
1 x 4148 diode
2 x SN74HC4851 multiplexer
2 x SN74HC595 shift register
2 x MCP6292 or MCP6022 op-amp
1 x 6N137S optocoupler
1 x MB1S bridge rectifier
1 x VX7805-500 DC-DC converter
1 x 10uf capacitor
1 x 22uf capacitor
2 x 24 pins female header
2 x 14 pins female header
2 x 8 pins female header
1 x 4 pins female header
2 x 5 pins male header
4 x 16 pins IC socket
2 x 8 pins IC socket
```

An export of the BOM out of Eagle is available in this repo.

**Audio-MIDI Expander**

```
3 x stereo Thonkicon jack socket
2 x mono Thonkicon jack socket
2 x 8 pins male header
1 x 4 pins male header
```

### Notes

Some parts could be replaced by other alternatives, consider the following:

- MCP6292/MCP6022
  - high slew rate,
  - single supply 0-5v
  - high bandwidth
  - Rail-to-Rail

- SN74HC4851
  - included inputs protection from voltages outside the supply range.

- BAT43
  - I only tried 2N4007 – *Be careful they protect the Teensy from frying in case of voltages outside 0-3.3v ± the forward voltage, which should not exceed 0.7v*

- VX7805-500 is obsolete and I haven't tried any replacement yet, potential candidates:
  - R-78CK5.0-0.5
  - R-78K5.0-0.5

- If you don't wish to use MIDI you can omit:
  - the optocoupler and its resistors and diode

- Unless using I2C, which is not the case with barebone Motherboard and its audio expander, you can omit:
  - the 2 pullup resistors R11 and R12

- IC sockets could be replaced by round female headers

## Schematics

An export of the schematics out of Eagle is available in this repo.

### Notes

The op-amps, multiplexers, and shift registers are displayed twice in the schematics and BOM file as if there were 2 sets of each but that is because one set is for through hole and one set is for SMD to have the choice. There is only 2 op-amps, 2 multiplexers and 2 shift registers.

## Audio-Midi expander

The Motherboard itself does not include any audio DAC, this is because of limited space and to reduce cost for modules that would not require audio. However an expander connector is there to provide the necessary for adding such feature.

I designed an audio-midi expander board that direcly attached on the back of the Motherboard. It provides 2 mono audio jacks, 1 stereo audio jack, and 2 MIDI TRS (Type A) jacks in the back to allow reducing clutter on the front, but the audio and MIDI signals are also available on the main headers to be passed to the front panels.

## Known issues 

In some versions of the audio-midi expander, the audio outputs are quite noisy. To reduce noise I added a through hole 0.33uF capacitor myself. This will be fixed in future batches.