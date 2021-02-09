/*
 * Copyright 2011 Jun Wako <wakojun@gmail.com>
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

#include <stddef.h>
#include <stdint.h>
#include "keycode.h"
#include "host.h"
#include "timer.h"
#include "print.h"
#include "debug.h"
#include "mousekey.h"

/* Figure out which variant the user wants. */
#ifndef MK_VARIANT

#    if !defined(MK_3_SPEED) && !defined(MK_KINETIC_SPEED)
#        if defined(MK_COMBINED)
#            define MK_VARIANT MK_TYPE_X11_COMBINED
#        else
#            define MK_VARIANT MK_TYPE_X11 /* Default */
#        endif
#        if defined(MK_MOMENTARY_ACCEL)
#            warning "MK_MOMENTARY_ACCEL ignored for MK_TYPE_X11"
#        endif

#    elif defined(MK_3_SPEED) && !defined(MK_KINETIC_SPEED)
#        if defined(MK_MOMENTARY_ACCEL)
#            define MK_VARIANT MK_TYPE_3_SPEED_MOMENTARY
#        else
#            define MK_VARIANT MK_TYPE_3_SPEED
#        endif
#        if defined(MK_COMBINED)
#            warning "MK_COMBINED ignored for MK_TYPE_3_SPEED"
#        endif

#    elif !defined(MK_3_SPEED) && defined(MK_KINETIC_SPEED)
#        define MK_VARIANT MK_TYPE_KINETIC
#        if defined(MK_COMBINED)
#            warning "MK_COMBINED ignored for MK_TYPE_KINETIC"
#        endif
#        if defined(MK_MOMENTARY_ACCEL)
#            warning "MK_MOMENTARY_ACCEL ignored for MK_TYPE_KINETIC"
#        endif

#    elif defined(MK_3_SPEED) && defined(MK_KINETIC_SPEED)
#        error "#define at most one of MK_3_SPEED MK_KINETIC_SPEED"
#    else
#        error "Inconceivable! I don't know what you want?"
#    endif

#endif

/*
 * Do not contemplate `MK_3_SPEED` nor `MK_KINETIC_SPEED` henceforth—nor
 * utter `MK_COMBINED` nor `MK_MOMENTARY_ACCEL` either—on pain of death.
 * Furnished below are macros to test for `MK_TYPE_*`.
 */
#define MK_KIND(v) ((MK_VARIANT & ~0xff) == ((v) & ~0xff))
#define MK_TYPE(v) (MK_VARIANT == (v))

#if MK_KIND(MK_TYPE_X11)

#    ifndef MOUSEKEY_MOVE_MAX
#        define MOUSEKEY_MOVE_MAX INT8_MAX
#    elif MOUSEKEY_MOVE_MAX > INT8_MAX
#        error MOUSEKEY_MOVE_MAX cannot be greater than 127
#    endif
#    ifndef MOUSEKEY_WHEEL_MAX
#        define MOUSEKEY_WHEEL_MAX INT8_MAX
#    elif MOUSEKEY_WHEEL_MAX > INT8_MAX
#        error MOUSEKEY_WHEEL_MAX cannot be greater than 127
#    endif

#    ifndef MOUSEKEY_MOVE_DELTA
#        define MOUSEKEY_MOVE_DELTA 5
#    endif
#    ifndef MOUSEKEY_WHEEL_DELTA
#        define MOUSEKEY_WHEEL_DELTA 1
#    endif
#    ifndef MOUSEKEY_DELAY
#        define MOUSEKEY_DELAY 300
#    endif
#    ifndef MOUSEKEY_INTERVAL
#        define MOUSEKEY_INTERVAL 50
#    endif
#    ifndef MOUSEKEY_MAX_SPEED
#        define MOUSEKEY_MAX_SPEED 10
#    endif
#    ifndef MOUSEKEY_TIME_TO_MAX
#        define MOUSEKEY_TIME_TO_MAX 20
#    endif
#    ifndef MOUSEKEY_WHEEL_DELAY
#        define MOUSEKEY_WHEEL_DELAY 300
#    endif
#    ifndef MOUSEKEY_WHEEL_INTERVAL
#        define MOUSEKEY_WHEEL_INTERVAL 100
#    endif
#    ifndef MOUSEKEY_WHEEL_MAX_SPEED
#        define MOUSEKEY_WHEEL_MAX_SPEED 8
#    endif
#    ifndef MOUSEKEY_WHEEL_TIME_TO_MAX
#        define MOUSEKEY_WHEEL_TIME_TO_MAX 40
#    endif

#elif MK_KIND(MK_TYPE_3_SPEED)

