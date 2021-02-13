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

typedef struct {
    uint16_t last_time;
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    uint8_t repeat;
#endif
} axes_t;

static axes_t mouse_axes;
static axes_t wheel_axes;

#if MK_KIND(MK_TYPE_KINETIC)
/* XXX: overflows if key held down > 65s. */
static uint16_t mouse_timer = 0;
#endif

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
/* TODO: untangle MK_TYPE_KINETIC.
 * Currently shares config/state w/ X11 but probably shouldn't. */

/* KC_MS_ACCEL{0,1,2} state bitflag */
static uint8_t mousekey_accel = 0;

/*
 * Mouse keys acceleration algorithm
 *  http://en.wikipedia.org/wiki/Mouse_keys
 *
 *  speed = delta * max_speed * (repeat / time_to_max)**((1000+curve)/1000)
 */
typedef struct {
    /* milliseconds between the initial key press and first repeated motion event (0-2550) */
    uint8_t delay;
    /* milliseconds between repeated motion events (0-255) */
    uint8_t interval;
    /* steady speed (in action_delta units) applied each event (0-255) */
    uint8_t max_speed;
    /* number of events (count) accelerating to steady speed (0-255) */
    uint8_t time_to_max;
    /* ramp used to reach maximum pointer speed */
    /* XXX: NOT IMPLEMENTED */
    /* int8_t curve = 0; */
} x11_t;

#    ifndef MK_X11_MOUSE
#        define MK_X11_MOUSE /* clang-format off */ { \
             .delay       = MOUSEKEY_DELAY / 10, \
             .interval    = MOUSEKEY_INTERVAL, \
             .max_speed   = MOUSEKEY_MAX_SPEED, \
             .time_to_max = MOUSEKEY_TIME_TO_MAX \
         } /* clang-format on */
#    endif

#    ifndef MK_X11_WHEEL
#        define MK_X11_WHEEL /* clang-format off */ { \
             .delay       = MOUSEKEY_WHEEL_DELAY / 10, \
             .interval    = MOUSEKEY_WHEEL_INTERVAL, \
             .max_speed   = MOUSEKEY_WHEEL_MAX_SPEED, \
             .time_to_max = MOUSEKEY_WHEEL_TIME_TO_MAX \
         } /* clang-format on */
#    endif

static x11_t mouse = MK_X11_MOUSE;
static x11_t wheel = MK_X11_WHEEL;

#endif /* MK_TYPE_X11 || MK_TYPE_KINETIC */

#if MK_KIND(MK_TYPE_X11)

static uint16_t x11_unit(const x11_t *what, uint8_t repeat, uint8_t delta) {
    if (mousekey_accel & (1 << 0)) {
#    if MK_TYPE(MK_TYPE_X11)
        return (delta * what->max_speed) / 4;
#    elif MK_TYPE(MK_TYPE_X11_COMBINED)
        return 1;
#    endif
    } else if (mousekey_accel & (1 << 1)) {
        return (delta * what->max_speed) / 2;
    } else if (mousekey_accel & (1 << 2)) {
#    if MK_TYPE(MK_TYPE_X11)
        return (delta * what->max_speed);
#    elif MK_TYPE(MK_TYPE_X11_COMBINED)
        return UINT16_MAX; /* will be clamp()'d */
#    endif
    } else if (repeat == 0) {
        return delta;
    } else if (repeat >= what->time_to_max) {
        return delta * what->max_speed;
    } else {
        return (delta * what->max_speed * repeat) / what->time_to_max;
    }
}

static inline uint8_t clamp(uint8_t n, uint8_t min, uint8_t max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

#    define move_unit() clamp(x11_unit(&mouse, mouse_axes.repeat, MOUSEKEY_MOVE_DELTA), 1, MOUSEKEY_MOVE_MAX)
#    define wheel_unit() clamp(x11_unit(&wheel, wheel_axes.repeat, MOUSEKEY_WHEEL_DELTA), 1, MOUSEKEY_WHEEL_MAX)

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
    } else if (mouse_axes.repeat && mouse_timer) {
        const float time_elapsed = timer_elapsed(mouse_timer) / 50;
        speed                    = MOUSEKEY_INITIAL_SPEED + MOUSEKEY_MOVE_DELTA * time_elapsed + MOUSEKEY_MOVE_DELTA * 0.5 * time_elapsed * time_elapsed;

        speed = speed > MOUSEKEY_BASE_SPEED ? MOUSEKEY_BASE_SPEED : speed;
    }

    /* convert speed to USB mouse speed 1 to 127 */
    speed = (uint8_t)(speed / (1000.0f / mouse.interval));
    speed = speed < 1 ? 1 : speed;

    return speed > MOUSEKEY_MOVE_MAX ? MOUSEKEY_MOVE_MAX : speed;
}

