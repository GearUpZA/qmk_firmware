#pragma once

#define MATRIX_ROWS 2
#define MATRIX_COLS 8

// Split keyboard config
#define SPLIT_HAND_PIN GP2
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1

// Joystick config
#define ANALOG_JOYSTICK_COUNT 2
#define ANALOG_JOYSTICK_AXIS_COUNT 2

// Left joystick pins
#define ANALOG_JOYSTICK_1_X_PIN GP26
#define ANALOG_JOYSTICK_1_Y_PIN GP27

// Right joystick pins
#define ANALOG_JOYSTICK_2_X_PIN GP28
#define ANALOG_JOYSTICK_2_Y_PIN GP29

// Joystick parameters
#define ANALOG_JOYSTICK_AXIS_MIN 0
#define ANALOG_JOYSTICK_AXIS_MAX 1023
#define ANALOG_JOYSTICK_SPEED_MAX 127
#define ANALOG_JOYSTICK_DEADZONE 100

// Debounce reduces chatter
#define DEBOUNCE 5

// Missing pin definitions
#define LCD_DC_PIN GP16
#define LCD_CS_PIN GP17
#define LCD_RST_PIN GP18
#define LCD_BL_PIN GP19
#define TOUCH_RST_PIN GP20
