#pragma once
#include "quantum.h"

/* Key matrix configuration
 * Left hand:  A B C D + Joystick (LX, LY)
 * Right hand: 1 2 3 4 + Joystick (RX, RY)
 */

#define LAYOUT( \
    A,  B,  C,  D,  N1, N2, N3, N4, \
    LX, LY, RX, RY                   \
) { \
    { KC_A, KC_B, KC_C, KC_D, KC_1, KC_2, KC_3, KC_4 }, \
    { LX,   LY,   RX,   RY,   KC_NO, KC_NO, KC_NO, KC_NO } \
}
