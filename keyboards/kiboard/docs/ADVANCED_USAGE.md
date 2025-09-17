# KiBoard - Advanced Usage Guide

Advanced features, customization, and power-user techniques for the KiBoard.

## Advanced Input Techniques

### Chord Typing

The KiBoard's combo system enables chord-based typing where multiple keys pressed simultaneously produce different outputs.

#### Stenographic-Style Input
```c
// Common word combinations
COMBO(combo_AB12, SEND_STRING("the"))     // A+B+1+2 = "the"
COMBO(combo_CD34, SEND_STRING("and"))     // C+D+3+4 = "and"
COMBO(combo_AC13, SEND_STRING("that"))    // A+C+1+3 = "that"

// Programming constructs
COMBO(combo_AB1, SEND_STRING("function")) // A+B+1 = "function"
COMBO(combo_CD2, SEND_STRING("return"))   // C+D+2 = "return"
COMBO(combo_AD3, SEND_STRING("class"))    // A+D+3 = "class"
```

#### Speed Typing Techniques
1. **Muscle Memory**: Practice specific chord combinations
2. **Common Patterns**: Assign frequent words to easy chords
3. **Context Switching**: Use layers for different typing contexts

### Multi-Modal Input

#### Context-Aware Profiles
```c
// Profile switching based on application
enum app_contexts {
    CONTEXT_GENERAL,
    CONTEXT_GAMING,
    CONTEXT_CAD,
    CONTEXT_MEDIA
};

void switch_to_context(uint8_t context) {
    switch(context) {
        case CONTEXT_GAMING:
            set_joystick_mode(1, JOY_MODE_ANALOG);   // Camera control
            set_joystick_mode(2, JOY_MODE_DIGITAL);  // UI navigation
            layer_move(GAMING_LAYER);
            break;
        case CONTEXT_CAD:
            set_joystick_mode(1, JOY_MODE_ANALOG);   // Viewport navigation
            set_joystick_mode(2, JOY_MODE_ANALOG);   // Tool control
            layer_move(CAD_LAYER);
            break;
    }
}
```

#### Automatic Context Detection
```c
// Detect active application and switch profiles
// Requires host-side software or specific key sequences
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t app_detect_timer;
    
    // Detect specific application shortcuts
    if (keycode == LALT(KC_TAB)) {
        app_detect_timer = timer_read();
        // Switch to appropriate profile after app switch
    }
    
    return true;
}
```

## Advanced Joystick Features

### Custom Analog Processing

#### Precision Control
```c
// Variable sensitivity based on joystick position
void send_precision_analog(int16_t x, int16_t y) {
    int16_t distance = sqrt((x * x) + (y * y));
    
    // Higher precision near center
    float sensitivity = (distance < 200) ? 0.5 : 2.0;
    
    int8_t mouse_x = (int8_t)((x - JOYSTICK_CENTER) * sensitivity / 8);
    int8_t mouse_y = (int8_t)((y - JOYSTICK_CENTER) * sensitivity / 8);
    
    // Send adjusted values
    report_mouse_t mouse_report = pointing_device_get_report();
    mouse_report.x = mouse_x;
    mouse_report.y = mouse_y;
    pointing_device_set_report(mouse_report);
    pointing_device_send();
}
```

#### Acceleration Curves
```c
// Non-linear response for better control
int8_t apply_acceleration_curve(int16_t raw_value) {
    int16_t normalized = raw_value - JOYSTICK_CENTER;
    
    if (abs(normalized) < 50) return 0;  // Deadzone
    
    // Cubic response for fine control at low values
    float normalized_float = (float)normalized / 512.0;
    float curved = normalized_float * normalized_float * normalized_float;
    
    return (int8_t)(curved * 127.0);
}
```

### Digital Mode Enhancements

