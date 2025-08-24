#include "analog.h"
#include "quantum.h"

void analog_init(void) {
    // Initialize ADC
    palSetLineMode(ANALOG_JOYSTICK_1_X_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(ANALOG_JOYSTICK_1_Y_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(ANALOG_JOYSTICK_2_X_PIN, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(ANALOG_JOYSTICK_2_Y_PIN, PAL_MODE_INPUT_ANALOG);
}

int16_t analog_read_joy(uint8_t pin) {
    int16_t val = analogReadPin(pin);

    // Apply deadzone
    if (abs(val - (ANALOG_JOYSTICK_AXIS_MAX / 2)) < ANALOG_JOYSTICK_DEADZONE) {
        return 0;
    }

    return val;
}

void matrix_scan_kb(void) {
    // Read joystick values
    int16_t lx = analog_read_joy(ANALOG_JOYSTICK_1_X_PIN);
    int16_t ly = analog_read_joy(ANALOG_JOYSTICK_1_Y_PIN);
    int16_t rx = analog_read_joy(ANALOG_JOYSTICK_2_X_PIN);
    int16_t ry = analog_read_joy(ANALOG_JOYSTICK_2_Y_PIN);

    // Process joystick values here
    // You can send these values over USB as mouse movement or custom reports

    matrix_scan_user();
}
