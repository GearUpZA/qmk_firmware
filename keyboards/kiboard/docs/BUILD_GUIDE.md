# KiBoard - Build Guide

A comprehensive guide to building your KiBoard - a customizable 8-key keyboard with dual analog joysticks and touchscreen configuration.

## Overview

The KiBoard is an innovative input device featuring:
- 8 mechanical switches arranged in two groups (A,B,C,D + 1,2,3,4)
- 2 analog joysticks with profile-based operation modes
- 2.8" ILI9341 touchscreen for real-time configuration
- Raspberry Pi Pico (RP2040) microcontroller
- 94+ possible key combinations through advanced combo system
- Dynamic joystick profiles (digital hat switch ↔ analog modes)

## Required Components

### Electronics
| Component | Quantity | Notes |
|-----------|----------|-------|
| Raspberry Pi Pico | 1 | RP2040-based microcontroller |
| ILI9341 2.8" TFT LCD | 1 | With XPT2046 touch controller |
| Mechanical Switches | 8 | Cherry MX compatible |
| 2-Axis Analog Joysticks | 2 | Standard Arduino-compatible |
| Diodes (1N4148) | 8 | For key matrix |
| Pin Headers | Various | For connections |
| Breadboard/PCB | 1 | For prototyping or permanent build |

### Hardware
| Component | Quantity | Notes |
|-----------|----------|-------|
| Keycaps | 8 | Compatible with your switches |
| Enclosure | 1 | Custom 3D printed or case |
| Wires | Various | 22-26 AWG recommended |
| Resistors (10kΩ) | 2 | Pull-up resistors for rows |

## Pin Connections

### Display Connections (ILI9341)
| Display Pin | Pico GPIO | Notes |
|-------------|-----------|-------|
| VCC | VBUS (5V) | Power |
| GND | GND | Ground |
| CS | GP17 | Chip Select |
| RESET | GP14 | Reset |
| DC | GP15 | Data/Command |
| MOSI | GP19 | SPI Data Out |
| SCK | GP18 | SPI Clock |
| MISO | GP16 | SPI Data In |
| LED | 3.3V | Backlight |

### Touch Controller (XPT2046)
| Touch Pin | Pico GPIO | Notes |
|-----------|-----------|-------|
| T_CS | GP13 | Touch Chip Select |
| T_IRQ | GP12 | Touch Interrupt |

### Keyboard Matrix
| Switch | Pico GPIO | Notes |
|--------|-----------|-------|
| Row 1 | GP8 | Keys A,B,C,D |
| Row 2 | GP9 | Keys 1,2,3,4 |
| Col A | GP0 | Switch A |
| Col B | GP1 | Switch B |
| Col C | GP2 | Switch C |
| Col D | GP3 | Switch D |
| Col 1 | GP4 | Switch 1 |
| Col 2 | GP5 | Switch 2 |
| Col 3 | GP6 | Switch 3 |
| Col 4 | GP7 | Switch 4 |

### Analog Joysticks
| Function | Pico GPIO | Notes |
|----------|-----------|-------|
| Joystick 1 X | GP26 | ADC0 |
| Joystick 1 Y | GP27 | ADC1 |
| Joystick 2 X | GP28 | ADC2 |
| Joystick 2 Y | GP29 | ADC3 |
| VCC | 3.3V | Power for both joysticks |
| GND | GND | Ground for both joysticks |

## Assembly Steps

### Step 1: Prepare the Pico
1. Solder pin headers to the Raspberry Pi Pico
2. Ensure all GPIO pins are accessible
3. Test the Pico with a simple blink program

### Step 2: Wire the Display
1. Connect the ILI9341 display according to the pin table
2. **Important**: Connect VCC to VBUS (5V), not 3.3V
3. Connect the touch controller pins
4. Test display functionality

### Step 3: Build the Key Matrix
1. Install mechanical switches in your chosen layout
2. Wire the matrix:
   - Connect all switches in row 1 to GP8
   - Connect all switches in row 2 to GP9
   - Connect each column to its respective GPIO pin
   - Add diodes to prevent ghosting (cathode toward column)

### Step 4: Install Joysticks
1. Mount joysticks in accessible positions
2. Connect power (3.3V) and ground
3. Wire X/Y axes to the designated ADC pins
4. Ensure joysticks have full range of motion

### Step 5: Final Assembly
1. Secure all components in enclosure
2. Route wires neatly to prevent interference
3. Test all connections before final assembly
4. Install keycaps and test mechanical feel

## Testing Hardware

### Basic Connectivity Test
```bash
# Test Pico connectivity
python -c "import board; print('Pico detected!')"
```

### Display Test
1. Load CircuitPython on the Pico
2. Copy test display code to verify screen function
3. Test touch responsiveness

### Key Matrix Test
1. Use a multimeter to verify continuity
2. Test each key press registers correctly
3. Verify no ghosting occurs

### Joystick Test
1. Use analog read functions to verify full range
2. Check center position stability
3. Test both joysticks independently

## Troubleshooting

### Display Issues
- **No display**: Check VCC connection (should be VBUS/5V)
- **Garbled display**: Verify SPI connections
- **No touch**: Check touch controller wiring

### Key Issues
- **Key not registering**: Check matrix wiring and diodes
- **Multiple keys register**: Check for shorts in matrix
- **Ghosting**: Verify diode orientation

### Joystick Issues
- **No analog input**: Check ADC pin connections
- **Jittery readings**: Add capacitors for noise filtering
- **Limited range**: Verify power supply is stable 3.3V

### Power Issues
- **Device not recognized**: Check USB connection
- **Intermittent operation**: Verify power supply capacity
- **Display dim**: Check backlight connection

## Enclosure Design

### 3D Printing Considerations
- Design for switch mounting (14x14mm holes)
- Include joystick mounting points
- Provide display cutout (exact dimensions needed)
- Consider cable management
- Include ventilation for electronics

### Alternative Enclosures
- Modified project boxes
- Laser-cut acrylic panels
- Custom PCB as structural element
- Modular designs for easy modifications

## Safety Notes

- Always disconnect power when making connections
- Use proper ESD precautions when handling electronics
- Double-check polarity on power connections
- Test with multimeter before applying power
- Use appropriate gauge wire for current requirements

## Next Steps

After successful hardware assembly:
1. Follow the [Firmware Installation Guide](FIRMWARE_GUIDE.md)
2. Configure your keymap using the [Configuration Guide](CONFIGURATION_GUIDE.md)
3. Explore advanced features in the [Advanced Usage Guide](ADVANCED_USAGE.md)

## Support

For build questions or issues:
- Check the troubleshooting section above
- Review connection diagrams carefully
- Test components individually before integration
- Document your specific build for future reference
