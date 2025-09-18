/* Copyright 2025 GearUpZA <you@example.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// Layer definitions
enum layers {
    _BASE = 0,
    _DIGITAL,
    _GAMING,
    _CONFIG
};

// Custom keycodes for layer switching and special functions
enum custom_keycodes {
    KC_PROFILE = SAFE_RANGE,
    KC_CONFIG,
    KC_RESET_PROFILE
};

// Base layer - analog joysticks for camera/mouse control
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_A,        KC_B,        KC_C,        KC_D, 
        KC_1,        KC_2,        KC_3,        TO(_DIGITAL)
    ),
    
    // Digital layer - joysticks act as hat switches for navigation
    [_DIGITAL] = LAYOUT(
        KC_W,        KC_A,        KC_S,        KC_D,
        KC_UP,       KC_LEFT,     KC_DOWN,     TO(_GAMING)
    ),
    
    // Gaming layer - optimized for FPS games
    [_GAMING] = LAYOUT(
        KC_LCTL,     KC_LSFT,     KC_SPC,      KC_R,
        KC_1,        KC_2,        KC_3,        TO(_CONFIG)
    ),
    
    // Configuration layer - settings and profiles
    [_CONFIG] = LAYOUT(
        KC_CONFIG,   KC_PROFILE,  KC_RESET_PROFILE, KC_NO,
        KC_F1,       KC_F2,       KC_F3,       TO(_BASE)
    )
};

#ifdef COMBO_ENABLE
// Combo definitions - practical combinations for common actions
enum combo_events {
    COMBO_ESC,      // A+B = Escape
    COMBO_ENTER,    // C+D = Enter  
    COMBO_TAB,      // A+C = Tab
    COMBO_BSPC,     // B+D = Backspace
    COMBO_SPACE,    // 1+2 = Space
    COMBO_SHIFT,    // 2+3 = Shift
    COMBO_CTRL,     // 1+3 = Ctrl
    COMBO_ALT,      // A+1 = Alt
    COMBO_GUI,      // D+4 = GUI
    COMBO_DEL,      // 3+4 = Delete
    COMBO_RESET     // A+D+1+4 = Reset keyboard
};

// Combo key arrays
const uint16_t PROGMEM combo_esc[] = {KC_A, KC_B, COMBO_END};
const uint16_t PROGMEM combo_enter[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {KC_A, KC_C, COMBO_END};
const uint16_t PROGMEM combo_bspc[] = {KC_B, KC_D, COMBO_END};
const uint16_t PROGMEM combo_space[] = {KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_shift[] = {KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_ctrl[] = {KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_alt[] = {KC_A, KC_1, COMBO_END};
const uint16_t PROGMEM combo_gui[] = {KC_D, KC_4, COMBO_END};
const uint16_t PROGMEM combo_del[] = {KC_3, KC_4, COMBO_END};
const uint16_t PROGMEM combo_reset[] = {KC_A, KC_D, KC_1, KC_4, COMBO_END};

// Combo assignment
combo_t key_combos[] = {
    [COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_ENTER] = COMBO(combo_enter, KC_ENT),
    [COMBO_TAB] = COMBO(combo_tab, KC_TAB),
    [COMBO_BSPC] = COMBO(combo_bspc, KC_BSPC),
    [COMBO_SPACE] = COMBO(combo_space, KC_SPC),
    [COMBO_SHIFT] = COMBO(combo_shift, KC_LSFT),
    [COMBO_CTRL] = COMBO(combo_ctrl, KC_LCTL),
    [COMBO_ALT] = COMBO(combo_alt, KC_LALT),
    [COMBO_GUI] = COMBO(combo_gui, KC_LGUI),
    [COMBO_DEL] = COMBO(combo_del, KC_DEL),
    [COMBO_RESET] = COMBO(combo_reset, QK_BOOT)
};
#endif

#ifdef JOYSTICK_ENABLE
// Joystick configuration using QMK's built-in framework
// Note: Axis configuration is handled in keyboard.json for modern QMK

// Layer-based joystick behavior
void matrix_scan_user(void) {
    static uint8_t last_layer = 255;
    uint8_t current_layer = get_highest_layer(layer_state);
    
    if (current_layer != last_layer) {
        switch (current_layer) {
            case _BASE:
                // Analog mode - QMK handles this automatically
                break;
            case _DIGITAL:
                // Digital mode - convert analog to buttons in process_record_user
                break;
            case _GAMING:
                // Gaming mode - hybrid analog/digital optimized for FPS
                break;
            case _CONFIG:
                // Config mode - joysticks for menu navigation
                break;
        }
        last_layer = current_layer;
    }
}

// Custom joystick processing for digital layers
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t last_joystick_read = 0;
    
    // Read joystick axes periodically when in digital mode
    if (get_highest_layer(layer_state) == _DIGITAL && 
        timer_elapsed(last_joystick_read) > 50) { // 20Hz update rate
        
        // Read axis values (QMK automatically handles ADC conversion)
        int16_t js1_x = joystick_read_axis(0);  // GP26
        int16_t js1_y = joystick_read_axis(1);  // GP27  
        int16_t js2_x = joystick_read_axis(2);  // GP28
        int16_t js2_y = joystick_read_axis(3);  // GP29
        
        // Convert analog to digital with deadzone
        #define DEADZONE 100
        #define THRESHOLD 200
        
        // Joystick 1 - WASD movement
        static bool js1_states[4] = {false}; // up, down, left, right
        bool new_js1_states[4] = {
            js1_y < -THRESHOLD,  // up
            js1_y > THRESHOLD,   // down
            js1_x < -THRESHOLD,  // left
            js1_x > THRESHOLD    // right
        };
        
        // Send key events for joystick 1
        uint16_t js1_keys[4] = {KC_W, KC_S, KC_A, KC_D};
        for (int i = 0; i < 4; i++) {
            if (new_js1_states[i] != js1_states[i]) {
                if (new_js1_states[i]) {
                    register_code(js1_keys[i]);
                } else {
                    unregister_code(js1_keys[i]);
                }
                js1_states[i] = new_js1_states[i];
            }
        }
        
        // Joystick 2 - Arrow keys
        static bool js2_states[4] = {false}; // up, down, left, right
        bool new_js2_states[4] = {
            js2_y < -THRESHOLD,  // up
            js2_y > THRESHOLD,   // down
            js2_x < -THRESHOLD,  // left
            js2_x > THRESHOLD    // right
        };
        
        // Send key events for joystick 2
        uint16_t js2_keys[4] = {KC_UP, KC_DOWN, KC_LEFT, KC_RGHT};
        for (int i = 0; i < 4; i++) {
            if (new_js2_states[i] != js2_states[i]) {
                if (new_js2_states[i]) {
                    register_code(js2_keys[i]);
                } else {
                    unregister_code(js2_keys[i]);
                }
                js2_states[i] = new_js2_states[i];
            }
        }
        
        last_joystick_read = timer_read();
    }
    
    // Handle custom keycodes
    switch (keycode) {
        case KC_PROFILE:
            if (record->event.pressed) {
                // Cycle through layers
                uint8_t current = get_highest_layer(layer_state);
                uint8_t next = (current + 1) % 4;
                layer_clear();
                layer_on(next);
            }
            return false;
            
        case KC_CONFIG:
            if (record->event.pressed) {
                layer_clear();
                layer_on(_CONFIG);
            }
            return false;
            
        case KC_RESET_PROFILE:
            if (record->event.pressed) {
                layer_clear();
                layer_on(_BASE);
            }
            return false;
    }
    
    return true;
}
#endif

// RGB or LED indicators for current layer (if available)
layer_state_t layer_state_set_user(layer_state_t state) {
    // Future implementation - indicate current layer via LEDs or display
    return state;
}