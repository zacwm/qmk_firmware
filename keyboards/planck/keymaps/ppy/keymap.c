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
    _NAV,
    _VIM,
    _VINSERT,
};

enum planck_keycodes {
    QWERTY = VIM_SAFE_RANGE,
    BACKLIT,
    CTRL_ESC,
    KC_BWRD,
    SFT_ENT,
    NAV_SCLN,
    RAISE_ENT,
    KVM_SWT,
    COPY,
    LOWER_BS,
};

// TODO:
// - rewrite vim layer to not rely in custom keycodes (it shouldn't need to). see https://beta.docs.qmk.fm/using-qmk/advanced-keycodes/feature_leader_key which may be useful
// - fix h->esc

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

#define S_LPRN MT(MOD_LSFT, KC_9)
#define S_RPRN MT(MOD_RSFT, KC_0)

#define S_LBRC MT(MOD_LSFT, KC_LBRC)
#define S_RBRC MT(MOD_RSFT, KC_RBRC)

#define WORD_L LALT(KC_LEFT)
#define WORD_R LALT(KC_RIGHT)

#define LINE_L LCTL(KC_LEFT)
#define LINE_R LCTL(KC_RIGHT)

#define CLOSE_W LGUI(KC_W)

#define GO_BCK LCTL(KC_O)
#define GO_FWD LCTL(KC_I)

#define CMD_ENT LGUI(KC_ENT)

#define PASTE LGUI(KC_V)

#define CAP_IMG LGUI(LSFT(KC_4))        // Capture portion of screen
#define CAP_MOV LGUI(LSFT(KC_5))        // Capture portion of screen

float song_kvm_setting[][2] = SONG(S__NOTE(_C5),S__NOTE(_C6),S__NOTE(_C7));

float song_kvm_0[][2] = SONG(S__NOTE(_C5),S__NOTE(_C5),S__NOTE(_C5),S__NOTE(_C5));
float song_kvm_1[][2] = SONG(S__NOTE(_G5),S__NOTE(_G5),S__NOTE(_G5),S__NOTE(_G5));

uint8_t vim_cmd_layer(void) { return _VIM; }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_planck_grid(
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
            CTRL_ESC,KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    NAV_SCLN,KC_QUOT,
            KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
            KC_MEH,  KC_LCTL, KC_LALT, RAISE,   KC_LGUI, LOWER_BS,KC_SPC,  KC_ENT, VIM_START,_______, COPY,    PASTE),

    [_LOWER] = LAYOUT_planck_grid(
            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
            KC_TILD, KC_LPRN, KC_RPRN, KC_LT,   KC_EQL,  KC_GT,   KC_UNDS, KC_MINS, KC_LBRC, KC_RBRC, KC_SCLN, KC_GRV,
            _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, _______, KC_BSLS, KC_ENT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_RAISE] = LAYOUT_planck_grid(
            _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, _______, _______, _______, _______, _______, KC_DEL,
            _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, _______, _______, _______, _______, _______, _______,
            _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______, _______, KC_ENT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_NAV] = LAYOUT_planck_grid(
            _______, _______, WORD_R,  WORD_R,  _______, _______, _______, KC_PGUP, GO_FWD,  GO_BCK,  _______, _______,
            _______, LINE_R,  _______, KC_PGDN, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
            _______, _______, CLOSE_W, _______, _______, WORD_L,  _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_ADJUST] = LAYOUT_planck_grid(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_ENT,
            RESET,   RGB_TOG, _______, _______, _______, _______, _______, _______, _______, KVM_SWT, CAP_IMG, CAP_MOV),

    [_VIM] = LAYOUT_planck_grid(
            _______, _______, VIM_W,   VIM_E,   VIM_R,   _______, VIM_Y,   VIM_U,   VIM_I,   VIM_O,   VIM_P,   _______,
            VIM_ESC, VIM_A,   VIM_S,   VIM_D,   _______, VIM_G,   VIM_H,   VIM_J,   VIM_K,   VIM_L,   _______, KC_ENT,
            VIM_SHIFT,_______,VIM_X,   VIM_C,   VIM_V,   VIM_B,   _______, _______, VIM_COMMA,VIM_PERIOD,_______,KC_ENT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, KVM_SWT, CAP_IMG, CAP_MOV),

    [_VINSERT] = LAYOUT_planck_grid(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,_______)

};