#    ifndef MK_C_OFFSET_UNMOD
#        define MK_C_OFFSET_UNMOD 16
#    endif
#    ifndef MK_C_INTERVAL_UNMOD
#        define MK_C_INTERVAL_UNMOD 16
#    endif
#    ifndef MK_C_OFFSET_0
#        define MK_C_OFFSET_0 1
#    endif
#    ifndef MK_C_INTERVAL_0
#        define MK_C_INTERVAL_0 32
#    endif
#    ifndef MK_C_OFFSET_1
#        define MK_C_OFFSET_1 4
#    endif
#    ifndef MK_C_INTERVAL_1
#        define MK_C_INTERVAL_1 16
#    endif
#    ifndef MK_C_OFFSET_2
#        define MK_C_OFFSET_2 32
#    endif
#    ifndef MK_C_INTERVAL_2
#        define MK_C_INTERVAL_2 16
#    endif

#    ifndef MK_W_OFFSET_UNMOD
#        define MK_W_OFFSET_UNMOD 1
#    endif
#    ifndef MK_W_INTERVAL_UNMOD
#        define MK_W_INTERVAL_UNMOD 40
#    endif
#    ifndef MK_W_OFFSET_0
#        define MK_W_OFFSET_0 1
#    endif
#    ifndef MK_W_INTERVAL_0
#        define MK_W_INTERVAL_0 360
#    endif
#    ifndef MK_W_OFFSET_1
#        define MK_W_OFFSET_1 1
#    endif
#    ifndef MK_W_INTERVAL_1
#        define MK_W_INTERVAL_1 120
#    endif
#    ifndef MK_W_OFFSET_2
#        define MK_W_OFFSET_2 1
#    endif
#    ifndef MK_W_INTERVAL_2
#        define MK_W_INTERVAL_2 20
#    endif

#elif MK_KIND(MK_TYPE_KINETIC)

/* TODO: Rename these four! */
/* Names coincide with those of MK_TYPE_X11 but have different semantics. */
#    ifndef MOUSEKEY_MOVE_MAX
#        define MOUSEKEY_MOVE_MAX INT8_MAX
#    elif MOUSEKEY_MOVE_MAX > INT8_MAX
#        error MOUSEKEY_MOVE_MAX cannot be greater than INT8_MAX
#    endif
#    ifndef MOUSEKEY_MOVE_DELTA
#        define MOUSEKEY_MOVE_DELTA 25
#    endif
#    ifndef MOUSEKEY_DELAY
#        define MOUSEKEY_DELAY 8
#    endif
#    ifndef MOUSEKEY_INTERVAL
#        define MOUSEKEY_INTERVAL 8
#    endif

/* Same semantics, maybe? */
#    ifndef MOUSEKEY_MAX_SPEED
#        define MOUSEKEY_MAX_SPEED 10
#    endif
#    ifndef MOUSEKEY_WHEEL_MAX_SPEED
#        define MOUSEKEY_WHEEL_MAX_SPEED 8
#    endif

#    ifndef MOUSEKEY_TIME_TO_MAX
#        define MOUSEKEY_TIME_TO_MAX 20
#    endif
#    ifndef MOUSEKEY_WHEEL_TIME_TO_MAX
#        define MOUSEKEY_WHEEL_TIME_TO_MAX 40
#    endif

/* Unique to MK_TYPE_KINETIC */
#    ifndef MOUSEKEY_INITIAL_SPEED
#        define MOUSEKEY_INITIAL_SPEED 100
#    endif
#    ifndef MOUSEKEY_BASE_SPEED
#        define MOUSEKEY_BASE_SPEED 1000
#    endif
#    ifndef MOUSEKEY_DECELERATED_SPEED
#        define MOUSEKEY_DECELERATED_SPEED 400
#    endif
#    ifndef MOUSEKEY_ACCELERATED_SPEED
#        define MOUSEKEY_ACCELERATED_SPEED 3000
#    endif
#    ifndef MOUSEKEY_WHEEL_INITIAL_MOVEMENTS
#        define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 16
#    endif
#    ifndef MOUSEKEY_WHEEL_BASE_MOVEMENTS
#        define MOUSEKEY_WHEEL_BASE_MOVEMENTS 32
#    endif
#    ifndef MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS
#        define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS 48
#    endif
#    ifndef MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS
#        define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS 8
#    endif

#else
#    error "Unknown MK_VARIANT"
#endif /* MK_VARIANT */

/**********************************************************************/

#pragma GCC diagnostic error "-Wextra"

