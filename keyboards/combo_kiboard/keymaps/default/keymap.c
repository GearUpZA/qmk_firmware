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

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A, KC_B, KC_C, KC_D,
        KC_1, KC_2, KC_3, KC_4
    ),
    [1] = LAYOUT(
        KC_E, KC_F, KC_G, KC_H,
        KC_5, KC_6, KC_7, KC_8
    ),
    [2] = LAYOUT(
        KC_I, KC_J, KC_K, KC_L,
        KC_9, KC_0, KC_MINS, KC_EQL
    ),
    [3] = LAYOUT(
        KC_M, KC_N, KC_O, KC_P,
        KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV
    ),
};

// QMK Combo definitions using proper QMK combo system
enum combo_events {
    COMBO_PROFILE_1,
    COMBO_PROFILE_2,
    COMBO_PROFILE_3,
    COMBO_PROFILE_4,
    COMBO_JOY1_TOGGLE,
    COMBO_JOY2_TOGGLE,
    COMBO_CALIBRATE,
    COMBO_RESET_SETTINGS,
    COMBO_LAYER_1,
    COMBO_LAYER_2,
    COMBO_LAYER_3,
    COMBO_EMERGENCY_RESET,
    COMBO_TOUCH_TOGGLE,
    COMBO_DEBUG_MODE,
    COMBO_FACTORY_RESET,
    COMBO_LENGTH
};

// Combo key sequences
const uint16_t PROGMEM combo_profile_1[] = {KC_A, KC_1, COMBO_END};
const uint16_t PROGMEM combo_profile_2[] = {KC_B, KC_2, COMBO_END};
const uint16_t PROGMEM combo_profile_3[] = {KC_C, KC_3, COMBO_END};
const uint16_t PROGMEM combo_profile_4[] = {KC_D, KC_4, COMBO_END};
const uint16_t PROGMEM combo_joy1_toggle[] = {KC_A, KC_B, COMBO_END};
const uint16_t PROGMEM combo_joy2_toggle[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_calibrate[] = {KC_1, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_reset_settings[] = {KC_A, KC_B, KC_C, COMBO_END};
const uint16_t PROGMEM combo_layer_1[] = {KC_A, KC_2, COMBO_END};
const uint16_t PROGMEM combo_layer_2[] = {KC_B, KC_3, COMBO_END};
const uint16_t PROGMEM combo_layer_3[] = {KC_C, KC_4, COMBO_END};
const uint16_t PROGMEM combo_emergency_reset[] = {KC_A, KC_D, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_touch_toggle[] = {KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_debug_mode[] = {KC_B, KC_C, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_factory_reset[] = {KC_A, KC_B, KC_C, KC_D, COMBO_END};

// QMK combo array
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

// QMK combo callback for custom actions
void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) return;

    switch (combo_index) {
        case COMBO_PROFILE_1:
            profile_manager_switch_profile(0);
            break;
        case COMBO_PROFILE_2:
            profile_manager_switch_profile(1);
            break;
        case COMBO_PROFILE_3:
            profile_manager_switch_profile(2);
            break;
        case COMBO_PROFILE_4:
            profile_manager_switch_profile(3);
            break;
        case COMBO_JOY1_TOGGLE:
            // Toggle joystick 1 mode
            {
                extern kiboard_state_t kiboard_state;
                kiboard_state.joystick1.analog_mode = !kiboard_state.joystick1.analog_mode;
                profile_manager_mark_dirty();
            }
            break;
        case COMBO_JOY2_TOGGLE:
            // Toggle joystick 2 mode
            {
                extern kiboard_state_t kiboard_state;
                kiboard_state.joystick2.analog_mode = !kiboard_state.joystick2.analog_mode;
                profile_manager_mark_dirty();
            }
            break;
        case COMBO_CALIBRATE:
            touch_interface_calibrate();
            break;
        case COMBO_RESET_SETTINGS:
            profile_manager_reset_current();
            break;
        case COMBO_EMERGENCY_RESET:
            profile_manager_emergency_reset();
            break;
        case COMBO_TOUCH_TOGGLE:
            {
                extern kiboard_state_t kiboard_state;
                kiboard_state.touchscreen_enabled = !kiboard_state.touchscreen_enabled;
                profile_manager_mark_dirty();
            }
            break;
        case COMBO_DEBUG_MODE:
            {
                extern kiboard_state_t kiboard_state;
                kiboard_state.debug_mode = !kiboard_state.debug_mode;
            }
            break;
        case COMBO_FACTORY_RESET:
            profile_manager_factory_reset();
            break;
    }
}
