# KiBoard

A customizable keyboard powered by Raspberry Pi Pico featuring 8 keys, 2 analog joysticks, and a 2.8" touchscreen for configuration.

## Hardware Requirements

- 1x Raspberry Pi Pico
- 1x ILI9341 2.8" TFT LCD Display with XPT2046 Touch Controller
- 8x Mechanical Switches
- 2x 2-axis Analog Joysticks
- Various headers and wiring

## Pin Connections

### Display Connections
| Display Pin | Pico GPIO |
|------------|-----------|
| VCC        | VBUS      |
| GND        | GND       |
| CS         | GP17      |
| RESET      | GP14      |
| DC         | GP15      |
| MOSI       | GP19      |
| SCK        | GP18      |
| MISO       | GP16      |
| LED        | 3.3V      |

### Touch Controller
| Touch Pin  | Pico GPIO |
|------------|-----------|
| T_CS       | GP13      |
| T_IRQ      | GP12      |

### Keyboard Matrix
| Component  | Pico GPIO |
|------------|-----------|
| Key A      | GP0       |
| Key B      | GP1       |
| Key C      | GP2       |
| Key D      | GP3       |
| Key 1      | GP4       |
| Key 2      | GP5       |
| Key 3      | GP6       |
| Key 4      | GP7       |

### Analog Joysticks
| Component  | Pico GPIO |
|------------|-----------|
| Joy1 X     | GP26      |
| Joy1 Y     | GP27      |
| Joy2 X     | GP28      |
| Joy2 Y     | GP29      |

## Software Setup

### QMK Firmware
1. Clone QMK Firmware repository
2. Copy KiBoard files to `keyboards/kiboard/`
3. Build firmware:
```bash
qmk compile -kb kiboard
```

### Display Configuration
1. Install CircuitPython on your Pico
2. Copy the following files to your Pico:
   - `display/touch_config.py`
   - Required libraries (adafruit_ili9341, adafruit_xpt2046)

## Operation Modes

### Normal Mode
- Standard keyboard operation
- Combo detection active
- Joystick input processing

### Configuration Mode
- Access by holding A+B+C+D for 3 seconds
- Touch interface for keymap configuration
- Settings stored in `keymap.json`

## Directory Structure
```
kiboard/
├── config.h           # Hardware configuration
├── kiboard.c         # Main keyboard logic
├── kiboard.h         # Main header file
├── config_mode.c     # Configuration mode handling
├── config_mode.h     # Configuration declarations
├── display/          # Display configuration
│   └── touch_config.py  # Touch UI implementation
├── keymaps/         
│   └── default/     # Default keymap
└── rules.mk         # Build configuration
```

## Custom Keymaps

The keyboard supports various combo configurations that can be modified through the touchscreen interface:
- Single key + Single key combinations (A+1, B+2, etc.)
- Two keys + Single key combinations (AB+1, BC+2, etc.)
- Single key + Two keys combinations (A+12, B+23, etc.)
- Two keys + Two keys combinations (AB+12, BC+23, etc.)

## Building and Flashing

1. Build QMK firmware:
```bash
qmk compile -kb kiboard
```

2. Flash firmware while holding BOOTSEL:
```bash
qmk flash -kb kiboard
```

3. Copy CircuitPython and display configuration:
```bash
cp display/touch_config.py /media/CIRCUITPY/
```

## Troubleshooting

1. Display not working
   - Check SPI connections
   - Verify CircuitPython installation
   - Check touch_config.py presence

2. Keys not registering
   - Verify matrix connections
   - Check QMK configuration
   - Rebuild and reflash firmware

## Contributing

1. Fork the repository
2. Create feature branch
3. Submit pull request

## License

This project is licensed under GPL-2.0.