#define NO_FIELD(no_field)                                              \
    _Pragma("GCC diagnostic push");                                     \
    _Pragma("GCC diagnostic ignored \"-Wmissing-field-initializers\""); \
    no_field;                                                           \
    _Pragma("GCC diagnostic pop")

inline int8_t times_inv_sqrt2(int8_t x) {
    // 181/256 is pretty close to 1/sqrt(2)
    // 0.70703125                 0.707106781
    // 1 too small for x=99 and x=198
    // This ends up being a mult and discard lower 8 bits
    return (x * 181) >> 8;
}

static report_mouse_t mouse_report = {0};

static void mousekey_debug(void);

static uint16_t last_timer_c = 0;
static uint16_t last_timer_w = 0;

#if MK_KIND(MK_TYPE_KINETIC)
/* XXX: overflows if key held down > 65s. */
static uint16_t mouse_timer = 0;
#endif

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
/* TODO: untangle MK_TYPE_KINETIC.
 * Currently shares config/state w/ X11 but probably shouldn't. */

/* KC_MS_ACCEL{0,1,2} state bitflag */
static uint8_t mousekey_accel = 0;

static uint8_t mousekey_repeat       = 0;
static uint8_t mousekey_wheel_repeat = 0;

/*
 * Mouse keys acceleration algorithm
 *  http://en.wikipedia.org/wiki/Mouse_keys
 *
 *  speed = delta * max_speed * (repeat / time_to_max)**((1000+curve)/1000)
 */
/* milliseconds between the initial key press and first repeated motion event (0-2550) */
uint8_t mk_delay = MOUSEKEY_DELAY / 10;
/* milliseconds between repeated motion events (0-255) */
uint8_t mk_interval = MOUSEKEY_INTERVAL;
/* steady speed (in action_delta units) applied each event (0-255) */
uint8_t mk_max_speed = MOUSEKEY_MAX_SPEED;
/* number of events (count) accelerating to steady speed (0-255) */
uint8_t mk_time_to_max = MOUSEKEY_TIME_TO_MAX;
/* ramp used to reach maximum pointer speed (NOT SUPPORTED) */
// int8_t mk_curve = 0;
/* wheel params */
/* milliseconds between the initial key press and first repeated motion event (0-2550) */
uint8_t mk_wheel_delay = MOUSEKEY_WHEEL_DELAY / 10;
/* milliseconds between repeated motion events (0-255) */
uint8_t mk_wheel_interval    = MOUSEKEY_WHEEL_INTERVAL;
uint8_t mk_wheel_max_speed   = MOUSEKEY_WHEEL_MAX_SPEED;
uint8_t mk_wheel_time_to_max = MOUSEKEY_WHEEL_TIME_TO_MAX;

#endif /* MK_TYPE_X11 || MK_TYPE_KINETIC */

#if MK_TYPE(MK_TYPE_X11)

static uint8_t move_unit(void) {
    uint16_t unit;
    if (mousekey_accel & (1 << 0)) {
        unit = (MOUSEKEY_MOVE_DELTA * mk_max_speed) / 4;
    } else if (mousekey_accel & (1 << 1)) {
        unit = (MOUSEKEY_MOVE_DELTA * mk_max_speed) / 2;
    } else if (mousekey_accel & (1 << 2)) {
        unit = (MOUSEKEY_MOVE_DELTA * mk_max_speed);
    } else if (mousekey_repeat == 0) {
        unit = MOUSEKEY_MOVE_DELTA;
    } else if (mousekey_repeat >= mk_time_to_max) {
        unit = MOUSEKEY_MOVE_DELTA * mk_max_speed;
    } else {
        unit = (MOUSEKEY_MOVE_DELTA * mk_max_speed * mousekey_repeat) / mk_time_to_max;
    }
    return (unit > MOUSEKEY_MOVE_MAX ? MOUSEKEY_MOVE_MAX : (unit == 0 ? 1 : unit));
}

static uint8_t wheel_unit(void) {
    uint16_t unit;
    if (mousekey_accel & (1 << 0)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed) / 4;
    } else if (mousekey_accel & (1 << 1)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed) / 2;
    } else if (mousekey_accel & (1 << 2)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed);
    } else if (mousekey_wheel_repeat == 0) {
        unit = MOUSEKEY_WHEEL_DELTA;
    } else if (mousekey_wheel_repeat >= mk_wheel_time_to_max) {
        unit = MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed;
    } else {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed * mousekey_wheel_repeat) / mk_wheel_time_to_max;
    }
    return (unit > MOUSEKEY_WHEEL_MAX ? MOUSEKEY_WHEEL_MAX : (unit == 0 ? 1 : unit));
}