static int ctrl_escape_activated;
static int keyword_activated;
static int semicolon_nav_activated;

static int kvm_target = 0;

static int lower_consumed = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if(!process_record_vimlayer(keycode, record)) return false;

    switch (keycode) {
        case COPY:
            if (record->event.pressed) {
                SEND_STRING(SS_LGUI("lc"));
                tap_code16(KC_ESC);
                tap_code16(KC_ESC);
            }
            return false;
        case LOWER_BS:
            if (record->event.pressed) {
                lower_consumed = 0;
                layer_on(_LOWER);
                return false;
            }
            else {
                layer_off(_LOWER);
                if (lower_consumed == 0)
                {
                    SEND_STRING(SS_LALT(SS_TAP(X_BSPC)));
                }

                return false;
            }

            break;
        case KC_BWRD:
            if (record->event.pressed) {
                keyword_activated = 1;
                return false;
            }
            else {
                if (keyword_activated == 2)
                {
                    tap_code16(KC_GRV);
                }
                else if (keyword_activated == 1)
                {
                    register_code(KC_LALT);
                    tap_code16(KC_BSPC);
                    unregister_code(KC_LALT);
                }

                keyword_activated = 0;
                return false;
            }

            break;
    }

    if (keyword_activated == 1)
    {
        tap_code16(KC_GRV);
        keyword_activated = 2;
        return true;
    }

    lower_consumed = 1;

    switch (keycode) {
        case NAV_SCLN:
            if (record->event.pressed) {
                if (get_mods() & MOD_BIT(KC_LSFT))
                {
                    tap_code16(KC_COLN);
                    return true;
                }

                semicolon_nav_activated = 1;
                layer_on(_NAV);
                return false;
            }
            else
            {
                if (semicolon_nav_activated == 1)
                    tap_code16(KC_SCLN);

                semicolon_nav_activated = 0;
                layer_off(_NAV);
                return false;
            }
        case KC_UP:
        case KC_DOWN:
        case KC_LEFT:
        case KC_RGHT:
        case KC_ENT:
        case KC_PGUP:
        case KC_PGDN:
        case WORD_L:
        case WORD_R:
        case LINE_L:
        case LINE_R:
        case GO_FWD:
        case GO_BCK:
        case CLOSE_W:
            if (semicolon_nav_activated == 1)
            {
                semicolon_nav_activated = 2;
                return true;
            }
        default:
            if (semicolon_nav_activated == 1)
            {
                tap_code16(KC_SCLN);
                semicolon_nav_activated = 2;
                break;
            }
    }

    switch (keycode) {
        case CTRL_ESC:
            if (record->event.pressed) {
                register_code(KC_ESC);
                register_code(KC_LCTL);
                ctrl_escape_activated = 1;
                return false;
            }
            else
            {
                unregister_code(KC_ESC);
                unregister_code(KC_LCTL);
                if (ctrl_escape_activated == 1)
                {
                    if (IS_LAYER_ON(_VINSERT))
                        layer_move(_VIM);
                    /* else */
                    /*     tap_code16(KC_ESC); */
                }

                ctrl_escape_activated = 0;
                return false;
            }
        case KC_J:
        case KC_K:
            if (ctrl_escape_activated == 1)
            {
                unregister_code(KC_LCTL);
                /* tap_code16(KC_ESC); */
                ctrl_escape_activated = 2;
                return true;
            }
            break;
        default:
            if (ctrl_escape_activated == 1)
                ctrl_escape_activated = 2;
            break;
    }

    switch (keycode) {
        case KVM_SWT:
            if (record->event.pressed) {
                if (get_mods() & MOD_BIT(KC_LCTL))
                {
                    SEND_STRING(SS_LCTL(SS_TAP(X_F12)) "t");
                    PLAY_SONG(song_kvm_setting);
                    return false;
                }

                tap_code16(KC_SCROLLLOCK);
                tap_code16(KC_SCROLLLOCK);

                // track the current target device to give audible feedback
                kvm_target = (kvm_target + 1) % 2;

                if (kvm_target == 1)
                    PLAY_SONG(song_kvm_1);
                else
                    PLAY_SONG(song_kvm_0);
            }

            return false;
            break;
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

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
