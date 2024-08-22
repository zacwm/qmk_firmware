#include QMK_KEYBOARD_H

enum preonic_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FKEYS,
};

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  BACKLIT
};

#define FKEYS MO(_FKEYS)
#define LSFTF9 LSFT(KC_F9)
#define LSFTF10 LSFT(KC_F10)
#define CTRLF2 LCTL(KC_F2)