#elif MK_KIND(MK_TYPE_KINETIC)

/*
 * Kinetic movement  acceleration algorithm
 *
 *  current speed = I + A * T/50 + A * 0.5 * T^2 | maximum B
 *
 * T: time since the mouse movement started
 * I: initial speed at time 0
 * A: acceleration
 * B: base mouse travel speed
 */

static uint8_t move_unit(void) {
    float speed = MOUSEKEY_INITIAL_SPEED;

    if (mousekey_accel & ((1 << 0) | (1 << 2))) {
        speed = mousekey_accel & (1 << 2) ? MOUSEKEY_ACCELERATED_SPEED : MOUSEKEY_DECELERATED_SPEED;
    } else if (mousekey_repeat && mouse_timer) {
        const float time_elapsed = timer_elapsed(mouse_timer) / 50;
        speed                    = MOUSEKEY_INITIAL_SPEED + MOUSEKEY_MOVE_DELTA * time_elapsed + MOUSEKEY_MOVE_DELTA * 0.5 * time_elapsed * time_elapsed;

        speed = speed > MOUSEKEY_BASE_SPEED ? MOUSEKEY_BASE_SPEED : speed;
    }

    /* convert speed to USB mouse speed 1 to 127 */
    speed = (uint8_t)(speed / (1000.0f / mk_interval));
    speed = speed < 1 ? 1 : speed;

    return speed > MOUSEKEY_MOVE_MAX ? MOUSEKEY_MOVE_MAX : speed;
}

/* XXX: was redefⁿ of MK_TYPE_X11's mk_wheel_interval as float */
static float kinetic_wheel_interval = 1000.0f / MOUSEKEY_WHEEL_INITIAL_MOVEMENTS;

static uint8_t wheel_unit(void) {
    float speed = MOUSEKEY_WHEEL_INITIAL_MOVEMENTS;

    if (mousekey_accel & ((1 << 0) | (1 << 2))) {
        speed = mousekey_accel & (1 << 2) ? MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS : MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS;
    } else if (mousekey_wheel_repeat /* XXX: was mousekey_repeat */ && mouse_timer) {
        if (kinetic_wheel_interval != MOUSEKEY_WHEEL_BASE_MOVEMENTS) {
            const float time_elapsed = timer_elapsed(mouse_timer) / 50;
            speed                    = MOUSEKEY_WHEEL_INITIAL_MOVEMENTS + 1 * time_elapsed + 1 * 0.5 * time_elapsed * time_elapsed;
        }
        speed = speed > MOUSEKEY_WHEEL_BASE_MOVEMENTS ? MOUSEKEY_WHEEL_BASE_MOVEMENTS : speed;
    }

    kinetic_wheel_interval = 1000.0f / speed;

    return 1; /* XXX: wat. */
}

#elif MK_TYPE(MK_TYPE_X11_COMBINED)

static uint8_t move_unit(void) {
    uint16_t unit;
    if (mousekey_accel & (1 << 0)) {
        unit = 1;
    } else if (mousekey_accel & (1 << 1)) {
        unit = (MOUSEKEY_MOVE_DELTA * mk_max_speed) / 2;
    } else if (mousekey_accel & (1 << 2)) {
        unit = MOUSEKEY_MOVE_MAX;
    } else if (mousekey_repeat == 0) {
        unit = MOUSEKEY_MOVE_DELTA;
    } else if (mousekey_repeat >= mk_time_to_max) {
        unit = MOUSEKEY_MOVE_DELTA * mk_max_speed;
    } else {
        unit = (MOUSEKEY_MOVE_DELTA * mk_max_speed * mousekey_repeat) / mk_time_to_max;
    }
    return (unit > MOUSEKEY_MOVE_MAX ? MOUSEKEY_MOVE_MAX : (unit == 0 ? 1 : unit));
}

static uint8_t wheel_unit(void) {
    uint16_t unit;
    if (mousekey_accel & (1 << 0)) {
        unit = 1;
    } else if (mousekey_accel & (1 << 1)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed) / 2;
    } else if (mousekey_accel & (1 << 2)) {
        unit = MOUSEKEY_WHEEL_MAX;
    } else if (mousekey_repeat == 0) {
        unit = MOUSEKEY_WHEEL_DELTA;
    } else if (mousekey_repeat >= mk_wheel_time_to_max) {
        unit = MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed;
    } else {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed * mousekey_repeat) / mk_wheel_time_to_max;
    }
    return (unit > MOUSEKEY_WHEEL_MAX ? MOUSEKEY_WHEEL_MAX : (unit == 0 ? 1 : unit));
}

