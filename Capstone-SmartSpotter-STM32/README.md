# 1252-Nucleo-G0B1RE-Firmware
Starting code (bare metal) for the Development board version Nucleo-G0B1RE

![Nucleo Board](/Docs/DEV_BOARD/NUCLEO-G0B1RE.png)

![Nucleo Board Pinout](/Docs/DEV_BOARD/nucleo64Pinout.png)

## Development board

- [Board Schematics](/Docs/DEV_BOARD/devBoardSchematics64.pdf)
- [Development Board User Manual](/Docs/DEV_BOARD/devBoardUserManual64.pdf)

## STM32G0B1RE Microcontroller

- [Reference Manual](/Docs/MCU/ReferenceManual_RM0444.pdf)
- [Datasheet](/Docs/MCU/Datasheet_stm32G0b1re.pdf)


# Smart Spotter – STM32 Firmware

This repository contains the firmware for the Smart Spotter capstone project.
The firmware is developed on the STM32 NUCLEO-G0B1RE platform using
SEGGER Embedded Studio.

## Architecture
- Lib/: reusable hardware drivers (GPIO, I2C, UART, Timer, etc.)
- FirmwareProjects/SmartSpotter-Firmware/: application-level capstone logic

## Project Focus
- IMU-based motion monitoring
- Anomaly-based failed repetition detection
- Visual alerting and event logging