/* XXX: was redefⁿ of MK_TYPE_X11's mk_wheel_interval as float */
static float kinetic_wheel_interval = 1000.0f / MOUSEKEY_WHEEL_INITIAL_MOVEMENTS;

static uint8_t wheel_unit(void) {
    float speed = MOUSEKEY_WHEEL_INITIAL_MOVEMENTS;

    if (mousekey_accel & ((1 << 0) | (1 << 2))) {
        speed = mousekey_accel & (1 << 2) ? MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS : MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS;
    } else if (wheel_axes.repeat /* XXX: was mouse_axes.repeat */ && mouse_timer) {
        if (kinetic_wheel_interval != MOUSEKEY_WHEEL_BASE_MOVEMENTS) {
            const float time_elapsed = timer_elapsed(mouse_timer) / 50;
            speed                    = MOUSEKEY_WHEEL_INITIAL_MOVEMENTS + 1 * time_elapsed + 1 * 0.5 * time_elapsed * time_elapsed;
        }
        speed = speed > MOUSEKEY_WHEEL_BASE_MOVEMENTS ? MOUSEKEY_WHEEL_BASE_MOVEMENTS : speed;
    }

    kinetic_wheel_interval = 1000.0f / speed;

    return 1; /* XXX: wat. */
}

#endif /* MK_KIND(MK_TYPE_X11 || MK_TYPE_KINETIC) */

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)