#endif /* MK_TYPE_X11 || MK_TYPE_KINETIC || MK_TYPE_X11_COMBINED */

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)

void mousekey_task(void) {
    // report cursor and scroll movement independently
    report_mouse_t const tmpmr = mouse_report;

    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.v = 0;
    mouse_report.h = 0;

    if ((tmpmr.x || tmpmr.y) && timer_elapsed(last_timer_c) > (mousekey_repeat ? mk_interval : mk_delay * 10)) {
        if (mousekey_repeat != UINT8_MAX) mousekey_repeat++;
        if (tmpmr.x != 0) mouse_report.x = move_unit() * ((tmpmr.x > 0) ? 1 : -1);
        if (tmpmr.y != 0) mouse_report.y = move_unit() * ((tmpmr.y > 0) ? 1 : -1);

        /* diagonal move [1/sqrt(2)] */
        if (mouse_report.x && mouse_report.y) {
            mouse_report.x = times_inv_sqrt2(mouse_report.x);
            if (mouse_report.x == 0) {
                mouse_report.x = 1;
            }
            mouse_report.y = times_inv_sqrt2(mouse_report.y);
            if (mouse_report.y == 0) {
                mouse_report.y = 1;
            }
        }
    }

#if MK_KIND(MK_TYPE_KINETIC)
    /* XXX previously we just redefined mk_wheel_interval as a float */
    mk_wheel_interval = (uint8_t)kinetic_wheel_interval;
#endif
    if ((tmpmr.v || tmpmr.h) && timer_elapsed(last_timer_w) > (mousekey_wheel_repeat ? mk_wheel_interval : mk_wheel_delay * 10)) {
        if (mousekey_wheel_repeat != UINT8_MAX) mousekey_wheel_repeat++;
        if (tmpmr.v != 0) mouse_report.v = wheel_unit() * ((tmpmr.v > 0) ? 1 : -1);
        if (tmpmr.h != 0) mouse_report.h = wheel_unit() * ((tmpmr.h > 0) ? 1 : -1);

        /* diagonal move [1/sqrt(2)] */
        if (mouse_report.v && mouse_report.h) {
            mouse_report.v = times_inv_sqrt2(mouse_report.v);
            if (mouse_report.v == 0) {
                mouse_report.v = 1;
            }
            mouse_report.h = times_inv_sqrt2(mouse_report.h);
            if (mouse_report.h == 0) {
                mouse_report.h = 1;
            }
        }
    }

    if (mouse_report.x || mouse_report.y || mouse_report.v || mouse_report.h) mousekey_send();
    mouse_report = tmpmr;
}

void mousekey_on(uint8_t code) {
#    if MK_KIND(MK_TYPE_KINETIC)
    if (mouse_timer == 0) {
        mouse_timer = timer_read();
    }
#    endif /* MK_TYPE_KINETIC */

    if (code == KC_MS_UP)
        mouse_report.y = move_unit() * -1;
    else if (code == KC_MS_DOWN)
        mouse_report.y = move_unit();
    else if (code == KC_MS_LEFT)
        mouse_report.x = move_unit() * -1;
    else if (code == KC_MS_RIGHT)
        mouse_report.x = move_unit();
    else if (code == KC_MS_WH_UP)
        mouse_report.v = wheel_unit();
    else if (code == KC_MS_WH_DOWN)
        mouse_report.v = wheel_unit() * -1;
    else if (code == KC_MS_WH_LEFT)
        mouse_report.h = wheel_unit() * -1;
    else if (code == KC_MS_WH_RIGHT)
        mouse_report.h = wheel_unit();
    else if (IS_MOUSEKEY_BUTTON(code))
        mouse_report.buttons |= 1 << (code - KC_MS_BTN1);
    else if (code == KC_MS_ACCEL0)
        mousekey_accel |= (1 << 0);
    else if (code == KC_MS_ACCEL1)
        mousekey_accel |= (1 << 1);
    else if (code == KC_MS_ACCEL2)
        mousekey_accel |= (1 << 2);
}

