# Firmware

The firmware is written with Arduino.

## Build and upload to Pico 2

In order to build any sketch for the Pico 2 you have to install the Arduino IDE and the Arduino-Pico package. Follow the instructions here: https://github.com/earlephilhower/arduino-pico

1. Select the Pico 2 in Tools -> Board -> Raspberry Pi Pico / Pico W
2. Open Motherboard.ino located in the firmware/example folder of this repo
3. Plug the Pico 2 to your computer
4. Then just click the arrow button to upload the code

## Upload a built firmware

To share a firmware with someone rather than sharing the sources, you first have to generate a uf2 file from the code:

1. Go to Sketch -> Export compiled Binary
2. Go to Sketch -> Show Sketch Folder
3. The uf2 file can be copied and shared.
4. To upload this file, hold the BOOTSEL button on the Pico 2 while not powered, plug it to your computer and release the button once the drive appears. Then drag and drop the uf2 file on the drive.