#### Directional Combos
```c
// Use joystick directions in combos with physical keys
const uint16_t PROGMEM combo_A_JOY1_UP[] = {KC_A, KC_F13, COMBO_END};
const uint16_t PROGMEM combo_B_JOY2_LEFT[] = {KC_B, KC_F19, COMBO_END};

// Advanced directional sequences
const uint16_t PROGMEM combo_joy_konami[] = {
    KC_F13, KC_F13, KC_F14, KC_F14,  // Up, Up, Down, Down
    KC_F15, KC_F16, KC_F15, KC_F16,  // Left, Right, Left, Right
    COMBO_END
};
COMBO(combo_joy_konami, SECRET_FUNCTION);
```

#### Gesture Recognition
```c
// Track joystick movement patterns
typedef struct {
    uint8_t sequence[8];
    uint8_t length;
    uint32_t timestamp;
} gesture_t;

gesture_t current_gesture = {0};

void track_joystick_gesture(uint8_t direction) {
    if (timer_elapsed32(current_gesture.timestamp) > 2000) {
        // Reset gesture if too much time passed
        current_gesture.length = 0;
    }
    
    if (current_gesture.length < 8) {
        current_gesture.sequence[current_gesture.length++] = direction;
        current_gesture.timestamp = timer_read32();
        
        check_gesture_complete();
    }
}
```

## Advanced Combo Techniques

### Conditional Combos

#### State-Dependent Combos
```c
// Different combos based on current layer/mode
bool process_combo_key_release(uint16_t combo_index, combo_t *combo, 
                              uint16_t keycode, keyrecord_t *record) {
    switch(combo_index) {
        case COMBO_A1:
            if (get_highest_layer(layer_state) == GAMING_LAYER) {
                tap_code(KC_SPACE);  // Jump in gaming
            } else {
                tap_code(KC_ENTER);  // Enter in normal mode
            }
            return false;
    }
    return true;
}
```

#### Time-Based Combos
```c
// Different actions based on hold time
static uint16_t combo_timer;

bool process_combo_key_press(uint16_t combo_index, combo_t *combo,
                            uint16_t keycode, keyrecord_t *record) {
    if (combo_index == COMBO_A1) {
        combo_timer = timer_read();
    }
    return true;
}

bool process_combo_key_release(uint16_t combo_index, combo_t *combo,
                              uint16_t keycode, keyrecord_t *record) {
    if (combo_index == COMBO_A1) {
        if (timer_elapsed(combo_timer) > 500) {
            // Long press action
            SEND_STRING("Long press!");
        } else {
            // Short press action
            tap_code(KC_SPACE);
        }
    }
    return false;
}
```

### Sequential Combos

#### Command Sequences
```c
// Multi-step command entry
enum sequence_state {
    SEQ_IDLE,
    SEQ_COMMAND_MODE,
    SEQ_PARAMETER_MODE
};

static uint8_t current_sequence = SEQ_IDLE;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(current_sequence) {
        case SEQ_COMMAND_MODE:
            if (keycode == KC_A && record->event.pressed) {
                // 'A' command - enter parameter mode
                current_sequence = SEQ_PARAMETER_MODE;
                SEND_STRING("Command A - Enter parameter: ");
                return false;
            }
            break;
        case SEQ_PARAMETER_MODE:
            if (keycode == KC_1 && record->event.pressed) {
                // Parameter 1
                execute_command_a_param_1();
                current_sequence = SEQ_IDLE;
                return false;
            }
            break;
    }
    return true;
}
```

## Display Integration

### Real-Time Feedback

#### Status Display
```c
// Show current configuration on display
void update_status_display(void) {
    char status_line[32];
    
    // Show joystick modes
    snprintf(status_line, sizeof(status_line), 
             "J1:%s J2:%s", 
             (joy1_mode == JOY_MODE_ANALOG) ? "ANA" : "DIG",
             (joy2_mode == JOY_MODE_ANALOG) ? "ANA" : "DIG");
    
    display_text(0, 0, status_line);
    
    // Show current layer
    snprintf(status_line, sizeof(status_line), 
             "Layer: %d", get_highest_layer(layer_state));
    display_text(0, 20, status_line);
}
```

