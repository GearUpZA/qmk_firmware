# KiBoard - Keymap and Combo Guide

Comprehensive guide to creating, customizing, and optimizing keymaps and combos for the KiBoard.

## Understanding the KiBoard Layout

### Physical Layout
```
Left Side:  [A] [B] [C] [D]
Right Side: [1] [2] [3] [4]

Joysticks:  Joy1 (left)  Joy2 (right)
Display:    2.8" touchscreen (center/top)
```

### Matrix Configuration
```c
// Physical to matrix mapping
#define MATRIX_ROWS 1
#define MATRIX_COLS 8

// Layout macro
#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, k07 \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07 } \
}
```

## Basic Keymap Creation

### Default Keymap Structure
```c
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,    KC_1,    KC_2,    KC_3,    KC_4
    )
};
```

### Multi-Layer Keymaps
```c
enum layer_names {
    _BASE,      // Layer 0 - Default
    _FUNC,      // Layer 1 - Function keys
    _GAMING,    // Layer 2 - Gaming layout
    _MEDIA      // Layer 3 - Media controls
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,    KC_1,    KC_2,    KC_3,    KC_4
    ),
    [_FUNC] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8
    ),
    [_GAMING] = LAYOUT(
        KC_W,    KC_A,    KC_S,    KC_D,    KC_SPACE,KC_LSFT, KC_LCTL, KC_TAB
    ),
    [_MEDIA] = LAYOUT(
        KC_MPRV, KC_MPLY, KC_MNXT, KC_MSTP, KC_VOLU, KC_VOLD, KC_MUTE, KC_EJCT
    )
};
```

## Layer Management

### Layer Switching Methods

#### Momentary Layer Access
```c
// Hold key for temporary layer access
MO(_FUNC)    // Hold for function layer
MO(_GAMING)  // Hold for gaming layer

// Example usage in keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_A,    KC_B,    KC_C,    MO(_FUNC), KC_1,  KC_2,    KC_3,    KC_4
    ),
    [_FUNC] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   _______,   KC_F5, KC_F6,   KC_F7,   KC_F8
    )
};
```

#### Toggle Layers
```c
// Press to toggle layer on/off
TG(_GAMING)  // Toggle gaming layer
TG(_MEDIA)   // Toggle media layer

// Layer tap - tap for key, hold for layer
LT(_FUNC, KC_SPACE)  // Tap for space, hold for function layer
```

#### Layer Switching
```c
// Switch to specific layer
TO(_GAMING)  // Switch to gaming layer
TO(_BASE)    // Switch back to base layer

// Default layer setting
DF(_BASE)    // Set base as default layer
```

### Layer Indicators
```c
// Visual feedback for current layer
bool led_update_user(led_t led_state) {
    // Update display or LEDs based on current layer
    uint8_t current_layer = get_highest_layer(layer_state);
    update_layer_display(current_layer);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Called when layer changes
    uint8_t layer = get_highest_layer(state);
    
    switch (layer) {
        case _GAMING:
            // Set joysticks for gaming
            set_joystick_mode(1, JOY_MODE_ANALOG);
            set_joystick_mode(2, JOY_MODE_DIGITAL);
            break;
        case _MEDIA:
            // Set joysticks for media control
            set_joystick_mode(1, JOY_MODE_DIGITAL);
            set_joystick_mode(2, JOY_MODE_DIGITAL);
            break;
    }
    
    return state;
}
```

## Combo System Deep Dive

### Combo Fundamentals

#### Basic Combo Definition
```c
// Include combo support
#include "g/keymap_combo.h"

// Define combo keys
const uint16_t PROGMEM combo_name[] = {KC_A, KC_1, COMBO_END};

// Define combo actions
combo_t key_combos[] = {
    COMBO(combo_name, KC_SPACE),  // A+1 produces Space
};

// Required: specify number of combos
uint16_t COMBO_LEN = sizeof(key_combos) / sizeof(combo_t);
```

