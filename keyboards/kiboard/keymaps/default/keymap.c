#include QMK_KEYBOARD_H
#include "g/keymap_combo.h"
#include "analog.h"

// Custom keycodes
enum custom_keycodes {
    JOY_PROFILE = SAFE_RANGE,
    JOY_NEW_SAFE_RANGE
};

// Single Left + Single Right combinations
const uint16_t PROGMEM combo_A1[] = {KC_A, KC_1, COMBO_END};
const uint16_t PROGMEM combo_A2[] = {KC_A, KC_2, COMBO_END};
const uint16_t PROGMEM combo_A3[] = {KC_A, KC_3, COMBO_END};
const uint16_t PROGMEM combo_A4[] = {KC_A, KC_4, COMBO_END};

const uint16_t PROGMEM combo_B1[] = {KC_B, KC_1, COMBO_END};
const uint16_t PROGMEM combo_B2[] = {KC_B, KC_2, COMBO_END};
const uint16_t PROGMEM combo_B3[] = {KC_B, KC_3, COMBO_END};
const uint16_t PROGMEM combo_B4[] = {KC_B, KC_4, COMBO_END};

const uint16_t PROGMEM combo_C1[] = {KC_C, KC_1, COMBO_END};
const uint16_t PROGMEM combo_C2[] = {KC_C, KC_2, COMBO_END};
const uint16_t PROGMEM combo_C3[] = {KC_C, KC_3, COMBO_END};
const uint16_t PROGMEM combo_C4[] = {KC_C, KC_4, COMBO_END};

