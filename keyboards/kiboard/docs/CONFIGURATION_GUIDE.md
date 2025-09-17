# KiBoard - Configuration Guide

Complete guide for configuring your KiBoard's keymaps, combos, and joystick profiles.

## Overview

The KiBoard offers multiple configuration methods:
- **Touchscreen Interface**: Visual, real-time configuration
- **QMK Keymap Editing**: Code-based customization
- **Dynamic Configuration**: Runtime adjustments
- **Profile Management**: Multiple saved configurations

## Configuration Methods

### Method 1: Touchscreen Interface (Recommended)

#### Entering Configuration Mode
1. **Key Combination**: Hold `A+B+C+D` for 3 seconds
2. **Alternative**: Press programmed `CONFIG` key if available
3. **Status**: Display shows configuration interface

#### Touch Interface Features
- **Visual Keymap Editor**: Drag and drop key assignments
- **Combo Builder**: Create combinations with visual feedback
- **Joystick Profiles**: Switch between analog/digital modes
- **Real-time Testing**: Test changes immediately
- **Save/Load**: Multiple configuration profiles

#### Navigation
- **Main Menu**: Touch zones for different configuration areas
- **Key Editor**: Touch keys to modify assignments
- **Combo Editor**: Select keys to create combinations
- **Settings**: System-wide configuration options

### Method 2: QMK Keymap Files

#### Default Keymap Structure
```c
// keyboards/kiboard/keymaps/default/keymap.c
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,    KC_1,    KC_2,    KC_3,    KC_4
    )
};
```

#### Creating Custom Keymaps
```bash
# Create new keymap
qmk new-keymap -kb kiboard -km myconfig

# Edit the keymap
# keyboards/kiboard/keymaps/myconfig/keymap.c

# Compile and flash
qmk compile -kb kiboard -km myconfig
qmk flash -kb kiboard -km myconfig
```

## Key Assignment Options

### Standard Keys
```c
// Basic alphanumeric
KC_A, KC_B, KC_C, KC_D          // Letters
KC_1, KC_2, KC_3, KC_4          // Numbers

// Modifiers
KC_LSFT, KC_RSFT                // Shift
KC_LCTL, KC_RCTL                // Control
KC_LALT, KC_RALT                // Alt
KC_LGUI, KC_RGUI                // Windows/Cmd key

// Function keys
KC_F1, KC_F2, KC_F3, KC_F4      // Function keys
KC_F13, KC_F14, KC_F15, KC_F16  // Extended function keys
```

### Special Keys
```c
// Media controls
KC_MUTE, KC_VOLU, KC_VOLD       // Volume
KC_MPLY, KC_MPRV, KC_MNXT       // Media playback
KC_MSTP                         // Stop

// Navigation
KC_HOME, KC_END, KC_PGUP, KC_PGDN
KC_UP, KC_DOWN, KC_LEFT, KC_RGHT

// System
KC_PWR, KC_SLEP, KC_WAKE        // Power management
```

### Custom Keys
```c
// Custom keycodes for special functions
enum custom_keycodes {
    JOY_PROFILE = SAFE_RANGE,    // Cycle joystick profiles
    CONFIG_MODE,                 // Enter configuration mode
    MACRO1,                      // Custom macro 1
    MACRO2,                      // Custom macro 2
    CUSTOM_SAFE_RANGE
};
```

## Combo Configuration

### Understanding Combos

Combos allow multiple keys to produce different outputs when pressed together. The KiBoard's 8 keys can create 94 different combinations.

### Combo Types

#### Single + Single (16 combinations)
```c
// Left side + Right side
const uint16_t PROGMEM combo_A1[] = {KC_A, KC_1, COMBO_END};
const uint16_t PROGMEM combo_B2[] = {KC_B, KC_2, COMBO_END};
```

#### Two + Single (24 combinations)
```c
// Two left keys + one right key
const uint16_t PROGMEM combo_AB1[] = {KC_A, KC_B, KC_1, COMBO_END};
const uint16_t PROGMEM combo_CD2[] = {KC_C, KC_D, KC_2, COMBO_END};
```

#### Single + Two (24 combinations)
```c
// One left key + two right keys
const uint16_t PROGMEM combo_A12[] = {KC_A, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_B34[] = {KC_B, KC_3, KC_4, COMBO_END};
```

