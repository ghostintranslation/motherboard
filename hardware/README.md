# Hardware

The PCB is made so that it is possible to use either through hole or SMD for most components.

## Bill Of Materials

### Motherboard

Note: A Mouser list of parts file that can be imported on the website is available in this folder.

| Qty | Description                | Ref                | SMD package       | TH available |
|-----|----------------------------|--------------------|-------------------|--------------|
| 4   | 10k resistor               | R1, R3, R10, R13   | 0805              | Yes          |
| 2   | 2.7k resistor              | R2, R11            | 0805              | Yes          |
| 3   | 2k resistor                | R6, R7, R8         | 0805              | Yes          |
| 2   | 1k resistor                | R4, R12            | 0805              | Yes          |
| 1   | ~157 resistor              | R5                 | 0805              | Yes          |
| 2   | 10k trim potentiometer     | R9, R14            | 0805              | Yes          |
| 4   | BAT43 diode                | D1, D2, D3, D4     | SOD-323 / SOD-123 | Yes          |
| 1   | 4148 diode                 | D5                 | SOD-323 / SOD-123 | Yes          |
| 1   | 10uf capacitor             | C1                 | 0805              | Yes          |
| 1   | 22uf capacitor             | C2                 | 0805              | Yes          |
| 2   | SN74HC4851 multiplexer     | IC1, IC3           | 16-TSSOP          | Yes          |
| 2   | SN74HC595 shift register   | IC2, IC4           | 16-TSSOP          | Yes          |
| 2   | MCP6292 / MCP6022 op-amp   | IC5, IC6           | 8-SOIC            | Yes          |
| 1   | 6N137S optocoupler         | OPTOCOUPLER        | 8-SOIC            | No           |
| 1   | MB1S bridge rectifier      | B1                 | 4-SOIC            | No           |
| 1   | VX7805-500 DC-DC converter | CONVERTER          | -                 | Yes          |
| 2   | 24 pins female header      | JP4, JP5           | -                 | Yes          |
| 2   | 14 pins female header      | TEENSY             | -                 | Yes          |
| 2   | 8 pins female header       | JP2                | -                 | Yes          |
| 2   | 4 pins female header       | JP1                | -                 | Yes          |
| 2   | 5 pins male header         | POWER              | -                 | Yes          |
| 4   | 16 pins IC socket (TH only)| IC1, IC2, IC3, IC4 | -                 | -            |
| 2   | 8 pins IC socket (TH only) | IC5, IC6           | -                 | -            |

### Audio-MIDI Expander

| Qty | Description                  | Ref                       | SMD package | TH available |
|-----|------------------------------|---------------------------|-------------|--------------|
| 3   | Stereo Thonkicon jack socket | Stereo, MIDI_OUT, MIDI_IN | -           | Yes          |       
| 2   | Mono Thonkicon jack socket   | L, R                      | -           | Yes          |       
| 2   | 8 pins male header           | -                         | -           | Yes          |       
| 1   | 4 pins male header           | -                         | -           | Yes          |       


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

- VX7805-500 is obsolete and can be replaced by Recom Power R-78K5.0-0.5 or R-78E5.0-0.5 or similar.

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

In some earlier versions of the audio-midi expander, the audio outputs are quite noisy. To reduce noise I added a through hole 0.33uF capacitor myself. This is fixed in later batches.