void mousekey_task(void) {
    // report cursor and scroll movement independently
    report_mouse_t const tmpmr = mouse_report;

    mouse_report.x = 0;
    mouse_report.y = 0;
    mouse_report.v = 0;
    mouse_report.h = 0;

    if ((tmpmr.x || tmpmr.y) && timer_elapsed(mouse_axes.last_time) > (mouse_axes.repeat ? mouse.interval : mouse.delay * 10)) {
        if (mouse_axes.repeat != UINT8_MAX) mouse_axes.repeat++;
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
    /* XXX previously we just redefined wheel.interval as a float */
    wheel.interval = (uint8_t)kinetic_wheel_interval;
#endif
    if ((tmpmr.v || tmpmr.h) && timer_elapsed(wheel_axes.last_time) > (wheel_axes.repeat ? wheel.interval : wheel.delay * 10)) {
        if (wheel_axes.repeat != UINT8_MAX) wheel_axes.repeat++;
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

    if ((tmpmr.x || tmpmr.y) && timer_elapsed(mouse_axes.last_time) > c_intervals[mk_speed]) {
        mouse_report.x = tmpmr.x;
        mouse_report.y = tmpmr.y;
    }
    if ((tmpmr.h || tmpmr.v) && timer_elapsed(wheel_axes.last_time) > w_intervals[mk_speed]) {
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

#endif /* MK_TYPE_3_SPEED */

#define MR mouse_report

void mousekey_on(uint8_t code) {
#if MK_KIND(MK_TYPE_3_SPEED)
    uint8_t const old_speed = mk_speed;
#elif MK_KIND(MK_TYPE_KINETIC)
    if (mouse_timer == 0) mouse_timer = timer_read();
#endif

    switch (code) {
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
#    define MU move_unit()
#    define WU wheel_unit()
#elif MK_KIND(MK_TYPE_3_SPEED)
#    define MU c_offsets[mk_speed]
#    define WU w_offsets[mk_speed]
#endif

            /* clang-format off */
        case KC_MS_UP:       MR.y = -MU; break;
        case KC_MS_DOWN:     MR.y = +MU; break;
        case KC_MS_LEFT:     MR.x = -MU; break;
        case KC_MS_RIGHT:    MR.x = +MU; break;

        case KC_MS_WH_UP:    MR.v = +WU; break;
        case KC_MS_WH_DOWN:  MR.v = -WU; break;
        case KC_MS_WH_LEFT:  MR.h = -WU; break;
        case KC_MS_WH_RIGHT: MR.h = +WU; break;
            /* clang-format on */

#undef MU
#undef WU

        case KC_MS_BTN1 ... KC_MS_BTN8: /* Cf. IS_MOUSEKEY_BUTTON() */
            mouse_report.buttons |= 1 << (code - KC_MS_BTN1);
            break;

            /* clang-format off */
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
        case KC_MS_ACCEL0: mousekey_accel |=  (1 << 0); break;
        case KC_MS_ACCEL1: mousekey_accel |=  (1 << 1); break;
        case KC_MS_ACCEL2: mousekey_accel |=  (1 << 2); break;
#elif MK_KIND(MK_TYPE_3_SPEED)
        case KC_MS_ACCEL0: mk_speed = mkspd_0; break;
        case KC_MS_ACCEL1: mk_speed = mkspd_1; break;
        case KC_MS_ACCEL2: mk_speed = mkspd_2; break;
#endif
            /* clang-format on */
    }

#if MK_KIND(MK_TYPE_3_SPEED)
    if (mk_speed != old_speed) adjust_speed();
#endif
}

void mousekey_off(uint8_t code) {
#if MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
    uint8_t const old_speed = mk_speed;
#endif

    switch (code) {
            /* clang-format off */
        case KC_MS_UP:       if (MR.y < 0) MR.y = 0; break;
        case KC_MS_DOWN:     if (MR.y > 0) MR.y = 0; break;
        case KC_MS_LEFT:     if (MR.x < 0) MR.x = 0; break;
        case KC_MS_RIGHT:    if (MR.x > 0) MR.x = 0; break;

        case KC_MS_WH_UP:    if (MR.v > 0) MR.v = 0; break;
        case KC_MS_WH_DOWN:  if (MR.v < 0) MR.v = 0; break;
        case KC_MS_WH_LEFT:  if (MR.h < 0) MR.h = 0; break;
        case KC_MS_WH_RIGHT: if (MR.h > 0) MR.h = 0; break;
            /* clang-format on */

        case KC_MS_BTN1 ... KC_MS_BTN8: /* Cf. IS_MOUSEKEY_BUTTON() */
            mouse_report.buttons &= ~(1 << (code - KC_MS_BTN1));
            break;

            /* clang-format off */
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
        case KC_MS_ACCEL0: mousekey_accel &= ~(1 << 0); break;
        case KC_MS_ACCEL1: mousekey_accel &= ~(1 << 1); break;
        case KC_MS_ACCEL2: mousekey_accel &= ~(1 << 2); break;
#elif MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
        case KC_MS_ACCEL0 ... KC_MS_ACCEL2: mk_speed = mkspd_unmod; break;
#endif
            /* clang-format on */
    }

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    if (MR.x == 0 && MR.y == 0) {
        mouse_axes.repeat = 0;
#    if MK_KIND(MK_TYPE_KINETIC)
        mouse_timer = 0;
#    endif
    }
    if (MR.h == 0 && MR.v == 0) {
        wheel_axes.repeat = 0;
    }

#elif MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
    if (mk_speed != old_speed) adjust_speed();

#endif
}

#undef MR

void mousekey_send(void) {
    mousekey_debug();
    uint16_t time = timer_read();
    if (mouse_report.x || mouse_report.y) mouse_axes.last_time = time;
    if (mouse_report.v || mouse_report.h) wheel_axes.last_time = time;
    host_mouse_send(&mouse_report);
}

/* FIXME: what's the use-case for this? */
void mousekey_clear(void) {
    /* old behaviour was to preserve .last_time */
    NO_FIELD(mouse_report = (report_mouse_t){});
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    mousekey_accel = 0;

    mouse_axes.repeat = 0;
    wheel_axes.repeat = 0;
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
    print_dec(mouse_axes.repeat);
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

#        if MK_KIND(MK_TYPE_X11)
static void print_x11_t(uint8_t n, const char *name, const x11_t *what) {
    xprintf(/* clang-format off */
        "\t%s\n"
        "%u: .delay(*10ms): %u\n"
        "%u: .interval(ms): %u\n"
        "%u: .max_speed: %u\n"
        "%u: .time_to_max: %u\n"

        , name
        , n + 1, what->delay
        , n + 2, what->interval
        , n + 3, what->max_speed
        , n + 4, what->time_to_max

        ); /* clang-format on */
}
#        endif /* MK_TYPE_X11 */

static void mousekey_param_print(void) {
#        if MK_KIND(MK_TYPE_X11)
    print_x11_t(0, "mouse", &mouse);
    print_x11_t(4, "wheel", &wheel);

#        else
    print("no knobs sorry\n");
#        endif
}

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
                PARAM(1, mouse.delay);
                PARAM(2, mouse.interval);
                PARAM(3, mouse.max_speed);
                PARAM(4, mouse.time_to_max);
                PARAM(5, wheel.delay);
                PARAM(6, wheel.interval);
                PARAM(7, wheel.max_speed);
                PARAM(8, wheel.time_to_max);
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
            mouse = (x11_t)MK_X11_MOUSE;
            wheel = (x11_t)MK_X11_WHEEL;
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