#### Combo Timing
```c
// In config.h - adjust combo detection window
#define COMBO_TERM 50    // 50ms window for combo detection
#define COMBO_COUNT 50   // Maximum number of combos

// Per-combo timing (advanced)
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case 0:  // First combo
            return 30;  // Faster timing for common combos
        case 1:  // Second combo
            return 80;  // Slower timing for difficult combos
    }
    return COMBO_TERM;  // Default timing
}
```

### Complete Combo System for KiBoard

#### Single + Single Combinations (16 total)
```c
// A with right side
const uint16_t PROGMEM combo_A1[] = {KC_A, KC_1, COMBO_END};
const uint16_t PROGMEM combo_A2[] = {KC_A, KC_2, COMBO_END};
const uint16_t PROGMEM combo_A3[] = {KC_A, KC_3, COMBO_END};
const uint16_t PROGMEM combo_A4[] = {KC_A, KC_4, COMBO_END};

// B with right side
const uint16_t PROGMEM combo_B1[] = {KC_B, KC_1, COMBO_END};
const uint16_t PROGMEM combo_B2[] = {KC_B, KC_2, COMBO_END};
const uint16_t PROGMEM combo_B3[] = {KC_B, KC_3, COMBO_END};
const uint16_t PROGMEM combo_B4[] = {KC_B, KC_4, COMBO_END};

// C with right side
const uint16_t PROGMEM combo_C1[] = {KC_C, KC_1, COMBO_END};
const uint16_t PROGMEM combo_C2[] = {KC_C, KC_2, COMBO_END};
const uint16_t PROGMEM combo_C3[] = {KC_C, KC_3, COMBO_END};
const uint16_t PROGMEM combo_C4[] = {KC_C, KC_4, COMBO_END};

// D with right side
const uint16_t PROGMEM combo_D1[] = {KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_D2[] = {KC_D, KC_2, COMBO_END};
const uint16_t PROGMEM combo_D3[] = {KC_D, KC_3, COMBO_END};
const uint16_t PROGMEM combo_D4[] = {KC_D, KC_4, COMBO_END};
```

#### Two + Single Combinations (24 total)
```c
// Two left + one right
const uint16_t PROGMEM combo_AB1[] = {KC_A, KC_B, KC_1, COMBO_END};
const uint16_t PROGMEM combo_AB2[] = {KC_A, KC_B, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AC1[] = {KC_A, KC_C, KC_1, COMBO_END};
const uint16_t PROGMEM combo_AD1[] = {KC_A, KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_BC1[] = {KC_B, KC_C, KC_1, COMBO_END};
const uint16_t PROGMEM combo_BD1[] = {KC_B, KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_CD1[] = {KC_C, KC_D, KC_1, COMBO_END};
// ... continue pattern for 2, 3, 4
```

#### Combo Actions Assignment
```c
combo_t key_combos[] = {
    // Common actions for single+single
    COMBO(combo_A1, KC_SPACE),     // A+1 = Space
    COMBO(combo_A2, KC_ENTER),     // A+2 = Enter
    COMBO(combo_A3, KC_TAB),       // A+3 = Tab
    COMBO(combo_A4, KC_ESC),       // A+4 = Escape
    
    COMBO(combo_B1, KC_LSFT),      // B+1 = Left Shift
    COMBO(combo_B2, KC_LCTL),      // B+2 = Left Control
    COMBO(combo_B3, KC_LALT),      // B+3 = Left Alt
    COMBO(combo_B4, KC_LGUI),      // B+4 = Left GUI
    
    // Modified keys
    COMBO(combo_C1, LCTL(KC_C)),   // C+1 = Ctrl+C
    COMBO(combo_C2, LCTL(KC_V)),   // C+2 = Ctrl+V
    COMBO(combo_C3, LCTL(KC_Z)),   // C+3 = Ctrl+Z
    COMBO(combo_C4, LCTL(KC_Y)),   // C+4 = Ctrl+Y
    
    // Navigation
    COMBO(combo_D1, KC_HOME),      // D+1 = Home
    COMBO(combo_D2, KC_END),       // D+2 = End
    COMBO(combo_D3, KC_PGUP),      // D+3 = Page Up
    COMBO(combo_D4, KC_PGDN),      // D+4 = Page Down
    
    // Two+one combinations for complex actions
    COMBO(combo_AB1, TG(_GAMING)), // A+B+1 = Toggle gaming layer
    COMBO(combo_CD1, JOY_PROFILE), // C+D+1 = Cycle joystick profiles
};
```

