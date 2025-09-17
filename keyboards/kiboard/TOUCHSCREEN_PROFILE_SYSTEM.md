# KiBoard Always-On Touchscreen Profile Management

## Overview

The KiBoard now features an always-on touchscreen display system that provides persistent access to joystick profile management without requiring entry into configuration mode. This system allows users to easily monitor and change joystick profiles during normal operation.

## Features

### Display Modes

1. **STATUS Mode** (Default)
   - Shows current active joystick profile
   - Displays joystick modes for both sticks
   - Provides touch-to-change instruction
   - Auto-refreshes every 5 seconds

2. **PROFILE_SELECT Mode**
   - Shows all 4 available profiles as selectable buttons
   - Highlights currently active profile
   - Includes configuration button for advanced settings
   - Touch-responsive profile switching

3. **CONFIG Mode**
   - Full configuration interface (existing functionality)
   - Advanced calibration and settings

4. **OFF Mode**
   - Power-saving mode after 30 seconds of inactivity
   - Touch anywhere to wake display

### Joystick Profiles

| Profile | Joy1 Mode | Joy2 Mode | Description |
|---------|-----------|-----------|-------------|
| 0 | Digital | Digital | Both joysticks as hat switches |
| 1 | Analog | Digital | Joy1 analog, Joy2 hat switch |
| 2 | Digital | Analog | Joy1 hat switch, Joy2 analog |
| 3 | Analog | Analog | Both joysticks as analog |

## Touch Interface

### Status Display Touch Zones

- **Anywhere on screen**: Enter profile selection mode

### Profile Selection Touch Zones

- **Profile 1 (10, 50) - (220, 90)**: Digital Both mode
- **Profile 2 (10, 100) - (220, 140)**: Mixed 1 (Analog/Digital)
- **Profile 3 (10, 150) - (220, 190)**: Mixed 2 (Digital/Analog)
- **Profile 4 (10, 200) - (220, 240)**: Analog Both mode
- **Config Button (250, 200) - (310, 240)**: Enter full configuration
- **Outside zones**: Return to status display

## Implementation

### Core Functions

#### Display Management
```c
void init_persistent_display(void);          // Initialize always-on display
void update_display_mode(display_mode_t mode); // Change display mode
void show_profile_status(void);              // Show current profile info
void show_profile_selector(void);            // Show profile selection UI
void update_profile_display(void);           // Handle periodic updates
```

#### Touch Handling
```c
void handle_display_touch(uint16_t x, uint16_t y); // Process touch input
```

#### Profile Information
```c
typedef struct {
    const char* name;        // Profile display name
    const char* joy1_mode;   // Joystick 1 mode description
    const char* joy2_mode;   // Joystick 2 mode description
    uint8_t profile_id;      // Numeric profile identifier
} profile_info_t;
```

### Integration Points

#### Keyboard Initialization
- Display initialization occurs during `keyboard_pre_init_kb()`
- Persistent display mode is set up regardless of config mode state
- Touch handling is integrated into main matrix scanning loop

#### Matrix Scanning
- Touch input checking added to `matrix_scan_kb()`
- Display updates handled during normal operation
- No blocking operations - maintains QMK responsiveness

## User Experience

### Normal Operation Flow

1. **Power On**: Display shows current profile status
2. **Touch Screen**: Immediately opens profile selector
3. **Select Profile**: Touch desired profile, returns to status display
4. **Auto Timeout**: Display dims after 30 seconds, touch to wake
5. **Profile Change**: Instant feedback and joystick mode switching

### Profile Switching

1. Touch screen during normal operation
2. Profile selector appears with current profile highlighted
3. Touch desired profile button
4. Profile changes immediately
5. Display returns to status showing new active profile
6. Joystick modes update for immediate use

### Advanced Configuration

1. Touch "Config" button in profile selector
2. Enters full configuration mode
3. Access to calibration, deadzone settings, etc.
4. Exit returns to normal status display

## Power Management

- **Active Display**: Full brightness and responsiveness
- **Dimmed Mode**: After 30 seconds of inactivity
- **Wake on Touch**: Any touch restores full operation
- **No Sleep Mode**: Display always available for profile info

## Status Indicators

### Profile Status Display
```
KiBoard - Active Profile
Mixed 1
Joy1: Analog
Joy2: Digital
Touch screen to change profile
Hold A+B+C+D for full config
```

### Profile Selector Display
```
Select Profile
[Digital Both]     <-- Highlighted if current
[Mixed 1      ]
[Mixed 2      ]
[Analog Both  ]
[Config]
```

## Technical Notes

### Memory Usage
- Minimal RAM impact: ~200 bytes for display state
- EEPROM storage: Profile settings persist across power cycles
- No dynamic allocation: Fixed data structures

### Performance
- Non-blocking touch checks during matrix scan
- Display updates only when needed
- 5-second status refresh prevents screen burn-in

### Compatibility
- Works with existing QMK functionality
- Preserves all keyboard features
- Compatible with custom keymaps and combos

## Configuration

### Display Settings
```c
#define DISPLAY_TIMEOUT_MS 30000  // Auto-dim timeout
#define STATUS_REFRESH_MS  5000   // Status display refresh rate
```

### Touch Zones (customizable in config_mode.h)
```c
#define TOUCH_ZONE_PROFILE_1_Y 50
#define TOUCH_ZONE_PROFILE_2_Y 100
#define TOUCH_ZONE_PROFILE_3_Y 150
#define TOUCH_ZONE_PROFILE_4_Y 200
#define TOUCH_ZONE_WIDTH      210
#define TOUCH_ZONE_HEIGHT     40
```

## Future Enhancements

1. **Custom Profile Names**: User-definable profile labels
2. **Brightness Control**: Touch-adjustable display brightness
3. **Theme Support**: Color schemes and display layouts
4. **Usage Statistics**: Profile usage tracking and display
5. **Quick Actions**: Direct access to common functions

## Troubleshooting

### Display Issues
- Black screen: Check display initialization and power
- Touch not responsive: Verify touch controller initialization
- Wrong profile shown: Check joystick mode detection functions

### Profile Switching Issues
- Profile doesn't change: Verify analog.c profile functions
- Touch zones wrong: Check touch zone definitions
- Delayed response: Ensure matrix_scan_kb() calls display functions

## Related Files

- `config_mode.h/.c`: Display mode management and touch handling
- `kiboard.c`: Main integration and initialization
- `analog.c/.h`: Joystick profile management
- `keymap.c`: Key combination handling for config mode entry

This system provides an intuitive and always-available interface for managing the KiBoard's dual joystick functionality while maintaining the full power and flexibility of the QMK firmware framework.
