#include QMK_KEYBOARD_H
#include "xtonhasvim.h"
#include "common.h"

float song_kvm_setting[][2] = SONG(S__NOTE(_C5),S__NOTE(_C6),S__NOTE(_C7));

float song_kvm_0[][2] = SONG(S__NOTE(_C5),S__NOTE(_C5),S__NOTE(_C5),S__NOTE(_C5));
float song_kvm_1[][2] = SONG(S__NOTE(_G5),S__NOTE(_G5),S__NOTE(_G5),S__NOTE(_G5));

uint8_t vim_cmd_layer(void) { return _VIM; }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_planck_grid(
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
            CTRL_ESC,KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    NAV_SCLN,KC_QUOT,
            KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
            MEH,     KC_LCTL, KC_LALT, FKEYS,   RAISE,   LOWER,   KC_SPC,  KC_ENT,  VIM_START,_______,COPY,    PASTE),

    [_LOWER] = LAYOUT_planck_grid(
            KC_TILD, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
            KC_GRV,  KC_LPRN, KC_RPRN, KC_LT,   KC_EQL,  KC_GT,   KC_UNDS, KC_MINS, KC_LBRC, KC_RBRC, KC_SCLN, _______,
            KC_LCBR, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, _______, KC_BSLS, KC_RCBR,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    // this is mainly here just to use layer_state_set_user
    // raise layer is actually a LGUI key, so any keys here would currently be prefixed with LGUI.
    [_RAISE] = LAYOUT_planck_grid(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_FKEYS] = LAYOUT_planck_grid(
            _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
            KC_LCTL, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______,
            _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_ENT,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_NAV] = LAYOUT_planck_grid(
            _______, _______, WORD_R,  WORD_R,  _______, _______, _______, KC_PGUP, LINE_L,  _______, _______, _______,
            _______, LINE_R,  _______, KC_PGDN, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
            _______, _______, CLOSE_W, _______, _______, WORD_L,  _______, _______, _______, _______, _______, _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_ADJUST] = LAYOUT_planck_grid(
            _______, RGB_HUI, RGB_HUD, _______, DM_REC1, _______, _______, _______, _______, _______, DM_PLY1, KC_DEL,
            _______, RGB_SAI, RGB_SAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,
            _______, _______, _______, KC_LOCK, _______, _______, _______, _______, CK_TOGG, CK_UP,   CK_DOWN, _______,
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

// track the state of grave surround
// 0 - not activated
// 1 - started (cursor placed between ``)
// 2 - consumed (success or revert)
static int grave_surround_state;

bool process_raise_specials(uint16_t keycode, keyrecord_t *record) {
    if (keycode == RAISE)
    {
        if (record->event.pressed)
        {
            layer_on(_RAISE);
            register_code(KC_LGUI);
        }
        else
        {
            layer_off(_RAISE);
            unregister_code(KC_LGUI);
        }

        return false;
    }

    return true;
}

bool process_grave_surround(uint16_t keycode, keyrecord_t *record) {
    if (keycode == LOWER)
    {
        if (record->event.pressed)
        {
            switch (grave_surround_state)
            {
                case 1:
                    tap_code16(KC_RGHT);
                    tap_code16(KC_BSPC);
                    tap_code16(KC_BSPC);
                    break;
                case 2:
                    tap_code16(KC_RGHT);
                    tap_code16(KC_SPC);
                    break;
            }
        }
        else
        {
            switch (grave_surround_state)
            {
                case 0:
                    if (lower_consumed == 0 && get_mods() == 0)
                    {
                        // begin grave surround sequence.
                        // starts on a release but ends on the next press (see above).
                        tap_code16(KC_GRV);
                        tap_code16(KC_GRV);
                        tap_code16(KC_LEFT);
                        grave_surround_state = 1;
                    }
                    break;

                default:
                    grave_surround_state = 0;
                    break;
            }
        }

        return true;
    }

    if (grave_surround_state == 1)
        grave_surround_state++;

    return true;
}

bool process_lower_specials(uint16_t keycode, keyrecord_t *record) {
    // handle actual layer toggle.
    if (keycode == LOWER)
    {
        if (record->event.pressed)
        {
            lower_consumed = 0;
            layer_on(_LOWER);
        }
        else
            layer_off(_LOWER);

        return false;
    }

    if (!IS_LAYER_ON(_LOWER) || lower_consumed > 0)
        return true;

    // consumed even on a release of another key to avoid false firings of grave surround.
    lower_consumed = 1;

    // handle special case keys, where a certain key is pressed immediately following
    // lower layer. this allows special space/backspace when intention is clear
    // and we are not attempting to just backspace or punctuate while typing symbols.
    if (!record->event.pressed)
        return true;

    switch (keycode) {
        case KC_SPC:
            SEND_STRING(SS_LSFT(SS_TAP(X_SPC)));
            return false;
        case KC_BSPC:
            SEND_STRING(SS_LALT(SS_TAP(X_BSPC)));
            return false;
    }

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
            }
            else
            {
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                unregister_code(KC_LALT);
                meh_activated = 0;
            }

            return true;
        case KC_Q:
            if (meh_activated == 1)
            {
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                unregister_code(KC_LALT);

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
    if (!process_raise_specials(keycode, record)) return false;

    if (!process_grave_surround(keycode, record)) return false;

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
