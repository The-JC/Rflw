# Rflw

Rflw is an STM32 based Reflow and oven controller. It is intended for DIY Reflow ovens, based around a small baking oven.

## Features

- PID controled
- 128x64 OLED display for configuration
- 3 Button control
- Bake and Reflow mode
  - Bake mode holds the set temperature indefinetly
  - Reflow mode follows a speciefied curve
- 32bit Microcontroller as a future proof platform
- Real-time operating system(RTOS) allowig for a easy implementation of further features.
- Wave Packet power regulation
- Includes Watchdogs to prevent a system failure
- Up to two temperatures probes

### Planned features

- Auto PID tuning
- Bake timer
- Create reflow profiles from within the program.
- Configuration and read out via USB.
- Add a fan for cooldown

## Installation

> **Requirments**
> CMake and gcc-arm-none-eabi is required for compilation
> Either OpenOCD or STLink is required for downloading the programm to the µC

### Debian/Ubuntu

1. Install CMake, gcc-arm-none-eabi and OpenOCD by running ```sudo apt-get install cmake gcc-arm-none-eabi openocd```
2. To compile and download using OpenOCD run: ```./build.sh flash```

## Documentation

[Docs](https://lab.katurisoft.com/pages/Julian/Rflw/)