#### Visual Combo Feedback
```c
// Show combo state on display
void display_combo_state(void) {
    static uint8_t pressed_keys = 0;
    
    // Track which keys are currently pressed
    if (IS_PRESSED(KC_A)) pressed_keys |= 1;
    if (IS_PRESSED(KC_B)) pressed_keys |= 2;
    if (IS_PRESSED(KC_C)) pressed_keys |= 4;
    if (IS_PRESSED(KC_D)) pressed_keys |= 8;
    
    // Visual representation of pressed keys
    draw_key_state(pressed_keys);
    
    // Show potential combos
    show_available_combos(pressed_keys);
}
```

### Custom UI Elements

#### Configuration Menus
```c
// Touch-based configuration interface
typedef struct {
    char* label;
    void (*action)(void);
} menu_item_t;

menu_item_t main_menu[] = {
    {"Joystick Profiles", show_joystick_menu},
    {"Combo Editor", show_combo_menu},
    {"Layer Settings", show_layer_menu},
    {"Save Config", save_configuration},
    {NULL, NULL}
};

void show_main_menu(void) {
    for (int i = 0; main_menu[i].label != NULL; i++) {
        draw_menu_item(i, main_menu[i].label);
    }
}
```

## Performance Optimization

### Efficient Combo Processing

#### Optimized Combo Detection
```c
// Use bit manipulation for faster combo detection
#define KEY_A_BIT (1 << 0)
#define KEY_B_BIT (1 << 1)
#define KEY_C_BIT (1 << 2)
#define KEY_D_BIT (1 << 3)
#define KEY_1_BIT (1 << 4)
#define KEY_2_BIT (1 << 5)
#define KEY_3_BIT (1 << 6)
#define KEY_4_BIT (1 << 7)

static uint8_t current_keys = 0;

void fast_combo_check(void) {
    // Update current key state
    current_keys = 0;
    if (IS_PRESSED(KC_A)) current_keys |= KEY_A_BIT;
    if (IS_PRESSED(KC_B)) current_keys |= KEY_B_BIT;
    // ... etc
    
    // Fast combo lookup using bit patterns
    switch(current_keys) {
        case (KEY_A_BIT | KEY_1_BIT):
            execute_combo_A1();
            break;
        case (KEY_B_BIT | KEY_2_BIT):
            execute_combo_B2();
            break;
        // ... etc
    }
}
```

### Memory Management

#### Efficient Data Structures
```c
// Use packed structures to save memory
typedef struct __attribute__((packed)) {
    uint8_t key_pattern;
    uint16_t action;
} compact_combo_t;

// Store combos in flash memory
const compact_combo_t combos[] PROGMEM = {
    {KEY_A_BIT | KEY_1_BIT, KC_SPACE},
    {KEY_B_BIT | KEY_2_BIT, KC_ENTER},
    // ...
};
```

## Integration with External Systems

### Host Software Integration

#### Custom HID Reports
```c
// Send custom data to host applications
typedef struct {
    uint8_t joystick_modes;
    uint8_t current_profile;
    uint16_t joystick_values[4];
} kiboard_report_t;

void send_kiboard_report(void) {
    kiboard_report_t report = {
        .joystick_modes = (joy1_mode << 4) | joy2_mode,
        .current_profile = current_profile,
        .joystick_values = {joy1_x, joy1_y, joy2_x, joy2_y}
    };
    
    raw_hid_send((uint8_t*)&report, sizeof(report));
}
```

