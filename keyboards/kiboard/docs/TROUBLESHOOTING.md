# KiBoard - Troubleshooting Guide

Comprehensive troubleshooting guide for hardware, firmware, and configuration issues.

## Quick Diagnostics

### Basic Functionality Check
1. **Power**: USB LED lights up, device is detected by computer
2. **Keys**: All 8 mechanical keys register presses
3. **Joysticks**: Both joysticks respond to movement
4. **Display**: Screen shows content and responds to touch
5. **Profiles**: Joystick profile switching works

### Diagnostic Commands
```bash
# Check if device is detected
lsusb | grep -i "feed"          # Linux
system_profiler SPUSBDataType   # macOS

# Monitor QMK console output
qmk console

# Test keyboard input
# Use online keyboard tester or text editor
```

## Hardware Issues

### Power Problems

#### Device Not Detected
**Symptoms**: Computer doesn't recognize KiBoard
**Causes**: 
- Faulty USB cable
- Power supply issues
- Driver problems
- Hardware damage

**Solutions**:
```bash
# Try different USB ports
# Test with known-good USB cable
# Check device manager (Windows) or system info

# Windows - check device manager
devmgmt.msc

# Linux - check USB devices
dmesg | tail -20
lsusb -v
```

#### Intermittent Power Loss
**Symptoms**: Device randomly disconnects/reconnects
**Causes**:
- Loose connections
- Insufficient power supply
- EMI interference

**Solutions**:
1. Check all connections for loose wires
2. Use powered USB hub if needed
3. Avoid interference sources (wireless devices, motors)
4. Add capacitors for power filtering

### Key Matrix Issues

#### Keys Not Registering
**Symptoms**: Some keys don't respond to presses
**Diagnostic**:
```c
// Add to keymap.c for debugging
void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_matrix = true;
}
```

**Common Causes**:
- Broken solder joints
- Faulty diodes
- Wrong matrix wiring
- Switch failure

**Solutions**:
1. **Test continuity** with multimeter
2. **Check diode orientation** (cathode toward column)
3. **Resolder joints** if necessary
4. **Replace faulty switches**

#### Ghosting/Multiple Key Registration
**Symptoms**: Pressing one key registers multiple keys
**Causes**:
- Missing or incorrect diodes
- Short circuits in matrix
- Electrical interference

**Solutions**:
```c
// Verify matrix configuration
#define MATRIX_ROWS 1
#define MATRIX_COLS 8

// Check diode installation
// Each key needs diode from switch to column
```

#### Stuck Keys
**Symptoms**: Keys appear permanently pressed
**Causes**:
- Mechanical switch stuck
- Short circuit
- Software issue

**Diagnostic**:
```bash
# Monitor which keys are "stuck"
qmk console
# Look for continuous key press events
```

### Joystick Problems

#### No Analog Response
**Symptoms**: Joysticks don't produce analog output
**Diagnostic**:
```c
// Add debugging to analog.c
void matrix_scan_kb(void) {
    int16_t x1 = analogReadPin(GP26);
    int16_t y1 = analogReadPin(GP27);
    
    dprintf("Joy1: X=%d Y=%d\n", x1, y1);
    
    matrix_scan_user();
}
```

**Common Causes**:
- Wrong pin connections
- Power supply issues
- Faulty joystick modules
- ADC configuration errors

**Solutions**:
1. **Verify pin connections**:
   - GP26: Joystick 1 X-axis
   - GP27: Joystick 1 Y-axis
   - GP28: Joystick 2 X-axis
   - GP29: Joystick 2 Y-axis

2. **Check power supply**:
   - 3.3V to VCC pins
   - GND to GND pins
   - Stable voltage under load

3. **Test joystick modules**:
   ```c
   // Simple test - should read ~512 at center
   int16_t center_x = analogReadPin(GP26);
   int16_t center_y = analogReadPin(GP27);
   ```

#### Jittery/Noisy Readings
**Symptoms**: Joystick values fluctuate when not moving
**Causes**:
- Electrical noise
- Poor connections
- Low-quality joystick modules

