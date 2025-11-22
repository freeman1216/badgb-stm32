## BADGB – STM32 version
A port of my [DMG emulator](https://github.com/freeman1216/badgb-sdl2) to STM32F411CEU6 on a ubiquitous blackpill board.
The emulator limitations are the same. 
Uses my [codebase](https://github.com/freeman1216/badhal) for all the hardware stuff.

## Hardware
- WeAct blackpill board
- ILI9341 display
- 8 buttons (preferably rc filtered,as no debouncing happens in the code)

## Pinout
# Exti input lines (no internal pullup)
- PA0 = right 
- PA1 = left
- PA2 = up
- PA3 = down
- PA4 = A 
- PA5 = B 
- PA6 = select
- PA7 = start
# ILI9341 SPI
- PB3 = SPI clock
- PB4 = SPI miso (not used, the code uses bidirectional output only)
- PB5 = SPI mosi
# ILI9341 GPIO
- PB6 = SPI cs (software driven)
- PB7 = ILI9341 dc
- PB8 = ILI9341 reset

## Demonstration
![Batman gif](img/bat.gif)
![Battletoads gif](img/bt.gif)

## Dependencies
* arm-none-eabi-gcc
* openocd
* make
* sed
* xxd (generates the rom header)

## Usage
Get your stuff connected according to the pinout, connect your board then run :
```shell
build.sh some_rom.gb

```

## Special Thanks
* [GBdev Pandocs](https://gbdev.io/pandocs/), main source of information regarding Gameboy emulation development 
* [PeanutGB by deltabeard](https://github.com/deltabeard/Peanut-GB/) MIT Licence, a very well written emulator, SBC instruction was taken from there
* [Sameboy by LIJI32](https://github.com/LIJI32/SameBoy/tree/master) MIT Licence, sometimes by looking at code there you will get more understanding then by reading the docs. DAA, LD HL,SP+e8, ADD SP,s8 instructions were taken from there
