/* Copyright 2015-2017 Jack Humbert
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
#include "xtonhasvim.h"

enum planck_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
    _VIM,
    _VINSERT,
};

enum planck_keycodes {
    QWERTY = VIM_SAFE_RANGE,
    BACKLIT,
    CTRL_ESC,
    SFT_ENT
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define RAISE_BS LT(RAISE, KC_QUOT)
#define RAISE_VIM LT(RAISE, KC_QUOT)

uint8_t vim_cmd_layer(void) {
    return _VIM;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT_planck_grid(
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
            CTRL_ESC,KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
            KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFT_ENT,
            KC_MEH,  KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   VIM_START,_______,KC_UP,   KC_RGHT
            ),

    [_LOWER] = LAYOUT_planck_grid(
            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
            _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______,_______, _______, _______, _______, _______
            ),

    [_RAISE] = LAYOUT_planck_grid(
            KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10, KC_BSPC,
            KC_DEL,  KC_F11,  KC_F12,  _______, _______, _______, KC_LEFT, KC_DOWN,  KC_UP,  KC_RGHT,KC_6,    KC_PLUS,
            _______, _______, _______, _______, _______, _______, _______, _______,  KC_1,   KC_2,   KC_3,    _______,
            _______, _______, _______, _______, _______, _______, _______, _______,  _______,KC_DOT, KC_EQL,  KC_MPLY
            ),

    [_ADJUST] = LAYOUT_planck_grid(
            _______, RESET,   AG_SWAP, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, _______, KC_DEL ,
            _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, KC_LEFT, KC_DOWN, KC_UP,    KC_RGHT, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, AG_NORM, _______,  _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______
            ),

    [_VIM] = LAYOUT_planck_grid(
            _______, _______, VIM_W,   VIM_E,   VIM_R, _______, VIM_Y,   VIM_U,   VIM_I,    VIM_O,   VIM_P,   _______,
            VIM_ESC, VIM_A,   VIM_S,   VIM_D,   _______, VIM_G,   VIM_H,   VIM_J,   VIM_K,    VIM_L,   _______, _______,
            VIM_SHIFT,_______,VIM_X,   VIM_C,   VIM_V,   VIM_B,   _______,_______,VIM_COMMA,VIM_PERIOD,_______,_______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______),

    [_VINSERT] = LAYOUT_planck_grid(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______)

};

static bool custom_mod_tap_consumed;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {
        case SFT_ENT:
            if (record->event.pressed) {
                register_code(KC_RSFT);
                custom_mod_tap_consumed = false;
                return true;
            }
            else
            {
                unregister_code(KC_RSFT);
                if (!custom_mod_tap_consumed)
                {
                    tap_code16(KC_ENT);
                }

                return false;
            }

        case CTRL_ESC:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                custom_mod_tap_consumed = false;
                return true;
            }
            else
            {
                unregister_code(KC_LCTL);
                if (!custom_mod_tap_consumed)
                {
                    if (IS_LAYER_ON(_VINSERT))
                        layer_move(_VIM);
                    else
                        tap_code16(KC_ESC);
                }

                return false;
            }
    }

    custom_mod_tap_consumed |= record->event.pressed;

    switch (keycode) {
        case KC_W:
            if (IS_LAYER_ON(_VINSERT) && record->event.pressed) {
                if (get_mods() & MOD_BIT(KC_LCTL) && (get_mods() & MOD_BIT(KC_LALT)) == 0)
                {
                    unregister_code(KC_LCTL);

                    register_code(KC_LALT);
                    tap_code16(KC_BSPC);
                    unregister_code(KC_LALT);

                    register_code(KC_LCTL);
                    return false;
                }
            }
            return true;
            break;
        case KC_BSPC:
            if (record->event.pressed) {
                if (get_mods() & MOD_BIT(KC_LCTL) && (get_mods() & MOD_BIT(KC_LALT)) == 0)
                {
                    unregister_code(KC_LCTL);

                    register_code(KC_LALT);
                    tap_code16(KC_BSPC);
                    unregister_code(KC_LALT);

                    register_code(KC_LCTL);
                    return false;
                }
            }
            return true;
            break;

        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case BACKLIT:
            if (record->event.pressed) {
                register_code(KC_RSFT);
#ifdef BACKLIGHT_ENABLE
                backlight_step();
#endif
#ifdef KEYBOARD_planck_rev5
                writePinLow(E6);
#endif
            } else {
                unregister_code(KC_RSFT);
#ifdef KEYBOARD_planck_rev5
                writePinHigh(E6);
#endif
            }
            return false;
            break;
    }

    if(!process_record_vimlayer(keycode, record)) return false;

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;

    }
}