void mousekey_off(uint8_t code) {
    if (code == KC_MS_UP && mouse_report.y < 0)
        mouse_report.y = 0;
    else if (code == KC_MS_DOWN && mouse_report.y > 0)
        mouse_report.y = 0;
    else if (code == KC_MS_LEFT && mouse_report.x < 0)
        mouse_report.x = 0;
    else if (code == KC_MS_RIGHT && mouse_report.x > 0)
        mouse_report.x = 0;
    else if (code == KC_MS_WH_UP && mouse_report.v > 0)
        mouse_report.v = 0;
    else if (code == KC_MS_WH_DOWN && mouse_report.v < 0)
        mouse_report.v = 0;
    else if (code == KC_MS_WH_LEFT && mouse_report.h < 0)
        mouse_report.h = 0;
    else if (code == KC_MS_WH_RIGHT && mouse_report.h > 0)
        mouse_report.h = 0;
    else if (IS_MOUSEKEY_BUTTON(code))
        mouse_report.buttons &= ~(1 << (code - KC_MS_BTN1));
    else if (code == KC_MS_ACCEL0)
        mousekey_accel &= ~(1 << 0);
    else if (code == KC_MS_ACCEL1)
        mousekey_accel &= ~(1 << 1);
    else if (code == KC_MS_ACCEL2)
        mousekey_accel &= ~(1 << 2);
    if (mouse_report.x == 0 && mouse_report.y == 0) {
        mousekey_repeat = 0;
#    if MK_KIND(MK_TYPE_KINETIC)
        mouse_timer = 0;
#    endif /* MK_TYPE_KINETIC */
    }
    if (mouse_report.v == 0 && mouse_report.h == 0) mousekey_wheel_repeat = 0;
}

#endif /* MK_TYPE_X11 || MK_TYPE_KINETIC */

#if MK_KIND(MK_TYPE_3_SPEED)

enum { mkspd_unmod, mkspd_0, mkspd_1, mkspd_2, mkspd_COUNT };
#    if MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
static uint8_t mk_speed = mkspd_unmod;
#    else
static uint8_t mk_speed = mkspd_1;
#    endif

static const uint16_t c_offsets[mkspd_COUNT]   = {MK_C_OFFSET_UNMOD, MK_C_OFFSET_0, MK_C_OFFSET_1, MK_C_OFFSET_2};
static const uint16_t c_intervals[mkspd_COUNT] = {MK_C_INTERVAL_UNMOD, MK_C_INTERVAL_0, MK_C_INTERVAL_1, MK_C_INTERVAL_2};
static const uint16_t w_offsets[mkspd_COUNT]   = {MK_W_OFFSET_UNMOD, MK_W_OFFSET_0, MK_W_OFFSET_1, MK_W_OFFSET_2};
static const uint16_t w_intervals[mkspd_COUNT] = {MK_W_INTERVAL_UNMOD, MK_W_INTERVAL_0, MK_W_INTERVAL_1, MK_W_INTERVAL_2};

void mousekey_task(void) {
    // report cursor and scroll movement independently
    report_mouse_t const tmpmr = mouse_report;
    mouse_report.x             = 0;
    mouse_report.y             = 0;
    mouse_report.v             = 0;
    mouse_report.h             = 0;

    if ((tmpmr.x || tmpmr.y) && timer_elapsed(last_timer_c) > c_intervals[mk_speed]) {
        mouse_report.x = tmpmr.x;
        mouse_report.y = tmpmr.y;
    }
    if ((tmpmr.h || tmpmr.v) && timer_elapsed(last_timer_w) > w_intervals[mk_speed]) {
        mouse_report.v = tmpmr.v;
        mouse_report.h = tmpmr.h;
    }

    if (mouse_report.x || mouse_report.y || mouse_report.v || mouse_report.h) mousekey_send();
    mouse_report = tmpmr;
}

void adjust_speed(void) {
    uint16_t const c_offset = c_offsets[mk_speed];
    uint16_t const w_offset = w_offsets[mk_speed];
    if (mouse_report.x > 0) mouse_report.x = c_offset;
    if (mouse_report.x < 0) mouse_report.x = c_offset * -1;
    if (mouse_report.y > 0) mouse_report.y = c_offset;
    if (mouse_report.y < 0) mouse_report.y = c_offset * -1;
    if (mouse_report.h > 0) mouse_report.h = w_offset;
    if (mouse_report.h < 0) mouse_report.h = w_offset * -1;
    if (mouse_report.v > 0) mouse_report.v = w_offset;
    if (mouse_report.v < 0) mouse_report.v = w_offset * -1;
    // adjust for diagonals
    if (mouse_report.x && mouse_report.y) {
        mouse_report.x = times_inv_sqrt2(mouse_report.x);
        if (mouse_report.x == 0) {
            mouse_report.x = 1;
        }
        mouse_report.y = times_inv_sqrt2(mouse_report.y);
        if (mouse_report.y == 0) {
            mouse_report.y = 1;
        }
    }
    if (mouse_report.h && mouse_report.v) {
        mouse_report.h = times_inv_sqrt2(mouse_report.h);
        mouse_report.v = times_inv_sqrt2(mouse_report.v);
    }
}

