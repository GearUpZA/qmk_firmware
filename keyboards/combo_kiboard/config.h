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

#pragma once

// Matrix configuration
#define MATRIX_ROWS 2
#define MATRIX_COLS 4

// Analog pins for joysticks
#define ANALOG_JOYSTICK_X_AXIS_PIN GP26
#define ANALOG_JOYSTICK_Y_AXIS_PIN GP27
#define ANALOG_JOYSTICK2_X_AXIS_PIN GP28
#define ANALOG_JOYSTICK2_Y_AXIS_PIN GP29

// Digital pins for joystick switches
#define JOYSTICK1_DIGITAL_X_PIN GP8
#define JOYSTICK1_DIGITAL_Y_PIN GP9
#define JOYSTICK2_DIGITAL_X_PIN GP10
#define JOYSTICK2_DIGITAL_Y_PIN GP11

// Mode switch pins
#define JOYSTICK1_MODE_PIN GP12
#define JOYSTICK2_MODE_PIN GP13

// Touchscreen SPI pins
#define TOUCHSCREEN_SPI_INSTANCE spi1
#define TOUCHSCREEN_CS_PIN GP14
#define TOUCHSCREEN_SCK_PIN GP15
#define TOUCHSCREEN_MOSI_PIN GP16
#define TOUCHSCREEN_MISO_PIN GP17
#define TOUCHSCREEN_INT_PIN GP18
#define TOUCHSCREEN_RST_PIN GP19

// Display pins (parallel interface)
#define DISPLAY_DC_PIN GP20
#define DISPLAY_CS_PIN GP21
#define DISPLAY_RST_PIN GP22

// Joystick configuration (using QMK's joystick feature)
#define JOYSTICK_AXIS_COUNT 4
#define JOYSTICK_AXIS_RESOLUTION 10

// Pointing device configuration (for mouse control)
#define POINTING_DEVICE_TASK_THROTTLE_MS 10

// Combo configuration (using QMK's combo system)
#define COMBO_COUNT 15
#define COMBO_TERM 200
#define COMBO_MUST_TAP_PER_COMBO
#define COMBO_MUST_HOLD_PER_COMBO
#define COMBO_TERM_PER_COMBO

// EEPROM configuration (using QMK's eeconfig)
#define EECONFIG_USER_DATA_SIZE 32

// ADC configuration for analog joysticks
#define ADC_RESOLUTION ADC_CFGR1_RES_12BIT
#define ANALOG_JOYSTICK_DEADZONE 50
#define ANALOG_JOYSTICK_SENSITIVITY 8

// Touchscreen configuration
#define TOUCHSCREEN_WIDTH 320
#define TOUCHSCREEN_HEIGHT 240
#define TOUCHSCREEN_CALIBRATION_POINTS 5

// Profile system configuration
#define MAX_PROFILES 4
#define PROFILE_NAME_LENGTH 16

// Debounce settings
#define DEBOUNCE 5
#define JOYSTICK_DEBOUNCE 10

// Power management
#define USB_SUSPEND_WAKEUP_DELAY 0

// Disable unused features to save space
#define NO_MUSIC_MODE
#define NO_AUDIO
#define DISABLE_LEADER

// Custom keycodes start
enum custom_keycodes {
    CK_PROF1 = SAFE_RANGE,
    CK_PROF2,
    CK_PROF3,
    CK_PROF4,
    CK_JOY1_MODE,
    CK_JOY2_MODE,
    CK_TOUCH_CAL,
    CK_COMBO_TOG,
    CK_SAFE_RANGE
};

// Custom combo definitions
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
    COMBO_FACTORY_RESET
};