#### Host Configuration Software
```python
# Python script to communicate with KiBoard
import hid

class KiBoardConfig:
    def __init__(self):
        self.device = hid.device()
        self.device.open(0xFEED, 0x0000)  # VID/PID from info.json
    
    def set_joystick_profile(self, profile):
        # Send configuration command
        report = [0x01, profile]  # Command ID 0x01 = set profile
        self.device.write(report)
    
    def get_status(self):
        # Request status report
        self.device.write([0x02])  # Command ID 0x02 = get status
        return self.device.read(64)
```

### Game Integration

#### Game-Specific Profiles
```c
// Automatic profile switching for specific games
typedef struct {
    char* process_name;
    uint8_t joystick1_mode;
    uint8_t joystick2_mode;
    uint8_t layer;
} game_profile_t;

const game_profile_t game_profiles[] = {
    {"csgo.exe", JOY_MODE_ANALOG, JOY_MODE_DIGITAL, FPS_LAYER},
    {"blender.exe", JOY_MODE_ANALOG, JOY_MODE_ANALOG, CAD_LAYER},
    {"spotify.exe", JOY_MODE_DIGITAL, JOY_MODE_DIGITAL, MEDIA_LAYER},
};
```

## Troubleshooting Advanced Features

### Debug Output

#### Comprehensive Logging
```c
// Debug output for advanced features
void debug_print_state(void) {
    dprintf("Joystick modes: J1=%d J2=%d\n", joy1_mode, joy2_mode);
    dprintf("Current layer: %d\n", get_highest_layer(layer_state));
    dprintf("Pressed keys: %08b\n", current_keys);
    dprintf("Joystick values: J1(%d,%d) J2(%d,%d)\n", 
            joy1_x, joy1_y, joy2_x, joy2_y);
}
```

### Performance Monitoring

#### Timing Analysis
```c
// Monitor scan rate and combo processing time
static uint32_t last_scan_time = 0;
static uint16_t scan_times[100];
static uint8_t scan_index = 0;

void monitor_performance(void) {
    uint32_t current_time = timer_read32();
    uint32_t scan_time = current_time - last_scan_time;
    
    scan_times[scan_index] = scan_time;
    scan_index = (scan_index + 1) % 100;
    
    // Calculate average scan time
    if (scan_index == 0) {
        uint32_t total = 0;
        for (int i = 0; i < 100; i++) {
            total += scan_times[i];
        }
        dprintf("Average scan time: %lu ms\n", total / 100);
    }
    
    last_scan_time = current_time;
}
```

## Community and Sharing

### Configuration Sharing

#### Export/Import System
```c
// Export current configuration as JSON
void export_config(void) {
    // Generate JSON representation of current config
    char* json = generate_config_json();
    save_to_file("kiboard_config.json", json);
}

// Import configuration from JSON
bool import_config(char* json_data) {
    config_t new_config;
    if (parse_config_json(json_data, &new_config)) {
        apply_configuration(&new_config);
        return true;
    }
    return false;
}
```

### Community Profiles

#### Shared Configurations
- **Gaming Profiles**: Optimized for different game genres
- **Productivity Profiles**: Designed for specific applications
- **Accessibility Profiles**: Customized for different needs
- **Artistic Profiles**: For creative applications

## Future Expansion

### Hardware Additions

#### Additional Sensors
```c
// Framework for additional input devices
typedef struct {
    uint8_t sensor_type;
    void (*init_function)(void);
    uint16_t (*read_function)(void);
    void (*process_function)(uint16_t value);
} sensor_config_t;

// Support for expansion modules
void register_sensor(sensor_config_t* sensor) {
    // Add to sensor list for processing
}
```

#### Wireless Connectivity
```c
// Framework for wireless features
enum wireless_modes {
    WIRELESS_OFF,
    WIRELESS_BLUETOOTH,
    WIRELESS_2_4GHZ,
    WIRELESS_WIFI
};

void configure_wireless(uint8_t mode) {
    // Set up wireless communication
}
```

This advanced usage guide provides comprehensive techniques for getting the most out of your KiBoard. The modular design and powerful QMK foundation enable endless customization possibilities.