### Advanced Combo Techniques

#### Conditional Combos
```c
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch (combo_index) {
        case 0:  // combo_A1
            // Only trigger in base layer
            return get_highest_layer(layer_state) == _BASE;
        case 10: // combo_AB1
            // Only trigger if not in gaming mode
            return get_highest_layer(layer_state) != _GAMING;
    }
    return true;
}
```

#### Combo Sequences
```c
// Multi-step combo sequences
static uint8_t combo_sequence = 0;

bool process_combo_key_release(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch (combo_index) {
        case COMBO_A1:
            if (combo_sequence == 0) {
                combo_sequence = 1;
                // First step of sequence
                return false;
            }
            break;
        case COMBO_B2:
            if (combo_sequence == 1) {
                combo_sequence = 0;
                // Complete sequence - execute action
                SEND_STRING("Sequence complete!");
                return false;
            }
            break;
    }
    
    // Reset sequence on any other combo
    combo_sequence = 0;
    return true;
}
```

#### Variable Combo Actions
```c
// Different actions based on context
bool process_combo_key_press(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    switch (combo_index) {
        case COMBO_A1:
            if (get_mods() & MOD_MASK_SHIFT) {
                // Shift+A+1 = different action
                tap_code(KC_CAPS);
            } else {
                // A+1 = normal action
                tap_code(KC_SPACE);
            }
            return false;
    }
    return true;
}
```

## Specialized Keymaps

### Gaming Keymap
```c
// Optimized for FPS games
[_GAMING] = LAYOUT(
    KC_W,        // Forward
    KC_A,        // Left
    KC_S,        // Backward  
    KC_D,        // Right
    KC_SPACE,    // Jump
    KC_LSFT,     // Sprint/Crouch
    KC_R,        // Reload
    KC_TAB       // Scoreboard
),

// Gaming-specific combos
COMBO(combo_WS, KC_LCTL),      // W+S = Crouch (impossible movement)
COMBO(combo_AD, KC_E),         // A+D = Interact (impossible movement)
COMBO(combo_SPACE_R, KC_F),    // Space+R = Flashlight
```

### Productivity Keymap
```c
// Office/productivity focused
[_PRODUCTIVITY] = LAYOUT(
    LCTL(KC_Z),    // Undo
    LCTL(KC_X),    // Cut
    LCTL(KC_C),    // Copy
    LCTL(KC_V),    // Paste
    KC_F1,         // Help
    KC_F5,         // Refresh
    LALT(KC_TAB),  // Alt+Tab
    LGUI(KC_L)     // Lock screen
),

// Productivity combos
COMBO(combo_UNDO_REDO, LCTL(KC_Y)),     // Undo+Cut = Redo
COMBO(combo_COPY_PASTE, LCTL(KC_A)),    // Copy+Paste = Select All
COMBO(combo_F1_F5, KC_F12),             // F1+F5 = F12 (Developer tools)
```

### CAD/Design Keymap
```c
// 3D modeling and CAD
[_CAD] = LAYOUT(
    KC_G,        // Grab/Move
    KC_R,        // Rotate
    KC_S,        // Scale
    KC_E,        // Extrude
    KC_1,        // Front view
    KC_3,        // Side view
    KC_7,        // Top view
    KC_0         // Camera view
),

// CAD combos for complex operations
COMBO(combo_GR, LSFT(KC_D)),     // G+R = Duplicate
COMBO(combo_SE, LCTL(KC_J)),     // S+E = Join
COMBO(combo_137, KC_HOME),       // 1+3+7 = Home view
```

