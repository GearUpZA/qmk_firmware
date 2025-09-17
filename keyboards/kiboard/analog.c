#include "kiboard.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// QMK function declarations
uint16_t analogReadPin(uint8_t pin);
void register_code(uint8_t keycode);
void unregister_code(uint8_t keycode);
void tap_code(uint8_t keycode);
uint16_t timer_read(void);
bool timer_elapsed(uint16_t last, uint16_t timeout);
void register_code16(uint16_t keycode);
void unregister_code16(uint16_t keycode);

// Weak function declarations for QMK callbacks
__attribute__((weak)) void matrix_scan_user(void) {}

// Math function declarations
int abs(int x);

// Mouse/pointing device declarations
typedef struct {
    int8_t x;
    int8_t y;
    uint8_t buttons;
} report_mouse_t;

report_mouse_t pointing_device_get_report(void);
void pointing_device_set_report(report_mouse_t mouse_report);
void pointing_device_send(void);

// Joystick modes
typedef enum {
    JOY_MODE_ANALOG,    // Analog output for camera/mouse control
    JOY_MODE_DIGITAL    // Digital hat switch for button presses
} joystick_mode_t;

// Joystick state tracking
typedef struct {
    bool up, down, left, right;
    bool prev_up, prev_down, prev_left, prev_right;
    uint16_t debounce_timer;
} joystick_digital_state_t;

// Configuration
#define JOYSTICK_THRESHOLD_HIGH 700   // Above this = direction active
#define JOYSTICK_THRESHOLD_LOW 300    // Below this = direction active
#define JOYSTICK_CENTER 512           // Center position
#define JOYSTICK_DEBOUNCE_MS 50       // Debounce time for digital mode
#define ANALOG_JOYSTICK_DEADZONE 50   // Deadzone for analog mode
#define ANALOG_JOYSTICK_AXIS_MAX 1023 // Max ADC value

// Pin definitions from config.h
#define GP26 26
#define GP27 27
#define GP28 28
#define GP29 29
#define JOYSTICK_1_X_PIN GP26
#define JOYSTICK_1_Y_PIN GP27
#define JOYSTICK_2_X_PIN GP28
#define JOYSTICK_2_Y_PIN GP29

// QMK keycode definitions
#define KC_F13 0x68
#define KC_F17 0x6C

// Global state
static joystick_mode_t joy1_mode = JOY_MODE_DIGITAL;
static joystick_mode_t joy2_mode = JOY_MODE_DIGITAL;
static joystick_digital_state_t joy1_digital = {0};
static joystick_digital_state_t joy2_digital = {0};

void analog_init(void) {
    // Initialize ADC pins for analog reading
    // RP2040 handles this automatically when analogReadPin is called
}

int apply_deadzone(int val) {
    // Apply deadzone for analog mode
    if (abs(val - (ANALOG_JOYSTICK_AXIS_MAX / 2)) < ANALOG_JOYSTICK_DEADZONE) {
        return JOYSTICK_CENTER;
    }
    return val;
}

int16_t analog_read_joy(uint8_t pin) {
    int16_t val = analogReadPin(pin);

    // Apply deadzone for analog mode
    if (abs(val - (ANALOG_JOYSTICK_AXIS_MAX / 2)) < ANALOG_JOYSTICK_DEADZONE) {
        return JOYSTICK_CENTER;
    }

    return val;
}

// Convert analog values to digital directions
void update_joystick_digital(int16_t x, int16_t y, joystick_digital_state_t* state) {
    // Store previous state
    state->prev_up = state->up;
    state->prev_down = state->down;
    state->prev_left = state->left;
    state->prev_right = state->right;

    // Update current state based on thresholds
    state->up = (y > JOYSTICK_THRESHOLD_HIGH);
    state->down = (y < JOYSTICK_THRESHOLD_LOW);
    state->left = (x < JOYSTICK_THRESHOLD_LOW);
    state->right = (x > JOYSTICK_THRESHOLD_HIGH);
}

// Send analog joystick values as mouse movement
void send_joystick_analog(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    // Convert to mouse movement range (-127 to 127)
    int8_t mouse_x = (int8_t)((x1 - JOYSTICK_CENTER) / 4);
    int8_t mouse_y = (int8_t)((y1 - JOYSTICK_CENTER) / 4);

    // Send as mouse movement (you can modify this for your specific analog needs)
    if (mouse_x != 0 || mouse_y != 0) {
        report_mouse_t mouse_report = pointing_device_get_report();
        mouse_report.x = mouse_x;
        mouse_report.y = mouse_y;
        pointing_device_set_report(mouse_report);
        pointing_device_send();
    }
}

// Check if digital state has changed (for debouncing)
bool joystick_digital_changed(joystick_digital_state_t* state) {
    return (state->up != state->prev_up) ||
           (state->down != state->prev_down) ||
           (state->left != state->prev_left) ||
           (state->right != state->prev_right);
}

