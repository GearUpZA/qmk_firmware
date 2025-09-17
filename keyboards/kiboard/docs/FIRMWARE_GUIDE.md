# KiBoard - Firmware Installation Guide

Complete guide for installing and configuring QMK firmware on your KiBoard.

## Prerequisites

### Required Software
- **QMK Toolbox** or **QMK CLI** for flashing
- **Git** for cloning repositories
- **Python 3.8+** for QMK tools
- **Code editor** (VS Code recommended)

### System Setup

#### Windows
```powershell
# Install QMK
pip install qmk
qmk setup

# Install drivers if needed
# Use Zadig for RP2040 BOOTSEL mode
```

#### macOS
```bash
# Install Homebrew first
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install QMK
brew install qmk/qmk/qmk
qmk setup
```

#### Linux
```bash
# Install QMK
python3 -m pip install --user qmk
qmk setup

# Install udev rules for device access
sudo cp util/udev/50-qmk.rules /etc/udev/rules.d/
```

## Firmware Compilation

### Clone QMK with KiBoard Support
```bash
# Clone QMK repository
git clone https://github.com/qmk/qmk_firmware.git
cd qmk_firmware

# Copy KiBoard files to keyboards directory
cp -r /path/to/kiboard keyboards/kiboard
```

### Build Default Firmware
```bash
# Compile default keymap
qmk compile -kb kiboard -km default

# The firmware file will be generated as:
# kiboard_default.uf2
```

### Build Custom Keymap
```bash
# Create custom keymap
qmk new-keymap -kb kiboard -km myname

# Edit your keymap
# keyboards/kiboard/keymaps/myname/keymap.c

# Compile custom keymap
qmk compile -kb kiboard -km myname
```

## Flashing Process

### Method 1: Drag and Drop (Recommended)
1. **Enter BOOTSEL Mode**:
   - Hold the BOOTSEL button on Pico
   - Connect USB cable
   - Release BOOTSEL button
   - Pico appears as USB drive "RPI-RP2"

2. **Flash Firmware**:
   - Drag `kiboard_default.uf2` to the RPI-RP2 drive
   - Pico will automatically reboot with new firmware

### Method 2: QMK Toolbox
1. Open QMK Toolbox
2. Select the `.uf2` file
3. Enter BOOTSEL mode on Pico
4. Click "Flash" in QMK Toolbox

### Method 3: Command Line
```bash
# Flash using QMK CLI
qmk flash -kb kiboard -km default

# Follow prompts to enter BOOTSEL mode
```

## Verification

### Test Basic Functionality
1. **Key Presses**: Test all 8 mechanical keys
2. **Joysticks**: Verify both joysticks respond
3. **Profile Switching**: Test joystick profile cycling
4. **Combos**: Test key combinations work

### Using QMK Console
```bash
# Enable console output for debugging
qmk console

# Press keys and observe output
# Useful for troubleshooting
```

## Configuration Modes

### Normal Operation Mode
- Standard keyboard functionality
- All combos active
- Joystick input processing
- Profile switching available

### Configuration Mode
**Entering Config Mode**:
- Hold A+B+C+D for 3 seconds
- Keyboard restarts in configuration mode
- Touch interface becomes active

**Configuration Features**:
- Visual keymap editing
- Combo customization
- Joystick profile settings
- Real-time preview

## CircuitPython Setup (Display Interface)

### Install CircuitPython
1. Download CircuitPython for RP2040
2. Enter BOOTSEL mode
3. Copy `.uf2` file to RPI-RP2 drive
4. Pico reboots as CIRCUITPY drive

### Install Required Libraries
```bash
# Download libraries from Adafruit
# Copy to CIRCUITPY/lib/
- adafruit_ili9341.mpy
- adafruit_xpt2046.mpy
- adafruit_display_text/
- adafruit_bitmap_font/
```

### Copy Configuration Interface
```bash
# Copy touch interface files
cp display/touch_config.py CIRCUITPY/
cp display/config_ui.py CIRCUITPY/
```

## Dual Boot Setup

Your KiBoard can run both QMK and CircuitPython:

### QMK Mode (Default)
- Full keyboard functionality
- Optimized performance
- All features available

### CircuitPython Mode (Configuration)
- Touch interface active
- Visual configuration
- Real-time keymap editing

### Switching Between Modes
```bash
# Switch to CircuitPython (config mode)
# Hold A+B+C+D for 3 seconds

# Switch back to QMK
# Use reset button or power cycle
```

## Troubleshooting

### Compilation Errors
```bash
# Clean build environment
qmk clean -kb kiboard

# Verify QMK setup
qmk doctor

# Check for missing dependencies
qmk setup --home /path/to/qmk_firmware
```

### Flashing Issues
- **Pico not detected**: Verify BOOTSEL button pressed
- **Permission denied**: Check udev rules (Linux)
- **Flash fails**: Try different USB port or cable

### Runtime Issues
- **Keys not working**: Check matrix wiring
- **Joysticks not responding**: Verify ADC connections
- **Display blank**: Check CircuitPython installation

### Common Error Messages

#### "Device not found"
```bash
# Check if device is detected
lsusb | grep -i raspberry  # Linux
system_profiler SPUSBDataType | grep -i raspberry  # macOS
```

#### "Compilation failed"
- Check file paths in keyboards/kiboard/
- Verify all source files present
- Check for syntax errors in keymap.c

#### "Flash timeout"
- Try different USB port
- Use shorter USB cable
- Check power supply

## Advanced Configuration

### Custom Keymaps
```c
// Example custom keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_1,    KC_2,    KC_3,    KC_4
    ),
    [1] = LAYOUT(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_5,    KC_6,    KC_7,    KC_8
    )
};
```

### Custom Combos
```c
// Define new combos
const uint16_t PROGMEM my_combo[] = {KC_A, KC_B, COMBO_END};
COMBO(my_combo, KC_SPACE);
```

### Joystick Customization
```c
// Custom joystick processing
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case JOY_PROFILE:
            if (record->event.pressed) {
                cycle_joystick_profiles();
            }
            return false;
    }
    return true;
}
```

## Backup and Recovery

### Backup Current Firmware
```bash
# Save current keymap
cp keyboards/kiboard/keymaps/default/keymap.c keymap_backup.c

# Export compiled firmware
cp kiboard_default.uf2 firmware_backup.uf2
```

### Factory Reset
1. Flash original default firmware
2. Reset EEPROM settings
3. Reconfigure as needed

### Recovery Mode
If keyboard becomes unresponsive:
1. Enter BOOTSEL mode
2. Flash known working firmware
3. Test basic functionality
4. Gradually restore customizations

## Performance Optimization

### Scan Rate Optimization
```c
// In config.h
#define DEBOUNCE 5              // Adjust debounce timing
#define COMBO_TERM 50           // Combo detection window
#define TAPPING_TERM 200        // Tap timing
```

### Memory Usage
```bash
# Check firmware size
qmk compile -kb kiboard -km default --verbose

# Optimize if needed:
# - Disable unused features
# - Reduce combo count
# - Optimize code
```

## Next Steps

After successful firmware installation:
1. Follow the [Configuration Guide](CONFIGURATION_GUIDE.md)
2. Explore [Advanced Usage](ADVANCED_USAGE.md)
3. Check out [Joystick Profiles](JOYSTICK_PROFILES.md)
4. Customize your [keymap and combos](KEYMAP_GUIDE.md)
