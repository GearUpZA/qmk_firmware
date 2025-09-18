/* Copyright 2025 Custom Keyboards
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

#include "combo_handler.h"
#include "profile_manager.h"

// Convert matrix positions to keycodes for combo definitions
#define KC_K00 KC_A  // Row 0, Col 0 - First button
#define KC_K01 KC_B  // Row 0, Col 1 - Second button
#define KC_K02 KC_C  // Row 0, Col 2 - Third button
#define KC_K03 KC_D  // Row 0, Col 3 - Fourth button
#define KC_K10 KC_1  // Row 1, Col 0 - Fifth button
#define KC_K11 KC_2  // Row 1, Col 1 - Sixth button
#define KC_K12 KC_3  // Row 1, Col 2 - Seventh button
#define KC_K13 KC_4  // Row 1, Col 3 - Eighth button

// Define combo key sequences using QMK's PROGMEM format
const uint16_t PROGMEM combo_profile_1[] = {KC_K00, KC_K10, COMBO_END};       // A + 1
const uint16_t PROGMEM combo_profile_2[] = {KC_K01, KC_K11, COMBO_END};       // B + 2
const uint16_t PROGMEM combo_profile_3[] = {KC_K02, KC_K12, COMBO_END};       // C + 3
const uint16_t PROGMEM combo_profile_4[] = {KC_K03, KC_K13, COMBO_END};       // D + 4

const uint16_t PROGMEM combo_joy1_toggle[] = {KC_K00, KC_K01, COMBO_END};     // A + B
const uint16_t PROGMEM combo_joy2_toggle[] = {KC_K02, KC_K03, COMBO_END};     // C + D

const uint16_t PROGMEM combo_calibrate[] = {KC_K10, KC_K11, KC_K12, COMBO_END}; // 1 + 2 + 3
const uint16_t PROGMEM combo_reset_settings[] = {KC_K00, KC_K01, KC_K02, COMBO_END}; // A + B + C

const uint16_t PROGMEM combo_layer_1[] = {KC_K00, KC_K11, COMBO_END};         // A + 2
const uint16_t PROGMEM combo_layer_2[] = {KC_K01, KC_K12, COMBO_END};         // B + 3
const uint16_t PROGMEM combo_layer_3[] = {KC_K02, KC_K13, COMBO_END};         // C + 4

const uint16_t PROGMEM combo_emergency_reset[] = {KC_K00, KC_K03, KC_K10, KC_K13, COMBO_END}; // A + D + 1 + 4
const uint16_t PROGMEM combo_touch_toggle[] = {KC_K10, KC_K12, COMBO_END};    // 1 + 3
const uint16_t PROGMEM combo_debug_mode[] = {KC_K01, KC_K02, KC_K11, KC_K12, COMBO_END}; // B + C + 2 + 3
const uint16_t PROGMEM combo_factory_reset[] = {KC_K00, KC_K01, KC_K02, KC_K03, COMBO_END}; // A + B + C + D

// QMK combo array - this is required by QMK's combo system
combo_t key_combos[] = {
    [COMBO_PROFILE_1]      = COMBO_ACTION(combo_profile_1),
    [COMBO_PROFILE_2]      = COMBO_ACTION(combo_profile_2),
    [COMBO_PROFILE_3]      = COMBO_ACTION(combo_profile_3),
    [COMBO_PROFILE_4]      = COMBO_ACTION(combo_profile_4),
    [COMBO_JOY1_TOGGLE]    = COMBO_ACTION(combo_joy1_toggle),
    [COMBO_JOY2_TOGGLE]    = COMBO_ACTION(combo_joy2_toggle),
    [COMBO_CALIBRATE]      = COMBO_ACTION(combo_calibrate),
    [COMBO_RESET_SETTINGS] = COMBO_ACTION(combo_reset_settings),
    [COMBO_LAYER_1]        = COMBO(combo_layer_1, TO(1)),
    [COMBO_LAYER_2]        = COMBO(combo_layer_2, TO(2)),
    [COMBO_LAYER_3]        = COMBO(combo_layer_3, TO(3)),
    [COMBO_EMERGENCY_RESET] = COMBO_ACTION(combo_emergency_reset),
    [COMBO_TOUCH_TOGGLE]   = COMBO_ACTION(combo_touch_toggle),
    [COMBO_DEBUG_MODE]     = COMBO_ACTION(combo_debug_mode),
    [COMBO_FACTORY_RESET]  = COMBO_ACTION(combo_factory_reset),
};

// Global state
static bool combo_handler_enabled = true;

void combo_handler_init(void) {
    combo_handler_enabled = true;
    // QMK combo system is initialized automatically
}

void combo_handler_reset(void) {
    // Reset any internal state if needed
    combo_handler_enabled = true;
}

// QMK callback: Called when a combo is pressed/released
void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!combo_handler_enabled || !pressed) {
        return; // Only process press events when enabled
    }

    switch (combo_index) {
        case COMBO_PROFILE_1:
            combo_switch_profile(0);
            break;
            
        case COMBO_PROFILE_2:
            combo_switch_profile(1);
            break;
            
        case COMBO_PROFILE_3:
            combo_switch_profile(2);
            break;
            
        case COMBO_PROFILE_4:
            combo_switch_profile(3);
            break;
            
        case COMBO_JOY1_TOGGLE:
            combo_toggle_joystick_mode(1);
            break;
            
        case COMBO_JOY2_TOGGLE:
            combo_toggle_joystick_mode(2);
            break;
            
        case COMBO_CALIBRATE:
            combo_start_calibration();
            break;
            
        case COMBO_RESET_SETTINGS:
            // Reset current profile settings
            profile_manager_reset_current();
            break;
            
        case COMBO_EMERGENCY_RESET:
            combo_emergency_reset();
            break;
            
        case COMBO_TOUCH_TOGGLE:
            // Toggle touchscreen functionality
            extern kiboard_state_t kiboard_state;
            kiboard_state.touchscreen_enabled = !kiboard_state.touchscreen_enabled;
            break;
            
        case COMBO_DEBUG_MODE:
            combo_toggle_debug_mode();
            break;
            
        case COMBO_FACTORY_RESET:
            combo_factory_reset();
            break;
    }
}

// QMK callback: Custom combo timing per combo
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case COMBO_EMERGENCY_RESET:
        case COMBO_FACTORY_RESET:
            return 500; // Longer timeout for dangerous actions
            
        case COMBO_CALIBRATE:
        case COMBO_DEBUG_MODE:
            return 300; // Medium timeout for configuration actions
            
        default:
            return COMBO_TERM; // Use default timing from config.h
    }
}

// QMK callback: Specify which combos must be tapped (not held)
bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case COMBO_PROFILE_1:
        case COMBO_PROFILE_2:
        case COMBO_PROFILE_3:
        case COMBO_PROFILE_4:
        case COMBO_JOY1_TOGGLE:
        case COMBO_JOY2_TOGGLE:
        case COMBO_TOUCH_TOGGLE:
            return true; // These should be quick taps
            
        default:
            return false; // Others can be held
    }
}

// QMK callback: Specify which combos must be held
bool get_combo_must_hold(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case COMBO_EMERGENCY_RESET:
        case COMBO_FACTORY_RESET:
            return true; // Safety measure - must hold these
            
        default:
            return false;
    }
}

// Custom action implementations
void combo_switch_profile(uint8_t profile_id) {
    if (profile_id < MAX_PROFILES) {
        profile_manager_switch_profile(profile_id);
        
        extern kiboard_state_t kiboard_state;
        kiboard_state.current_profile = profile_id;
        
        if (kiboard_state.debug_mode) {
            uprintf("COMBO: Switched to profile %d\n", profile_id);
        }
    }
}

void combo_toggle_joystick_mode(uint8_t joystick_id) {
    extern kiboard_state_t kiboard_state;
    
    if (joystick_id == 1) {
        kiboard_state.joystick1.analog_mode = !kiboard_state.joystick1.analog_mode;
        if (kiboard_state.debug_mode) {
            uprintf("COMBO: Joy1 mode: %s\n", 
                kiboard_state.joystick1.analog_mode ? "Analog" : "Digital");
        }
    } else if (joystick_id == 2) {
        kiboard_state.joystick2.analog_mode = !kiboard_state.joystick2.analog_mode;
        if (kiboard_state.debug_mode) {
            uprintf("COMBO: Joy2 mode: %s\n", 
                kiboard_state.joystick2.analog_mode ? "Analog" : "Digital");
        }
    }
    
    // Save the configuration
    profile_manager_save_current();
}

void combo_start_calibration(void) {
    extern kiboard_state_t kiboard_state;
    
    if (kiboard_state.debug_mode) {
        uprintf("COMBO: Starting calibration sequence\n");
    }
    
    // Start touchscreen calibration
    extern bool touchscreen_calibrate(void);
    touchscreen_calibrate();
    
    // Could also calibrate joysticks here
    extern void joystick_calibrate(uint8_t joystick_id);
    joystick_calibrate(1);
    joystick_calibrate(2);
}

void combo_emergency_reset(void) {
    extern kiboard_state_t kiboard_state;
    
    if (kiboard_state.debug_mode) {
        uprintf("COMBO: Emergency reset activated\n");
    }
    
    // Disable potentially problematic features
    kiboard_state.combo_enabled = false;
    kiboard_state.touchscreen_enabled = false;
    
    // Switch to safe profile
    kiboard_state.current_profile = 0;
    profile_manager_emergency_reset();
    
    // Re-enable combos after reset
    wait_ms(100);
    kiboard_state.combo_enabled = true;
}

void combo_factory_reset(void) {
    extern kiboard_state_t kiboard_state;
    
    if (kiboard_state.debug_mode) {
        uprintf("COMBO: Factory reset initiated\n");
    }
    
    // Reset all settings to defaults
    extern void factory_reset(void);
    factory_reset();
}

void combo_toggle_debug_mode(void) {
    extern kiboard_state_t kiboard_state;
    
    kiboard_state.debug_mode = !kiboard_state.debug_mode;
    
    if (kiboard_state.debug_mode) {
        uprintf("COMBO: Debug mode ON\n");
    } else {
        uprintf("COMBO: Debug mode OFF\n");
    }
}

void combo_switch_layer(uint8_t layer) {
    if (layer < 8) { // Reasonable layer limit
        layer_move(layer);
        
        extern kiboard_state_t kiboard_state;
        if (kiboard_state.debug_mode) {
            uprintf("COMBO: Switched to layer %d\n", layer);
        }
    }
}

// Utility functions
bool combo_handler_is_enabled(void) {
    return combo_handler_enabled;
}

void combo_handler_enable(void) {
    combo_handler_enabled = true;
    combo_enable(); // QMK function
}

void combo_handler_disable(void) {
    combo_handler_enabled = false;
    combo_disable(); // QMK function
}