#### Two + Two (36 combinations)
```c
// Two left keys + two right keys
const uint16_t PROGMEM combo_AB12[] = {KC_A, KC_B, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_CD34[] = {KC_C, KC_D, KC_3, KC_4, COMBO_END};
```

### Combo Actions

#### Basic Key Output
```c
// Combo produces single key
COMBO(combo_A1, KC_SPACE)      // A+1 = Space
COMBO(combo_B2, KC_ENTER)      // B+2 = Enter
```

#### Modified Keys
```c
// Combo with modifiers
COMBO(combo_A1, LCTL(KC_C))    // A+1 = Ctrl+C
COMBO(combo_B2, LALT(KC_TAB))  // B+2 = Alt+Tab
```

#### Custom Actions
```c
// Combo triggers custom function
COMBO(combo_CD12, JOY_PROFILE) // C+D+1+2 = Change joystick profile
```

### Advanced Combo Features

#### Combo Sequences
```c
// Sequential combos for complex actions
if (combo_triggered(combo_A1)) {
    tap_code(KC_ESC);
    wait_ms(100);
    tap_code(KC_ENTER);
}
```

#### Conditional Combos
```c
// Combos that depend on current state
bool process_combo_key_release(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (get_joystick_mode(1) == JOY_MODE_DIGITAL) {
        // Different behavior in digital mode
        return true;
    }
    return false;
}
```

## Joystick Configuration

### Profile Management

#### Available Profiles
1. **Profile 0**: Both joysticks digital (default)
2. **Profile 1**: Joy1 analog, Joy2 digital
3. **Profile 2**: Joy1 digital, Joy2 analog
4. **Profile 3**: Both joysticks analog

#### Profile Switching
```c
// Manual profile switching
set_joystick_mode(1, JOY_MODE_ANALOG);   // Set joystick 1 to analog
set_joystick_mode(2, JOY_MODE_DIGITAL);  // Set joystick 2 to digital

// Cycle through all profiles
cycle_joystick_profiles();
```

### Digital Mode Configuration

#### Threshold Adjustment
```c
// In analog.c - adjust these values
#define JOYSTICK_THRESHOLD_HIGH 700   // Direction active above this
#define JOYSTICK_THRESHOLD_LOW 300    // Direction active below this
#define JOYSTICK_DEBOUNCE_MS 50       // Debounce time
```

#### Key Mapping
```c
// Default digital mappings (can be changed)
// Joystick 1: F13 (Up), F14 (Down), F15 (Left), F16 (Right)
// Joystick 2: F17 (Up), F18 (Down), F19 (Left), F20 (Right)

// Custom mapping example
#define JOY1_UP_KEY KC_W
#define JOY1_DOWN_KEY KC_S
#define JOY1_LEFT_KEY KC_A
#define JOY1_RIGHT_KEY KC_D
```

### Analog Mode Configuration

#### Mouse Sensitivity
```c
// Adjust in send_joystick_analog() function
int8_t mouse_x = (int8_t)((x1 - JOYSTICK_CENTER) / 4);  // Divide by 4 = low sensitivity
int8_t mouse_x = (int8_t)((x1 - JOYSTICK_CENTER) / 2);  // Divide by 2 = high sensitivity
```

#### Deadzone Settings
```c
// Prevent drift in center position
#define ANALOG_JOYSTICK_DEADZONE 50   // Increase for larger deadzone
```

#### Custom Analog Processing
```c
void send_joystick_analog(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    // Custom processing for analog values
    // Example: Game controller emulation
    // Example: MIDI control values
    // Example: Custom HID reports
}
```

## Advanced Configuration

### Macros

#### Simple Macros
```c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO1:
            if (record->event.pressed) {
                SEND_STRING("Hello, World!");
            }
            return false;
        
        case MACRO2:
            if (record->event.pressed) {
                tap_code(KC_LCTL);
                tap_code(KC_C);
                wait_ms(100);
                tap_code(KC_LCTL);
                tap_code(KC_V);
            }
            return false;
    }
    return true;
}
```

#### Dynamic Macros
```c
// Record and playback macros at runtime
// Assign to combo for easy access
COMBO(combo_A4, DM_REC1)       // A+4 = Start recording macro 1
COMBO(combo_B4, DM_PLY1)       // B+4 = Play macro 1
```