**Solutions**:
```c
// Add software filtering
#define NOISE_THRESHOLD 10

int16_t filter_noise(int16_t raw_value, int16_t previous_value) {
    if (abs(raw_value - previous_value) < NOISE_THRESHOLD) {
        return previous_value;  // Ignore small changes
    }
    return raw_value;
}

// Hardware solutions:
// - Add capacitors (10-100nF) across analog inputs
// - Use shielded cables
// - Improve grounding
```

#### Limited Range
**Symptoms**: Joystick doesn't reach full 0-1023 range
**Causes**:
- Mechanical limitations
- Voltage reference issues
- Calibration needed

**Solutions**:
```c
// Calibration routine
typedef struct {
    int16_t min_x, max_x, center_x;
    int16_t min_y, max_y, center_y;
} joystick_cal_t;

void calibrate_joystick(joystick_cal_t* cal) {
    // Prompt user to move joystick to extremes
    // Record min/max values
    // Save to EEPROM
}
```

### Display Issues

#### Blank/Black Screen
**Symptoms**: Display shows nothing
**Causes**:
- Power issues
- SPI connection problems
- Wrong pin configuration
- Display module failure

**Diagnostic Steps**:
1. **Check power**: 5V to VCC, GND connected
2. **Verify SPI connections**:
   - GP18: SCK (Clock)
   - GP19: MOSI (Data Out)
   - GP16: MISO (Data In)
   - GP17: CS (Chip Select)
   - GP15: DC (Data/Command)
   - GP14: RESET

3. **Test with simple code**:
   ```python
   # CircuitPython test
   import board
   import displayio
   from adafruit_ili9341 import ILI9341
   
   spi = board.SPI()
   display_bus = displayio.FourWire(spi, command=board.GP15, chip_select=board.GP17, reset=board.GP14)
   display = ILI9341(display_bus, width=320, height=240)
   ```

#### Garbled Display
**Symptoms**: Display shows corrupted/wrong content
**Causes**:
- SPI timing issues
- Interference
- Wrong display initialization

**Solutions**:
```c
// Adjust SPI speed
#define SPI_FREQUENCY 1000000  // Start with 1MHz, increase if stable

// Add delays in initialization
void display_init(void) {
    reset_display();
    wait_ms(100);
    send_init_commands();
    wait_ms(50);
}
```

#### Touch Not Working
**Symptoms**: Display shows content but doesn't respond to touch
**Causes**:
- Touch controller not initialized
- Wrong pin connections
- Calibration issues

**Diagnostic**:
```python
# Test touch controller separately
from adafruit_xpt2046 import XPT2046

touch = XPT2046(spi, cs=board.GP13, irq=board.GP12)
while True:
    point = touch.touch_point
    if point:
        print(f"Touch at: {point}")
```

## Firmware Issues

### Compilation Errors

#### Missing Dependencies
**Error**: `'quantum.h' file not found`
**Solution**:
```bash
# Ensure QMK is properly set up
qmk setup
qmk doctor

# Check QMK installation
qmk config user.keyboard=kiboard
```

#### Undefined References
**Error**: `undefined reference to 'function_name'`
**Causes**:
- Missing source files
- Wrong function names
- Include path issues

**Solutions**:
```makefile
# Check rules.mk includes all necessary files
SRC += analog.c config_mode.c

# Verify function declarations in headers
# Check spelling and case sensitivity
```

#### Memory Overflow
**Error**: `region 'FLASH' overflowed`
**Causes**:
- Too many combos
- Large keymap arrays
- Debug code in release build

**Solutions**:
```c
// Reduce combo count
// Disable unused features in rules.mk
CONSOLE_ENABLE = no
COMMAND_ENABLE = no

// Use PROGMEM for large arrays
const uint16_t PROGMEM large_array[] = {
    // data
};
```

### Runtime Errors

#### Keyboard Not Responding
**Symptoms**: Firmware loads but no key presses register
**Diagnostic**:
```bash
# Check QMK console for errors
qmk console

# Look for matrix scan issues
# Check for infinite loops in code
```

