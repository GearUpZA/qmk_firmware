#pragma once

#define MATRIX_ROWS 2
#define MATRIX_COLS 8

// Split keyboard config
#define SPLIT_HAND_PIN GP2
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1

// Joystick config
#define ANALOG_JOYSTICK_X_AXIS_PIN GP26
#define ANALOG_JOYSTICK_Y_AXIS_PIN GP27
#define ANALOG_JOYSTICK_X_AXIS_PIN_RIGHT GP28
#define ANALOG_JOYSTICK_Y_AXIS_PIN_RIGHT GP29

// Debounce reduces chatter
#define DEBOUNCE 5