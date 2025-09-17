# KiBoard

A revolutionary 8-key keyboard with dual analog joysticks and touchscreen configuration interface, powered by Raspberry Pi Pico and QMK firmware.

![KiBoard Layout](docs/images/kiboard_layout.png)

## Features

- **8 Mechanical Keys**: Premium switches in optimized layout (A,B,C,D + 1,2,3,4)
- **Dual Analog Joysticks**: Profile-switchable between digital (hat switch) and analog modes
- **2.8" Touchscreen**: Real-time configuration with visual interface
- **94+ Key Combinations**: Advanced combo system for maximum functionality
- **Dynamic Profiles**: Switch joystick modes on-the-fly for different applications
- **QMK Powered**: Full customization with powerful firmware
- **Raspberry Pi Pico**: Modern RP2040 microcontroller with USB-C

## Quick Start

### 🚀 [Quick Start Guide](docs/QUICK_START.md)
Get up and running in 5 minutes!

### 📚 Documentation
- **[Build Guide](docs/BUILD_GUIDE.md)** - Complete hardware assembly instructions
- **[Firmware Guide](docs/FIRMWARE_GUIDE.md)** - QMK installation and compilation
- **[Configuration Guide](docs/CONFIGURATION_GUIDE.md)** - Keymap and combo setup
- **[Joystick Profiles](docs/JOYSTICK_PROFILES.md)** - Analog/digital mode details
- **[Keymap Guide](docs/KEYMAP_GUIDE.md)** - Advanced keymap customization
- **[Advanced Usage](docs/ADVANCED_USAGE.md)** - Power user features
- **[Troubleshooting](docs/TROUBLESHOOTING.md)** - Common issues and solutions

## Applications

### 🎮 Gaming
- **FPS Games**: Analog joystick for camera control, digital for UI navigation
- **Strategy Games**: Combo commands for complex unit control
- **MMO Gaming**: Macro combos for spell casting and inventory management

### 💼 Productivity
- **CAD/3D Modeling**: Dual analog joysticks for viewport navigation
- **Video Editing**: Timeline scrubbing and tool shortcuts
- **Software Development**: IDE shortcuts and navigation
- **Presentations**: Remote control with analog precision

### 🎨 Creative Applications
- **Digital Art**: Brush control and canvas navigation
- **Music Production**: MIDI control and transport functions
- **Photo Editing**: Precise tool control and slider adjustment

### ♿ Accessibility
- **Reduced Mobility**: Compact layout with maximum functionality
- **One-Handed Operation**: All functions accessible with minimal movement
- **Custom Adaptations**: Fully configurable for specific needs

## Hardware Specifications

| Component | Specification |
|-----------|---------------|
| **Microcontroller** | Raspberry Pi Pico (RP2040) |
| **Keys** | 8x Mechanical switches (Cherry MX compatible) |
| **Joysticks** | 2x 2-axis analog joysticks |
| **Display** | 2.8" ILI9341 TFT with XPT2046 touch controller |
| **Connectivity** | USB-C |
| **Power** | USB bus powered |
| **Dimensions** | ~200mm x 100mm x 40mm |
| **Weight** | ~300g |

## Pin Configuration

### Key Matrix
| Pin | Function |
|-----|----------|
| GP0-GP7 | Column pins (A,B,C,D,1,2,3,4) |
| GP8-GP9 | Row pins |

### Analog Joysticks
| Pin | Function |
|-----|----------|
| GP26 | Joystick 1 X-axis |
| GP27 | Joystick 1 Y-axis |
| GP28 | Joystick 2 X-axis |
| GP29 | Joystick 2 Y-axis |

### Display Interface
| Pin | Function |
|-----|----------|
| GP17 | Display CS |
| GP15 | Display DC |
| GP14 | Display Reset |
| GP18-19 | SPI (SCK/MOSI) |
| GP13 | Touch CS |
| GP12 | Touch IRQ |

## Software Features

### Joystick Profiles
- **Profile 0**: Both joysticks digital (default)
- **Profile 1**: Joy1 analog + Joy2 digital
- **Profile 2**: Joy1 digital + Joy2 analog  
- **Profile 3**: Both joysticks analog

### Combo System
- **16** Single+Single combinations (A+1, B+2, etc.)
- **24** Two+Single combinations (AB+1, CD+2, etc.)
- **24** Single+Two combinations (A+12, B+34, etc.)
- **30** Two+Two combinations (AB+12, CD+34, etc.)

### Advanced Features
- **Real-time configuration** via touchscreen
- **Dynamic macro recording** and playback
- **Layer system** for different application contexts
- **Profile automation** based on active application
- **Custom HID reports** for specialized software integration

## Getting Started

### Prerequisites
- Basic soldering skills (for assembly)
- Computer with USB port
- QMK development environment (for customization)

### Quick Setup
1. **[Build Hardware](docs/BUILD_GUIDE.md)** - Assemble components
2. **[Flash Firmware](docs/FIRMWARE_GUIDE.md)** - Install QMK firmware
3. **[Configure Layout](docs/CONFIGURATION_GUIDE.md)** - Set up your keymap
4. **[Start Using](docs/QUICK_START.md)** - Basic operation guide

## Community

### Resources
- **Documentation**: Comprehensive guides for all aspects
- **Example Configurations**: Pre-made setups for common applications
- **Troubleshooting**: Solutions for common issues
- **Advanced Techniques**: Power user features and optimizations

### Contributing
- **Bug Reports**: Help improve the firmware and documentation
- **Configuration Sharing**: Share your custom setups
- **Feature Requests**: Suggest new capabilities
- **Documentation**: Help expand and improve guides

## License

This project is licensed under GPL-2.0. See [LICENSE](LICENSE) for details.

## Acknowledgments

- **QMK Firmware**: Powerful keyboard firmware foundation
- **Raspberry Pi Foundation**: Excellent RP2040 microcontroller
- **Community Contributors**: Ideas, testing, and feedback

---

**Ready to build your KiBoard?** Start with the [Build Guide](docs/BUILD_GUIDE.md) or jump to [Quick Start](docs/QUICK_START.md) if you have an assembled unit!