**Common Fixes**:
1. **Reset to defaults**: Flash known-good firmware
2. **Check matrix configuration**: Verify pin assignments
3. **Test individual components**: Keys, joysticks separately

#### Combo System Not Working
**Symptoms**: Individual keys work but combos don't trigger
**Causes**:
- Wrong combo definitions
- Timing issues
- Conflicting key mappings

**Debug Code**:
```c
// Add combo debugging
bool process_combo_key_press(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    dprintf("Combo %d triggered with key %d\n", combo_index, keycode);
    return true;
}

// Check combo timing
#define COMBO_TERM 50  // Adjust as needed
```

#### Profile Switching Not Working
**Symptoms**: Joystick profiles don't change
**Diagnostic**:
```c
// Add debug output to profile functions
void cycle_joystick_profiles(void) {
    dprintf("Current profile: J1=%d J2=%d\n", joy1_mode, joy2_mode);
    // ... rest of function
}
```

### Configuration Issues

#### Settings Not Persisting
**Symptoms**: Configuration resets after power cycle
**Causes**:
- EEPROM not enabled
- Wrong EEPROM addresses
- Corruption during write

**Solutions**:
```c
// Enable EEPROM in config.h
#define EEPROM_ENABLE

// Use proper EEPROM functions
void save_settings(void) {
    eeprom_update_byte(EECONFIG_USER, current_profile);
    eeconfig_update_user(settings_data);
}

void load_settings(void) {
    if (eeconfig_is_enabled()) {
        current_profile = eeprom_read_byte(EECONFIG_USER);
        settings_data = eeconfig_read_user();
    }
}
```

#### Configuration Mode Not Accessible
**Symptoms**: Can't enter touch interface mode
**Causes**:
- Wrong key combination
- Timing issues
- CircuitPython not installed

**Solutions**:
1. **Verify key combination**: A+B+C+D held for 3+ seconds
2. **Check CircuitPython installation**
3. **Test individual keys** to ensure all work

## Software Integration Issues

### Host Recognition Problems

#### Device Not Recognized
**Symptoms**: Computer doesn't see keyboard
**Solutions**:
```bash
# Windows - reinstall drivers
# Use Zadig to install WinUSB driver for BOOTSEL mode

# Linux - check permissions
sudo usermod -a -G plugdev $USER
# Log out and back in

# macOS - reset USB preferences
sudo rm /Library/Preferences/SystemConfiguration/com.apple.USBPreferences.plist
```

#### Wrong Device Type
**Symptoms**: Device appears as something other than keyboard
**Causes**:
- Wrong USB descriptor
- Firmware configuration error

**Check Configuration**:
```c
// In config.h
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x0000
#define DEVICE_VER   0x0001
#define MANUFACTURER "Custom"
#define PRODUCT      "KiBoard"
```

### Application-Specific Issues

#### Game Not Recognizing Input
**Causes**:
- Game expects specific device type
- Key mappings not compatible
- Timing issues

**Solutions**:
1. **Use gamepad emulation** for analog joysticks
2. **Map to standard keys** for digital mode
3. **Check game input settings**

#### CAD Software Issues
**Symptoms**: Joystick input not working in CAD applications
**Solutions**:
```c
// Ensure analog mode is active
set_joystick_mode(1, JOY_MODE_ANALOG);
set_joystick_mode(2, JOY_MODE_ANALOG);

// Some CAD software expects specific HID reports
// May need custom HID descriptor
```

## Performance Issues

### Slow Response
**Symptoms**: Noticeable delay between key press and action
**Causes**:
- High debounce values
- Slow scan rate
- Processing overhead

**Optimization**:
```c
// Reduce debounce time
#define DEBOUNCE 1  // From default 5ms

// Optimize matrix scanning
// Remove unnecessary processing from matrix_scan_kb()

// Profile critical code sections
uint32_t start_time = timer_read32();
// ... critical code ...
uint32_t elapsed = timer_elapsed32(start_time);
dprintf("Function took %lu ms\n", elapsed);
```

### High CPU Usage
**Symptoms**: Host computer shows high CPU usage
**Causes**:
- Too frequent reports
- Inefficient processing
- Continuous analog updates

