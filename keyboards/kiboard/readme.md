# KiBoard

A split keyboard powered by RP2040 (Raspberry Pi Pico) featuring:
* 8 keys (4 per half)
* 2 analog joysticks (1 per half)
* QMK firmware
* Split communication via UART

## Hardware
* Controller: Raspberry Pi Pico (RP2040)
* Keys: 8 mechanical switches
* Joysticks: 2x 2-axis analog joysticks

## Building
1. Clone QMK firmware
2. Make and flash: `qmk compile -kb kiboard`