### Media Control Keymap
```c
// Audio/video control
[_MEDIA] = LAYOUT(
    KC_MPRV,     // Previous track
    KC_MPLY,     // Play/Pause
    KC_MNXT,     // Next track
    KC_MSTP,     // Stop
    KC_VOLU,     // Volume up
    KC_VOLD,     // Volume down
    KC_MUTE,     // Mute
    KC_EJCT      // Eject
),

// Media combos
COMBO(combo_PREV_NEXT, KC_MRWD),      // Prev+Next = Rewind
COMBO(combo_VOL_UP_DOWN, KC_BRIU),    // Vol+/Vol- = Brightness up
COMBO(combo_PLAY_STOP, KC_WAKE),      // Play+Stop = Wake display
```

## Custom Keycodes and Macros

### Defining Custom Keycodes
```c
enum custom_keycodes {
    CUSTOM_KEY1 = SAFE_RANGE,
    CUSTOM_KEY2,
    MACRO_HELLO,
    MACRO_EMAIL,
    DYNAMIC_MACRO_START
};
```

### Simple Macros
```c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO_HELLO:
            if (record->event.pressed) {
                SEND_STRING("Hello, World!");
            }
            return false;
            
        case MACRO_EMAIL:
            if (record->event.pressed) {
                SEND_STRING("user@example.com");
            }
            return false;
            
        case CUSTOM_KEY1:
            if (record->event.pressed) {
                // Complex sequence
                tap_code(KC_LCTL);
                tap_code(KC_A);
                wait_ms(100);
                tap_code(KC_LCTL);
                tap_code(KC_C);
            }
            return false;
    }
    return true;
}
```

### Dynamic Macros
```c
// Enable dynamic macros in rules.mk
// DYNAMIC_MACRO_ENABLE = yes

// Use in keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_A,    KC_B,     KC_C,     KC_D, 
        DM_REC1, DM_PLY1,  DM_REC2,  DM_PLY2
    )
};

// Combo for macro controls
COMBO(combo_A1, DM_REC1),  // A+1 = Record macro 1
COMBO(combo_A2, DM_PLY1),  // A+2 = Play macro 1
COMBO(combo_B1, DM_REC2),  // B+1 = Record macro 2
COMBO(combo_B2, DM_PLY2),  // B+2 = Play macro 2
COMBO(combo_AB, DM_RSTP),  // A+B = Stop recording
```

### Advanced Macro Techniques

#### Conditional Macros
```c
case SMART_MACRO:
    if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
            // Shift+macro = different action
            SEND_STRING("UPPERCASE TEXT");
        } else if (get_mods() & MOD_MASK_CTRL) {
            // Ctrl+macro = another action
            tap_code(KC_F12);
        } else {
            // Normal macro
            SEND_STRING("normal text");
        }
    }
    return false;
```

#### Application-Specific Macros
```c
// Different macros based on detected application
enum app_types {
    APP_UNKNOWN,
    APP_BROWSER,
    APP_EDITOR,
    APP_GAME
};

static uint8_t current_app = APP_UNKNOWN;

case CONTEXT_MACRO:
    if (record->event.pressed) {
        switch (current_app) {
            case APP_BROWSER:
                tap_code16(LCTL(KC_T));  // New tab
                break;
            case APP_EDITOR:
                tap_code16(LCTL(KC_S));  // Save
                break;
            case APP_GAME:
                tap_code(KC_ESC);        // Pause/Menu
                break;
        }
    }
    return false;
```

## Joystick Integration with Keymaps

### Joystick as Layer Modifiers
```c
// Use joystick directions to modify layer behavior
layer_state_t layer_state_set_user(layer_state_t state) {
    // Check joystick state when changing layers
    if (get_joystick_digital_state(1, JOY_DIR_UP)) {
        // Joystick 1 up = modify layer behavior
        return state | (1UL << _FUNC);
    }
    return state;
}
```