void mousekey_on(uint8_t code) {
    uint16_t const c_offset  = c_offsets[mk_speed];
    uint16_t const w_offset  = w_offsets[mk_speed];
    uint8_t const  old_speed = mk_speed;
    if (code == KC_MS_UP)
        mouse_report.y = c_offset * -1;
    else if (code == KC_MS_DOWN)
        mouse_report.y = c_offset;
    else if (code == KC_MS_LEFT)
        mouse_report.x = c_offset * -1;
    else if (code == KC_MS_RIGHT)
        mouse_report.x = c_offset;
    else if (code == KC_MS_WH_UP)
        mouse_report.v = w_offset;
    else if (code == KC_MS_WH_DOWN)
        mouse_report.v = w_offset * -1;
    else if (code == KC_MS_WH_LEFT)
        mouse_report.h = w_offset * -1;
    else if (code == KC_MS_WH_RIGHT)
        mouse_report.h = w_offset;
    else if (IS_MOUSEKEY_BUTTON(code))
        mouse_report.buttons |= 1 << (code - KC_MS_BTN1);
    else if (code == KC_MS_ACCEL0)
        mk_speed = mkspd_0;
    else if (code == KC_MS_ACCEL1)
        mk_speed = mkspd_1;
    else if (code == KC_MS_ACCEL2)
        mk_speed = mkspd_2;
    if (mk_speed != old_speed) adjust_speed();
}

void mousekey_off(uint8_t code) {
#    if MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
    uint8_t const old_speed = mk_speed;
#    endif
    if (code == KC_MS_UP && mouse_report.y < 0)
        mouse_report.y = 0;
    else if (code == KC_MS_DOWN && mouse_report.y > 0)
        mouse_report.y = 0;
    else if (code == KC_MS_LEFT && mouse_report.x < 0)
        mouse_report.x = 0;
    else if (code == KC_MS_RIGHT && mouse_report.x > 0)
        mouse_report.x = 0;
    else if (code == KC_MS_WH_UP && mouse_report.v > 0)
        mouse_report.v = 0;
    else if (code == KC_MS_WH_DOWN && mouse_report.v < 0)
        mouse_report.v = 0;
    else if (code == KC_MS_WH_LEFT && mouse_report.h < 0)
        mouse_report.h = 0;
    else if (code == KC_MS_WH_RIGHT && mouse_report.h > 0)
        mouse_report.h = 0;
    else if (IS_MOUSEKEY_BUTTON(code))
        mouse_report.buttons &= ~(1 << (code - KC_MS_BTN1));
#    if MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
    else if (code == KC_MS_ACCEL0)
        mk_speed = mkspd_unmod;
    else if (code == KC_MS_ACCEL1)
        mk_speed = mkspd_unmod;
    else if (code == KC_MS_ACCEL2)
        mk_speed = mkspd_unmod;
    if (mk_speed != old_speed) adjust_speed();
#    endif
}

#endif /* MK_TYPE_3_SPEED */

void mousekey_send(void) {
    mousekey_debug();
    uint16_t time = timer_read();
    if (mouse_report.x || mouse_report.y) last_timer_c = time;
    if (mouse_report.v || mouse_report.h) last_timer_w = time;
    host_mouse_send(&mouse_report);
}

void mousekey_clear(void) {
    NO_FIELD(mouse_report = (report_mouse_t){});
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    mousekey_accel        = 0;
    mousekey_repeat       = 0;
    mousekey_wheel_repeat = 0;
#endif
}

static void mousekey_debug(void) {
    if (!debug_mouse) return;

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    print("mousekey [btn|x y v h](rep/acl): [");
    print_hex8(mouse_report.buttons);
    print("|");
    print_decs(mouse_report.x);
    print(" ");
    print_decs(mouse_report.y);
    print(" ");
    print_decs(mouse_report.v);
    print(" ");
    print_decs(mouse_report.h);
    print("](");
    print_dec(mousekey_repeat);
    print("/");
    print_dec(mousekey_accel);
    print(")\n");
#endif
}

/***********************************************************
 * Mousekey console
 ***********************************************************/
#ifdef COMMAND_ENABLE

#    if !defined(NO_PRINT) && !defined(USER_PRINT)
static void mousekey_param_print(void) {
    xprintf(/* clang-format off */

#if MK_KIND(MK_TYPE_X11)
        "1:	delay(*10ms): %u\n"
        "2:	interval(ms): %u\n"
        "3:	max_speed: %u\n"
        "4:	time_to_max: %u\n"
        "5:	wheel_max_speed: %u\n"
        "6:	wheel_time_to_max: %u\n"

        , mk_delay
        , mk_interval
        , mk_max_speed
        , mk_time_to_max
        , mk_wheel_max_speed
        , mk_wheel_time_to_max
#else
        "no knobs sorry\n"
#endif

    ); /* clang-format on */
}
#    endif /* !NO_PRINT && !USER_PRINT */

