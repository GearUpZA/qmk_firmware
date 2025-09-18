# Combo KiBoard

A custom 8-button keyboard with dual analog/digital joysticks, combo system, and touchscreen interface, designed for the Raspberry Pi Pico.

## Features

- 8 buttons in 2x4 matrix configuration using QMK's direct pin matrix
- Dual joysticks with analog/digital mode switching using QMK's joystick and pointing device APIs
- 15 customizable combos using QMK's built-in combo system
- Touchscreen interface for configuration
- Profile management using QMK's eeconfig system
- Profile switching via combos or touchscreen
- Mouse control via QMK's pointing device feature

## QMK Features Used

- **Combos**: QMK's built-in combo system with `COMBO_ACTION()` and `process_combo_event()`
- **Joystick**: QMK's joystick feature for HID joystick functionality  
- **Pointing Device**: QMK's pointing device API for mouse control
- **EECONFIG**: QMK's EEPROM configuration system for persistent settings
- **Layers**: QMK's layer system for different key layouts
- **Direct Pin Matrix**: QMK's direct pin matrix for buttonless matrix scanning

## Hardware

- **MCU**: Raspberry Pi Pico (RP2040)
- **Buttons**: 8 buttons (GP0-GP7)
- **Joysticks**: 2 analog joysticks (GP26-GP29) with digital inputs (GP8-GP11)
- **Touchscreen**: Waveshare 2.8" capacitive via SPI (GP14-GP19)
- **Mode switches**: GP12-GP13 for joystick mode switching

## Combos

| Combo | Keys | Action |
|-------|------|--------|
| Profile 1 | A + 1 | Switch to profile 1 |
| Profile 2 | B + 2 | Switch to profile 2 |
| Profile 3 | C + 3 | Switch to profile 3 |
| Profile 4 | D + 4 | Switch to profile 4 |
| Joy1 Toggle | A + B | Toggle joystick 1 mode |
| Joy2 Toggle | C + D | Toggle joystick 2 mode |
| Calibrate | 1 + 2 + 3 | Start calibration |
| Emergency Reset | A + D + 1 + 4 | Emergency reset |
| Factory Reset | A + B + C + D | Factory reset |

## Building

```bash
qmk compile -kb combo_kiboard -km default
```

## Flashing

```bash
qmk flash -kb combo_kiboard -km default
```

Hold the BOOTSEL button while connecting USB to enter bootloader mode.
