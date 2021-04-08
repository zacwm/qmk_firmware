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
    NAV_SCLN,
    KVM_SWT,
    COPY,
    LOWER,
    MEH
};

#define RAISE MO(_RAISE)

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
            MEH,     KC_LCTL, KC_LALT, RAISE,   KC_LGUI, LOWER,   KC_SPC,  KC_ENT,  VIM_START,_______,COPY,    PASTE),

    [_LOWER] = LAYOUT_planck_grid(
            KC_TILD, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
            KC_GRV,  KC_LPRN, KC_RPRN, KC_LT,   KC_EQL,  KC_GT,   KC_UNDS, KC_MINS, KC_LBRC, KC_RBRC, KC_SCLN, _______,
            _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, _______, KC_BSLS, KC_ENT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_RAISE] = LAYOUT_planck_grid(
            _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
            KC_LCTL, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______,
            _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_ENT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_NAV] = LAYOUT_planck_grid(
            _______, _______, WORD_R,  WORD_R,  _______, _______, _______, KC_PGUP, GO_FWD,  GO_BCK,  _______, _______,
            _______, LINE_R,  _______, KC_PGDN, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
            _______, _______, CLOSE_W, _______, _______, WORD_L,  _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_ADJUST] = LAYOUT_planck_grid(
            _______, RGB_HUI, RGB_HUD, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
            _______, RGB_SAI, RGB_SAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_ENT,
            RESET,   RGB_TOG, _______, _______, _______, _______, _______, _______, _______, KVM_SWT, CAP_IMG, CAP_MOV),

    [_VIM] = LAYOUT_planck_grid(
            _______, _______, VIM_W,   VIM_E,   VIM_R,   _______, VIM_Y,   VIM_U,   VIM_I,   VIM_O,   VIM_P,   _______,
            VIM_ESC, VIM_A,   VIM_S,   VIM_D,   _______, VIM_G,   VIM_H,   VIM_J,   VIM_K,   VIM_L,   _______, _______,
            VIM_SHIFT,_______,VIM_X,   VIM_C,   VIM_V,   VIM_B,   _______, _______, VIM_COMMA,VIM_PERIOD,_______,_______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, KVM_SWT, CAP_IMG, CAP_MOV),

    [_VINSERT] = LAYOUT_planck_grid(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            VIM_ESC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______)
};

// track the state of CTRL_ESC over multiple key presses.
// 0 - not activated
// 1 - pressed (esc activated and held)
// 2 - consumed (upgraded to ctrl or autoexited on special case keys)
static int ctrl_escape_activated;

// track the state of CTRL_ESC over multiple key presses.
// 0 - not activated
// 1 - activated
static int meh_activated;

// track the state of NAV_SCLN
// 0 - not activated
// 1 - pressed (waiting to decide on semicolon or nav)
// 2 - consumed (upgraded to semicolon or used in nav layer)
static int semicolon_nav_activated;

// keep track of the current kvm target (to play a different sound on switch).
static int kvm_target;

// whether a symbol was typed after lower layer switch
static int lower_consumed;

bool process_lower_specials(uint16_t keycode, keyrecord_t *record) {
    // handle actual layer toggle.
    if (keycode == LOWER)
    {
        lower_consumed = 0;
        if (record->event.pressed)
            layer_on(_LOWER);
        else
            layer_off(_LOWER);

        return false;
    }

    // handle special case keys, where a certain key is pressed immediately following
    // lower layer. this allows special space/backspace when intention is clear
    // and we are not attempting to just backspace or punctuate while typing symbols.
    if (!record->event.pressed || lower_consumed > 0 || !IS_LAYER_ON(_LOWER))
        return true;

    switch (keycode) {
        case LOWER:
            layer_on(_LOWER);
            return false;
        case KC_SPC:
            SEND_STRING(SS_LSFT(SS_TAP(X_SPC)));
            return false;
        case KC_BSPC:
            SEND_STRING(SS_LALT(SS_TAP(X_BSPC)));
            return false;
    }

    lower_consumed = 1;
    return true;
}

bool process_meh(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MEH:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_LALT);
                meh_activated = 1;
                return false;
            }
            else
            {
                clear_mods();
                meh_activated = 0;
                return false;
            }
        case KC_L:
            if (meh_activated == 1)
            {
                clear_mods();

                // lock windows
                SEND_STRING(SS_LGUI(SS_TAP(X_L)));
                // lock macOS and turn off screen
                SEND_STRING(SS_LCTL(SS_LGUI(SS_TAP(X_Q))));
                meh_activated = 0;
                return false;
            }
            break;
    }

    return true;
}

bool process_ctrl_esc(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CTRL_ESC:
            if (record->event.pressed) {
                register_code(KC_ESC);
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
                }

                ctrl_escape_activated = 0;
                return false;
            }
        case KC_J:
        case KC_K:
            if (ctrl_escape_activated == 1)
            {
                unregister_code(KC_LCTL);
                ctrl_escape_activated = 2;
                return true;
            }
            break;
        default:
            if (ctrl_escape_activated == 1)
            {
                unregister_code(KC_ESC);
                register_code(KC_LCTL);
                ctrl_escape_activated = 2;
            }
            break;
    }

    return true;
}

bool process_nav_scln(uint16_t keycode, keyrecord_t *record) {
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

    return true;
}

bool process_macros(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed)
        return true;

    switch (keycode) {
        case COPY:
            SEND_STRING(SS_LGUI("lc"));
            tap_code16(KC_ESC);
            tap_code16(KC_ESC);
            return false;

        case KVM_SWT:
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

            return false;

        case KC_W:
            if (IS_LAYER_ON(_VINSERT)) {
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
            break;

        case KC_BSPC:
            if (get_mods() & MOD_BIT(KC_LCTL))
            {
                uint8_t mod_state = get_mods();
                clear_mods();

                register_code(KC_LALT);
                tap_code16(KC_BSPC);
                unregister_code(KC_LALT);

                set_mods(mod_state);
                return false;
            }
            break;
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_lower_specials(keycode, record)) return false;

    if (!process_record_vimlayer(keycode, record)) return false;

    if (!process_macros(keycode, record)) return false;

    if (!process_nav_scln(keycode, record)) return false;

    if (!process_ctrl_esc(keycode, record)) return false;

    if (!process_meh(keycode, record)) return false;
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
