# KiBoard Joystick Profiles

The KiBoard supports multiple joystick operation modes that can be switched dynamically during use.

## Joystick Modes

### Digital (Hat Switch) Mode
- Each joystick acts as 4 discrete directional buttons
- Provides digital on/off input for each direction (Up, Down, Left, Right)
- Ideal for navigation, menu control, and gaming applications
- Each direction can be used in combos with physical keys

### Analog Mode
- Joystick provides continuous analog values
- Can be used for camera control, mouse movement, or analog gaming controls
- Provides smooth, proportional input based on joystick position
- Includes configurable deadzone to prevent drift

## Available Profiles

The keyboard supports 4 different profile combinations:

### Profile 0 (Default): Both Digital
- **Joystick 1**: Digital (4 directional buttons)
- **Joystick 2**: Digital (4 directional buttons)
- **Total**: 16 digital inputs (8 keys + 8 joystick directions)

### Profile 1: Joy1 Analog, Joy2 Digital
- **Joystick 1**: Analog (mouse/camera control)
- **Joystick 2**: Digital (4 directional buttons)
- **Use case**: FPS gaming, CAD navigation

### Profile 2: Joy1 Digital, Joy2 Analog
- **Joystick 1**: Digital (4 directional buttons)
- **Joystick 2**: Analog (mouse/camera control)
- **Use case**: Media control, presentation remote

### Profile 3: Both Analog
- **Joystick 1**: Analog (primary camera/mouse control)
- **Joystick 2**: Analog (secondary analog input)
- **Use case**: Dual-stick gaming, 3D modeling

## Profile Switching

### Method 1: Key Combination
- Add `JOY_PROFILE` to any key in your keymap
- Press the key to cycle through profiles

### Method 2: Combo (Recommended)
- Create a combo using multiple keys to switch profiles
- Example: `A+D+1+4` to cycle profiles

### Method 3: Function Call
- Use `cycle_joystick_profiles()` in custom code
- Can be triggered by timers, external inputs, etc.

## Digital Mode Key Mappings

In digital mode, joystick directions are mapped to function keys:

### Joystick 1 Digital Mapping
- **Up**: F13
- **Down**: F14
- **Left**: F15
- **Right**: F16

### Joystick 2 Digital Mapping
- **Up**: F17
- **Down**: F18
- **Left**: F19
- **Right**: F20

These can be remapped in your keymap or through the touchscreen configuration interface.

## Analog Mode Configuration

### Mouse Movement (Default)
- Joystick movement translates to mouse cursor movement
- Configurable sensitivity and deadzone
- Suitable for presentation control, media center use

### Custom Analog Processing
- Raw analog values can be processed for custom applications
- Modify `send_joystick_analog()` function in `analog.c`
- Examples: Game controller emulation, MIDI control, custom HID reports

## Technical Details

### Thresholds
- **Digital High Threshold**: 700 (direction considered "pressed")
- **Digital Low Threshold**: 300 (direction considered "pressed")
- **Analog Deadzone**: 50 (prevents drift in center position)
- **Debounce Time**: 50ms (prevents rapid switching)

### ADC Resolution
- 10-bit ADC (0-1023 range)
- Center position: 512
- Full range available for analog processing

## Integration with Combos

Digital joystick directions can be used in combos just like physical keys:

```c
// Example combos with joystick directions
const uint16_t PROGMEM combo_A_JOY1_UP[] = {KC_A, KC_F13, COMBO_END};
const uint16_t PROGMEM combo_JOY1_UP_JOY2_RIGHT[] = {KC_F13, KC_F20, COMBO_END};
```

This allows for complex input combinations using both physical keys and joystick directions.