### Layer System

#### Multiple Layers
```c
// Define multiple layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Base layer
    [0] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,    KC_1,    KC_2,    KC_3,    KC_4
    ),
    
    // Function layer
    [1] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8
    ),
    
    // Gaming layer
    [2] = LAYOUT(
        KC_W,    KC_A,    KC_S,    KC_D,    KC_SPACE,KC_LSFT, KC_LCTL, KC_TAB
    )
};
```

#### Layer Switching
```c
// Momentary layer activation
MO(1)                          // Hold for layer 1

// Toggle layer
TG(1)                          // Press to toggle layer 1

// Layer tap
LT(1, KC_SPACE)               // Tap for space, hold for layer 1
```

### Configuration Persistence

#### EEPROM Storage
```c
// Save settings to EEPROM
void save_config_to_eeprom(void) {
    eeprom_update_byte(EECONFIG_USER, joystick_profile);
    eeprom_update_word(EECONFIG_USER + 1, custom_settings);
}

// Load settings from EEPROM
void load_config_from_eeprom(void) {
    joystick_profile = eeprom_read_byte(EECONFIG_USER);
    custom_settings = eeprom_read_word(EECONFIG_USER + 1);
}
```

#### Configuration Files
```c
// Save/load configurations as JSON (via touch interface)
{
    "profile_name": "Gaming Setup",
    "joystick_mode_1": "digital",
    "joystick_mode_2": "analog",
    "combos": [
        {"keys": ["A", "1"], "output": "SPACE"},
        {"keys": ["B", "2"], "output": "ENTER"}
    ]
}
```

## Configuration Examples

### Gaming Setup
```c
// WASD movement with joystick camera control
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_W,    KC_A,    KC_S,    KC_D,    KC_SPACE,KC_LSFT, KC_LCTL, KC_R
    )
};

// Set joystick 1 to analog for camera
// Set joystick 2 to digital for UI navigation
```

### Productivity Setup
```c
// Application shortcuts and window management
COMBO(combo_A1, LALT(KC_TAB))     // A+1 = Alt+Tab
COMBO(combo_A2, LGUI(KC_L))       // A+2 = Lock screen
COMBO(combo_B1, LCTL(KC_C))       // B+1 = Copy
COMBO(combo_B2, LCTL(KC_V))       // B+2 = Paste
```

### Media Control Setup
```c
// Media and volume controls
COMBO(combo_A1, KC_MPLY)          // A+1 = Play/Pause
COMBO(combo_A2, KC_MNXT)          // A+2 = Next track
COMBO(combo_B1, KC_VOLU)          // B+1 = Volume up
COMBO(combo_B2, KC_VOLD)          // B+2 = Volume down
```

## Troubleshooting Configuration

### Common Issues

#### Combos Not Working
- Check combo timing (COMBO_TERM setting)
- Verify key definitions match physical layout
- Test individual keys first

#### Joystick Not Responding
- Verify profile settings
- Check analog pin connections
- Test with different threshold values

#### Configuration Mode Not Accessible
- Ensure all 4 keys (A+B+C+D) are pressed
- Check timing (hold for 3+ seconds)
- Verify firmware includes config mode

### Debug Tools

#### QMK Console
```bash
# Monitor key presses and combo detection
qmk console

# Useful output for debugging:
# - Key press events
# - Combo triggers
# - Joystick values
```

#### Test Mode
```c
// Add debug output to your keymap
void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_matrix = true;
    debug_keyboard = true;
}
```

## Configuration Backup

### Save Current Settings
```bash
# Backup keymap file
cp keyboards/kiboard/keymaps/default/keymap.c backup_keymap.c

# Export configuration via touch interface
# Settings -> Export -> Save to file
```

### Restore Settings
```bash
# Restore from backup
cp backup_keymap.c keyboards/kiboard/keymaps/default/keymap.c
qmk compile -kb kiboard -km default
qmk flash -kb kiboard -km default
```

## Next Steps

- Explore [Advanced Usage](ADVANCED_USAGE.md) for power user features
- Check [Joystick Profiles](JOYSTICK_PROFILES.md) for detailed joystick configuration
- Review [Troubleshooting Guide](TROUBLESHOOTING.md) for common issues
- Join the community for configuration sharing and tips