### Joystick Direction Combos
```c
// Define joystick directions as keys for combos
// (Requires custom implementation in analog.c)
#define JOY1_UP    KC_F13
#define JOY1_DOWN  KC_F14
#define JOY1_LEFT  KC_F15
#define JOY1_RIGHT KC_F16

// Combos with joystick directions
const uint16_t PROGMEM combo_A_JOY_UP[] = {KC_A, JOY1_UP, COMBO_END};
const uint16_t PROGMEM combo_JOY_CROSS[] = {JOY1_UP, JOY1_DOWN, COMBO_END};

COMBO(combo_A_JOY_UP, KC_W),      // A + Joystick Up = W
COMBO(combo_JOY_CROSS, KC_SPACE), // Joystick Up+Down = Space
```

## Configuration Management

### Multiple Keymap Profiles
```c
// Store multiple configurations in EEPROM
typedef struct {
    uint8_t combo_assignments[MAX_COMBOS];
    uint8_t layer_preferences;
    uint8_t joystick_modes;
} keymap_config_t;

static keymap_config_t stored_configs[4];  // 4 stored profiles

void load_keymap_profile(uint8_t profile) {
    if (profile < 4) {
        // Load configuration from EEPROM
        eeprom_read_block(&stored_configs[profile], 
                         EECONFIG_USER + (profile * sizeof(keymap_config_t)),
                         sizeof(keymap_config_t));
        apply_keymap_config(&stored_configs[profile]);
    }
}
```

### Real-Time Keymap Editing
```c
// Allow combo reassignment at runtime
bool reassign_combo(uint8_t combo_index, uint16_t new_action) {
    if (combo_index < COMBO_COUNT) {
        // Store new assignment
        runtime_combo_actions[combo_index] = new_action;
        
        // Save to EEPROM for persistence
        eeprom_update_word(EECONFIG_USER_COMBOS + (combo_index * 2), new_action);
        
        return true;
    }
    return false;
}
```

## Optimization and Performance

### Combo Performance
```c
// Optimize combo checking for common patterns
uint8_t quick_combo_check(void) {
    // Use bit manipulation for fast combo detection
    uint8_t pressed_keys = 0;
    if (IS_PRESSED(KC_A)) pressed_keys |= 0x01;
    if (IS_PRESSED(KC_B)) pressed_keys |= 0x02;
    if (IS_PRESSED(KC_C)) pressed_keys |= 0x04;
    if (IS_PRESSED(KC_D)) pressed_keys |= 0x08;
    if (IS_PRESSED(KC_1)) pressed_keys |= 0x10;
    if (IS_PRESSED(KC_2)) pressed_keys |= 0x20;
    if (IS_PRESSED(KC_3)) pressed_keys |= 0x40;
    if (IS_PRESSED(KC_4)) pressed_keys |= 0x80;
    
    // Quick lookup for common combos
    switch (pressed_keys) {
        case 0x11:  // A+1
            return COMBO_A1;
        case 0x22:  // B+2
            return COMBO_B2;
        // ... etc
    }
    return 0;  // No combo
}
```

### Memory Optimization
```c
// Use PROGMEM for large combo arrays
const uint16_t combo_actions[] PROGMEM = {
    KC_SPACE, KC_ENTER, KC_TAB, KC_ESC,
    // ... all combo actions
};

// Access with pgm_read_word
uint16_t get_combo_action(uint8_t index) {
    return pgm_read_word(&combo_actions[index]);
}
```

## Testing and Validation

### Keymap Testing
```c
// Built-in test mode for validating keymaps
void test_all_combos(void) {
    for (uint16_t i = 0; i < COMBO_COUNT; i++) {
        dprintf("Testing combo %d\n", i);
        // Simulate combo trigger
        process_combo(i, true);
        wait_ms(100);
        process_combo(i, false);
        wait_ms(100);
    }
}
```

### Combo Visualization
```c
// Show active combos on display
void display_combo_hints(void) {
    uint8_t pressed = get_pressed_keys();
    
    // Show possible combos with current key state
    for (uint16_t i = 0; i < COMBO_COUNT; i++) {
        if (combo_possible(i, pressed)) {
            display_combo_hint(i);
        }
    }
}
```

This comprehensive guide covers all aspects of creating and customizing keymaps and combos for your KiBoard. The combination of physical keys, joystick directions, and intelligent combo system provides virtually unlimited customization possibilities!