const uint16_t PROGMEM combo_D1[] = {KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_D2[] = {KC_D, KC_2, COMBO_END};
const uint16_t PROGMEM combo_D3[] = {KC_D, KC_3, COMBO_END};
const uint16_t PROGMEM combo_D4[] = {KC_D, KC_4, COMBO_END};

// Two Left + Single Right combinations
const uint16_t PROGMEM combo_AB1[] = {KC_A, KC_B, KC_1, COMBO_END};
const uint16_t PROGMEM combo_AB2[] = {KC_A, KC_B, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AB3[] = {KC_A, KC_B, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AB4[] = {KC_A, KC_B, KC_4, COMBO_END};

const uint16_t PROGMEM combo_AC1[] = {KC_A, KC_C, KC_1, COMBO_END};
const uint16_t PROGMEM combo_AC2[] = {KC_A, KC_C, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AC3[] = {KC_A, KC_C, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AC4[] = {KC_A, KC_C, KC_4, COMBO_END};

const uint16_t PROGMEM combo_AD1[] = {KC_A, KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_AD2[] = {KC_A, KC_D, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AD3[] = {KC_A, KC_D, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AD4[] = {KC_A, KC_D, KC_4, COMBO_END};

const uint16_t PROGMEM combo_BC1[] = {KC_B, KC_C, KC_1, COMBO_END};
const uint16_t PROGMEM combo_BC2[] = {KC_B, KC_C, KC_2, COMBO_END};
const uint16_t PROGMEM combo_BC3[] = {KC_B, KC_C, KC_3, COMBO_END};
const uint16_t PROGMEM combo_BC4[] = {KC_B, KC_C, KC_4, COMBO_END};

const uint16_t PROGMEM combo_BD1[] = {KC_B, KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_BD2[] = {KC_B, KC_D, KC_2, COMBO_END};
const uint16_t PROGMEM combo_BD3[] = {KC_B, KC_D, KC_3, COMBO_END};
const uint16_t PROGMEM combo_BD4[] = {KC_B, KC_D, KC_4, COMBO_END};

const uint16_t PROGMEM combo_CD1[] = {KC_C, KC_D, KC_1, COMBO_END};
const uint16_t PROGMEM combo_CD2[] = {KC_C, KC_D, KC_2, COMBO_END};
const uint16_t PROGMEM combo_CD3[] = {KC_C, KC_D, KC_3, COMBO_END};
const uint16_t PROGMEM combo_CD4[] = {KC_C, KC_D, KC_4, COMBO_END};

// Single Left + Two Right combinations
const uint16_t PROGMEM combo_A12[] = {KC_A, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_A13[] = {KC_A, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_A14[] = {KC_A, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_A23[] = {KC_A, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_A24[] = {KC_A, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_A34[] = {KC_A, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_B12[] = {KC_B, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_B13[] = {KC_B, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_B14[] = {KC_B, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_B23[] = {KC_B, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_B24[] = {KC_B, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_B34[] = {KC_B, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_C12[] = {KC_C, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_C13[] = {KC_C, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_C14[] = {KC_C, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_C23[] = {KC_C, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_C24[] = {KC_C, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_C34[] = {KC_C, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_D12[] = {KC_D, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_D13[] = {KC_D, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_D14[] = {KC_D, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_D23[] = {KC_D, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_D24[] = {KC_D, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_D34[] = {KC_D, KC_3, KC_4, COMBO_END};

// Two Left + Two Right combinations
const uint16_t PROGMEM combo_AB12[] = {KC_A, KC_B, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AB13[] = {KC_A, KC_B, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AB14[] = {KC_A, KC_B, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_AB23[] = {KC_A, KC_B, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AB24[] = {KC_A, KC_B, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_AB34[] = {KC_A, KC_B, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_AC12[] = {KC_A, KC_C, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AC13[] = {KC_A, KC_C, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AC14[] = {KC_A, KC_C, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_AC23[] = {KC_A, KC_C, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AC24[] = {KC_A, KC_C, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_AC34[] = {KC_A, KC_C, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_AD12[] = {KC_A, KC_D, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_AD13[] = {KC_A, KC_D, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AD14[] = {KC_A, KC_D, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_AD23[] = {KC_A, KC_D, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_AD24[] = {KC_A, KC_D, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_AD34[] = {KC_A, KC_D, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_BC12[] = {KC_B, KC_C, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_BC13[] = {KC_B, KC_C, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_BC14[] = {KC_B, KC_C, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_BC23[] = {KC_B, KC_C, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_BC24[] = {KC_B, KC_C, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_BC34[] = {KC_B, KC_C, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_BD12[] = {KC_B, KC_D, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_BD13[] = {KC_B, KC_D, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_BD14[] = {KC_B, KC_D, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_BD23[] = {KC_B, KC_D, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_BD24[] = {KC_B, KC_D, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_BD34[] = {KC_B, KC_D, KC_3, KC_4, COMBO_END};

const uint16_t PROGMEM combo_CD12[] = {KC_C, KC_D, KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_CD13[] = {KC_C, KC_D, KC_1, KC_3, COMBO_END};
const uint16_t PROGMEM combo_CD14[] = {KC_C, KC_D, KC_1, KC_4, COMBO_END};
const uint16_t PROGMEM combo_CD23[] = {KC_C, KC_D, KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_CD24[] = {KC_C, KC_D, KC_2, KC_4, COMBO_END};
const uint16_t PROGMEM combo_CD34[] = {KC_C, KC_D, KC_3, KC_4, COMBO_END};

combo_t key_combos[] = {
    // Placeholders for combo actions - to be defined later
    COMBO(combo_A1, XXXXXXX),
    COMBO(combo_A2, XXXXXXX),
    // ... continue for all combos
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A,    KC_B,    KC_C,    KC_D,    KC_1,    KC_2,    KC_3,    KC_4
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle custom keycodes
    switch (keycode) {
        case JOY_PROFILE:
            if (record->event.pressed) {
                cycle_joystick_profiles();
            }
            return false;
    }

    // Special combo to enter config mode: Hold A+B+C+D for 3 seconds
    static uint16_t config_timer;
    if (layer_state == 0 &&
        IS_PRESSED(KC_A) && IS_PRESSED(KC_B) &&
        IS_PRESSED(KC_C) && IS_PRESSED(KC_D)) {
        if (record->event.pressed) {
            config_timer = timer_read();
        } else {
            if (timer_elapsed(config_timer) > 3000) {
                enter_config_mode();
                reset_keyboard(); // This will restart the keyboard
            }
        }
        return false;
    }
    return true;
}