#    if !defined(NO_PRINT) && !defined(USER_PRINT)
static void mousekey_console_help(void) {
    mousekey_param_print();
    xprintf(/* clang-format off */
        "p:	print values\n"
        "d:	set defaults\n"
        "up:	+1\n"
        "dn:	-1\n"
        "lt:	+10\n"
        "rt:	-10\n"
        "ESC/q:	quit\n"

#        if MK_KIND(MK_TYPE_X11)
        "\n"
        "speed = delta * max_speed * (repeat / time_to_max)\n"
        "where delta: cursor=%d, wheel=%d\n"
        "See http://en.wikipedia.org/wiki/Mouse_keys\n"
        , MOUSEKEY_MOVE_DELTA, MOUSEKEY_WHEEL_DELTA
#        endif

    ); /* clang-format on */
}
#    endif /* !NO_PRINT && !USER_PRINT */

/* Only used by `quantum/command.c` / `command_proc()`. To avoid
 * any doubt: we return `false` to return to the main console,
 * which differs from the `bool` that `command_proc()` returns. */
bool mousekey_console(uint8_t code) {
    static uint8_t  param = 0;
    static uint8_t *pp    = NULL;
    static char *   desc  = NULL;

#    if defined(NO_PRINT) || defined(USER_PRINT) /* -Wunused-parameter */
    (void)desc;
#    endif

    int8_t change = 0;

    switch (code) {
        case KC_H:
        case KC_SLASH: /* ? */
#    if !defined(NO_PRINT) && !defined(USER_PRINT)
            print("\n\t- Mousekey -\n");
            mousekey_console_help();
#    endif
            break;

        case KC_Q:
        case KC_ESC:
            print("q\n");
            if (!param) return false;
            param = 0;
            pp    = NULL;
            desc  = NULL;
            break;

        case KC_P:
#    if !defined(NO_PRINT) && !defined(USER_PRINT)
            print("\n\t- Values -\n");
            mousekey_param_print();
#    endif
            break;

        case KC_1 ... KC_0: /* KC_0 gives param = 10 */
            param = 1 + code - KC_1;
            switch (param) { /* clang-format off */
#               define PARAM(n, v) case n: pp = &(v); desc = #v; break

#    if MK_KIND(MK_TYPE_X11)
                PARAM(1, mk_delay);
                PARAM(2, mk_interval);
                PARAM(3, mk_max_speed);
                PARAM(4, mk_time_to_max);
                PARAM(5, mk_wheel_max_speed);
                PARAM(6, mk_wheel_time_to_max);
#    endif /* MK_TYPE_X11 */

#               undef PARAM /* clang-format on */
                default:
                    param = 0;
                    print("?\n");
                    break;
            }
            if (param) xprintf("%u\n", param);
            break;

            /* clang-format off */
        case KC_UP:    change =  +1; break;
        case KC_DOWN:  change =  -1; break;
        case KC_LEFT:  change = -10; break;
        case KC_RIGHT: change = +10; break;
            /* clang-format on */

        case KC_D:

#    if MK_KIND(MK_TYPE_X11)
            mk_delay             = MOUSEKEY_DELAY / 10;
            mk_interval          = MOUSEKEY_INTERVAL;
            mk_max_speed         = MOUSEKEY_MAX_SPEED;
            mk_time_to_max       = MOUSEKEY_TIME_TO_MAX;
            mk_wheel_max_speed   = MOUSEKEY_WHEEL_MAX_SPEED;
            mk_wheel_time_to_max = MOUSEKEY_WHEEL_TIME_TO_MAX;
#    endif /* MK_TYPE_X11 */

            print("defaults\n");
            break;

        default:
            print("?\n");
            break;
    }

    if (change) {
        if (pp) {
            int16_t val = *pp + change;
            if (val > (int16_t)UINT8_MAX)
                *pp = UINT8_MAX;
            else if (val < 0)
                *pp = 0;
            else
                *pp = (uint8_t)val;
            xprintf("= %u\n", *pp);
        } else {
            print("?\n");
        }
    }

    if (param) {
        xprintf("M%u:%s> ", param, desc ? desc : "???");
    } else {
        print("M> ");
    }
    return true;
}

#endif /* COMMAND_ENABLE */