**Solutions**:
```c
// Reduce report frequency
static uint16_t last_report_time = 0;
if (timer_elapsed(last_report_time) > 8) {  // ~125Hz max
    send_report();
    last_report_time = timer_read();
}

// Use change detection for analog values
static int16_t last_x = 0, last_y = 0;
if (abs(current_x - last_x) > threshold || abs(current_y - last_y) > threshold) {
    send_analog_report();
    last_x = current_x;
    last_y = current_y;
}
```

## Emergency Recovery

### Firmware Recovery

#### Device Unresponsive
1. **Enter BOOTSEL mode**:
   - Disconnect USB
   - Hold BOOTSEL button
   - Connect USB
   - Release BOOTSEL after device appears

2. **Flash recovery firmware**:
   ```bash
   # Flash minimal firmware
   qmk flash -kb kiboard -km default
   ```

#### Corrupted Firmware
**Symptoms**: Device powers on but behaves erratically
**Recovery**:
1. **Force BOOTSEL mode** (hardware method)
2. **Flash known-good firmware**
3. **Reset EEPROM**:
   ```c
   void reset_eeprom(void) {
       eeconfig_init();
       soft_reset_keyboard();
   }
   ```

### Hardware Recovery

#### Short Circuit Protection
**If you suspect a short circuit**:
1. **Immediately disconnect power**
2. **Check for obvious shorts** (solder bridges, loose wires)
3. **Test with multimeter** before reconnecting
4. **Use current-limited power supply** for testing

#### Component Replacement
**Order of component testing**:
1. **Raspberry Pi Pico** (most expensive, test last)
2. **Display module** (test with simple code)
3. **Joystick modules** (cheap, easy to replace)
4. **Switches and diodes** (mechanical wear items)

## Prevention and Maintenance

### Regular Checks
- **Monthly**: Clean contacts, check connections
- **Quarterly**: Update firmware, backup configuration
- **Yearly**: Replace wear components (switches, joysticks)

### Best Practices
1. **Use ESD protection** when working on hardware
2. **Keep firmware backups** of working configurations
3. **Document modifications** for future reference
4. **Test changes incrementally** rather than all at once

### Monitoring Tools
```c
// Built-in health monitoring
void system_health_check(void) {
    // Check voltage levels
    uint16_t vcc = analogReadPin(ADC_INTERNAL_VCC);
    if (vcc < minimum_voltage) {
        // Warning: low voltage
    }
    
    // Monitor scan rate
    static uint32_t scan_count = 0;
    static uint32_t last_time = 0;
    scan_count++;
    
    if (timer_elapsed32(last_time) > 1000) {
        dprintf("Scan rate: %lu Hz\n", scan_count);
        scan_count = 0;
        last_time = timer_read32();
    }
}
```

## Getting Help

### Community Resources
- **QMK Discord**: Real-time help with firmware issues
- **GitHub Issues**: Report bugs and feature requests
- **Reddit r/MechanicalKeyboards**: General keyboard help
- **Documentation**: Always check official QMK docs first

### Information to Provide When Asking for Help
1. **Hardware details**: Exact components used
2. **Firmware version**: QMK commit hash
3. **Error messages**: Exact text of any errors
4. **Steps to reproduce**: What you were doing when issue occurred
5. **Attempted solutions**: What you've already tried

### Creating Good Bug Reports
```text
**Hardware Configuration:**
- Raspberry Pi Pico Rev 1.3
- ILI9341 display from vendor X
- Joystick modules: model Y
- Switches: Cherry MX Red

**Firmware:**
- QMK commit: abc123def
- Keymap: custom based on default
- Features enabled: COMBO, ANALOG_DRIVER, JOYSTICK

**Issue Description:**
Clear description of problem

**Steps to Reproduce:**
1. Step one
2. Step two
3. Expected vs actual behavior

**Additional Information:**
- QMK console output
- Photos of wiring (if hardware issue)
- Configuration files
```

This troubleshooting guide should help you diagnose and fix most common issues with your KiBoard. Remember to work systematically and test changes incrementally!
