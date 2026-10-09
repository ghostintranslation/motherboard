# Motherboard

The Motherboard is a Eurorack platform that provides an easy way to build modules abstracting low level details away from the developer.  

## Features

* 15 analog inputs
* 16 digital outputs
* 2 audio outputs
* Addressable LEDs
* MIDI TRS type A in and out
* 10 pin Eurorack either way power
* Uses a Pico 2 (RP2350)

## The idea

Wether analog or digital, often modules are designed from scratch as one-offs creations. This means going through the same challenges and solving them in slightly different ways. It’s possible to do better and not reinvent the wheel every time. 

By separating the functional circuitry from the user interface circuitry, a pattern will emerge and make it possible to reuse at least a few things. 

With analog it is more difficult to achieve as the circuit is very much dependent on the inputs and outputs, but it would still be possible to have a generic user interface connected to a specific functional circuit. 

With digital this gets interesting. The functional circuit can be completely generic and be reused for many modules, simply reading inputs and generating outputs, it doesn’t need to be designed around the final module. The low level code can be generic and reused too, while the high level code would be specific to each module.


## Instructions

See the hardware's and firmware's instructions under their respective folder.

## Known issues


## TODO

# About me

You can buy assembled modules on my website:

https://www.ghostintranslation.com/

You can also find me on Bandcamp, Instagram, YouTube:

https://ghostintranslation.bandcamp.com/

https://www.instagram.com/ghostintranslation/

https://www.youtube.com/ghostintranslation

To support my work:

https://ko-fi.com/ghostintranslation

https://www.buymeacoffee.com/ghostintranslation

# License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details