// Send digital joystick presses as key events
void send_joystick_digital_events(joystick_digital_state_t* state, uint16_t base_keycode) {
    if (state->up && !state->prev_up) {
        register_code16(base_keycode); // UP
    } else if (!state->up && state->prev_up) {
        unregister_code16(base_keycode);
    }

    if (state->down && !state->prev_down) {
        register_code16(base_keycode + 1); // DOWN
    } else if (!state->down && state->prev_down) {
        unregister_code16(base_keycode + 1);
    }

    if (state->left && !state->prev_left) {
        register_code16(base_keycode + 2); // LEFT
    } else if (!state->left && state->prev_left) {
        unregister_code16(base_keycode + 2);
    }

    if (state->right && !state->prev_right) {
        register_code16(base_keycode + 3); // RIGHT
    } else if (!state->right && state->prev_right) {
        unregister_code16(base_keycode + 3);
    }
}

// Profile management functions
void set_joystick_mode(uint8_t joystick, joystick_mode_t mode) {
    if (joystick == 1) {
        joy1_mode = mode;
    } else if (joystick == 2) {
        joy2_mode = mode;
    }
}

joystick_mode_t get_joystick_mode(uint8_t joystick) {
    return (joystick == 1) ? joy1_mode : joy2_mode;
}

void cycle_joystick_profiles(void) {
    // Cycle through different profile combinations
    if (joy1_mode == JOY_MODE_DIGITAL && joy2_mode == JOY_MODE_DIGITAL) {
        // Profile 1: Joy1=Analog, Joy2=Digital
        joy1_mode = JOY_MODE_ANALOG;
        joy2_mode = JOY_MODE_DIGITAL;
    } else if (joy1_mode == JOY_MODE_ANALOG && joy2_mode == JOY_MODE_DIGITAL) {
        // Profile 2: Joy1=Digital, Joy2=Analog
        joy1_mode = JOY_MODE_DIGITAL;
        joy2_mode = JOY_MODE_ANALOG;
    } else if (joy1_mode == JOY_MODE_DIGITAL && joy2_mode == JOY_MODE_ANALOG) {
        // Profile 3: Joy1=Analog, Joy2=Analog
        joy1_mode = JOY_MODE_ANALOG;
        joy2_mode = JOY_MODE_ANALOG;
    } else {
        // Profile 0: Joy1=Digital, Joy2=Digital (default)
        joy1_mode = JOY_MODE_DIGITAL;
        joy2_mode = JOY_MODE_DIGITAL;
    }
}

void matrix_scan_kb(void) {
    // Read joystick values
    int16_t joy1_x = analog_read_joy(JOYSTICK_1_X_PIN);
    int16_t joy1_y = analog_read_joy(JOYSTICK_1_Y_PIN);
    int16_t joy2_x = analog_read_joy(JOYSTICK_2_X_PIN);
    int16_t joy2_y = analog_read_joy(JOYSTICK_2_Y_PIN);

    // Process Joystick 1
    if (joy1_mode == JOY_MODE_DIGITAL) {
        update_joystick_digital(joy1_x, joy1_y, &joy1_digital);

        // Send digital events if state changed and debounce time passed
        if (joystick_digital_changed(&joy1_digital) &&
            timer_elapsed(joy1_digital.debounce_timer, JOYSTICK_DEBOUNCE_MS)) {
            // Base keycode for Joy1: you'll need to define these in your keymap
            // JOY1_UP, JOY1_DOWN, JOY1_LEFT, JOY1_RIGHT
            send_joystick_digital_events(&joy1_digital, KC_F13); // Example base keycode
            joy1_digital.debounce_timer = timer_read();
        }
    }

    // Process Joystick 2
    if (joy2_mode == JOY_MODE_DIGITAL) {
        update_joystick_digital(joy2_x, joy2_y, &joy2_digital);

        if (joystick_digital_changed(&joy2_digital) &&
            timer_elapsed(joy2_digital.debounce_timer, JOYSTICK_DEBOUNCE_MS)) {
            // Base keycode for Joy2: JOY2_UP, JOY2_DOWN, JOY2_LEFT, JOY2_RIGHT
            send_joystick_digital_events(&joy2_digital, KC_F17); // Example base keycode
            joy2_digital.debounce_timer = timer_read();
        }
    }

    // Handle analog modes
    if (joy1_mode == JOY_MODE_ANALOG || joy2_mode == JOY_MODE_ANALOG) {
        int16_t analog_x1 = (joy1_mode == JOY_MODE_ANALOG) ? joy1_x : JOYSTICK_CENTER;
        int16_t analog_y1 = (joy1_mode == JOY_MODE_ANALOG) ? joy1_y : JOYSTICK_CENTER;
        int16_t analog_x2 = (joy2_mode == JOY_MODE_ANALOG) ? joy2_x : JOYSTICK_CENTER;
        int16_t analog_y2 = (joy2_mode == JOY_MODE_ANALOG) ? joy2_y : JOYSTICK_CENTER;

        send_joystick_analog(analog_x1, analog_y1, analog_x2, analog_y2);
    }

    matrix_scan_user();
}
