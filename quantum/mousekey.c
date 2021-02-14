/*
 * Copyright 2011 Jun Wako <wakojun@gmail.com>
 * Copyright 2021 Liyang HU <qmk.tk@liyang.hu>
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
#pragma GCC diagnostic error "-Wconversion"

/**********************************************************************/

typedef struct {
    int8_t dx, dy;
} dxdy_t;

static const dxdy_t dxdy_0; /* {}; dodge -Wmissing-field-initializers */

inline static uint8_t div_sqrt2(uint8_t x) {
    // 181/256 is pretty close to 1/sqrt(2)
    // 0.70703125                 0.707106781
    // 1 too small for x=99 and x=198
    // This ends up being a mult and discard lower 8 bits
    return (uint8_t)((x * 181) >> 8);
}

#define signum(x) (typeof(x))(((x) > 0) - ((x) < 0))

static dxdy_t bishop(dxdy_t d, uint8_t unit) {
    uint8_t move = d.dx && d.dy ? div_sqrt2(unit) : unit;

    /* diagonal move [1/sqrt(2)] */
    /* valid only if |dx| ≡ |dy| */
    /* which is true for us here */
    /* ensure we move at least 1 */
    if (move == 0) move = 1;

    return (dxdy_t) /* clang-format off */ {
        .dx = (int8_t)(signum(d.dx) * move),
        .dy = (int8_t)(signum(d.dy) * move)
    }; /* clang-format on */
}

/**********************************************************************/

typedef struct {
    uint16_t last_time;
    /* dxdy is {,re}set by mousekey_o{n,ff}() to {±d,0}—but never updated
     * by mousekey_task()—where `d` is usually 1, representing the resolved
     * direction from the current state of KC_MS_{,WH_}{UP,DOWN,LEFT,RIGHT}.
     *
     * Except MK_TYPE_3_SPEED which uses d = speed_t.offset[speed]. */
    dxdy_t dxdy;
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

#define MASK_BTN(kc) (uint8_t) MOUSE_BTN_MASK((kc) - (KC_MS_BTN1))
static uint8_t btn_state = 0;

/**********************************************************************/

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
/* TODO: untangle MK_TYPE_KINETIC.
 * Currently shares config/state w/ X11 but probably shouldn't. */

#    define MASK_ACCEL(kc) (uint8_t)(1 << ((kc) - (KC_MS_ACCEL0)))
static uint8_t accel_state = 0;

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

#elif MK_KIND(MK_TYPE_3_SPEED)

enum { speed_unmod, speed_0, speed_1, speed_2, speed_COUNT };

#    if MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
static uint8_t speed = speed_unmod;
#    else
static uint8_t speed = speed_1;
#    endif

typedef struct {
    uint8_t interval[speed_COUNT];
    uint8_t offset[speed_COUNT];
} speed_t;

#    define MK_3_FIELD(axes, field, op) /* clang-format off */ \
         { MK_ ## axes ## _ ## field ## _UNMOD op \
         , MK_ ## axes ## _ ## field ## _0 op \
         , MK_ ## axes ## _ ## field ## _1 op \
         , MK_ ## axes ## _ ## field ## _2 op \
         } /* clang-format on */

#    ifndef MK_3_SPEED_MOUSE
#        define MK_3_SPEED_MOUSE /* clang-format off */ { \
             .interval = MK_3_FIELD(C, INTERVAL, >> 2), \
             .offset   = MK_3_FIELD(C, OFFSET, ) \
         } /* clang-format on */
#    endif

#    ifndef MK_3_SPEED_WHEEL
#        define MK_3_SPEED_WHEEL /* clang-format off */ { \
             .interval = MK_3_FIELD(W, INTERVAL, >> 2), \
             .offset   = MK_3_FIELD(W, OFFSET, ) \
         } /* clang-format on */
#    endif

static speed_t mouse = MK_3_SPEED_MOUSE;
static speed_t wheel = MK_3_SPEED_WHEEL;

#endif /* MK_TYPE_X11 || MK_TYPE_KINETIC, MK_TYPE_3_SPEED */

#if MK_KIND(MK_TYPE_X11)

static uint16_t x11_unit(const x11_t *what, uint8_t repeat, uint8_t delta) {
    if (accel_state & MASK_ACCEL(KC_MS_ACCEL0)) {
#    if MK_TYPE(MK_TYPE_X11)
        return (uint16_t)((uint16_t)delta * what->max_speed) >> 2;
#    elif MK_TYPE(MK_TYPE_X11_COMBINED)
        return 1;
#    endif
    } else if (accel_state & MASK_ACCEL(KC_MS_ACCEL1)) {
        return (uint16_t)((uint16_t)delta * what->max_speed) >> 1;
    } else if (accel_state & MASK_ACCEL(KC_MS_ACCEL2)) {
#    if MK_TYPE(MK_TYPE_X11)
        return (uint16_t)((uint16_t)delta * what->max_speed);
#    elif MK_TYPE(MK_TYPE_X11_COMBINED)
        return UINT16_MAX; /* will be clamped */
#    endif
    } else if (repeat == 0) {
        return delta;
    } else if (repeat >= what->time_to_max) {
        return (uint16_t)((uint16_t)delta * what->max_speed);
    } else {
        return (uint16_t)(((uint32_t)delta * what->max_speed * repeat) / what->time_to_max);
    }
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

    if (accel_state & MASK_ACCEL(KC_MS_ACCEL0)) {
        speed = MOUSEKEY_DECELERATED_SPEED;
    } else if (accel_state & MASK_ACCEL(KC_MS_ACCEL2)) {
        speed = MOUSEKEY_ACCELERATED_SPEED;
    } else if (mouse_axes.repeat && mouse_timer) {
        const float time_elapsed = timer_elapsed(mouse_timer) / 50;
        speed                    = MOUSEKEY_INITIAL_SPEED + MOUSEKEY_MOVE_DELTA * time_elapsed + MOUSEKEY_MOVE_DELTA * 0.5 * time_elapsed * time_elapsed;

        speed = speed > MOUSEKEY_BASE_SPEED ? MOUSEKEY_BASE_SPEED : speed;
    }

    /* convert speed to USB mouse speed 1 to 127 */
    speed = (uint8_t)(speed / (1000.0f / mouse.interval));
    speed = speed < 1 ? 1 : speed;

    return speed > MOUSEKEY_MOVE_MAX ? MOUSEKEY_MOVE_MAX : (uint8_t)speed;
}

/* XXX: was redefⁿ of MK_TYPE_X11's mk_wheel_interval as float */
static float kinetic_wheel_interval = 1000.0f / MOUSEKEY_WHEEL_INITIAL_MOVEMENTS;

static uint8_t wheel_unit(void) {
    float speed = MOUSEKEY_WHEEL_INITIAL_MOVEMENTS;

    if (accel_state & MASK_ACCEL(KC_MS_ACCEL0)) {
        speed = MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS;
    } else if (accel_state & MASK_ACCEL(KC_MS_ACCEL2)) {
        speed = MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS;
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

#if MK_KIND(MK_TYPE_X11)

static dxdy_t x11_step(const x11_t *what, axes_t *axes, uint8_t delta, uint8_t max) {
    if (!axes->dxdy.dx && !axes->dxdy.dy) return dxdy_0;

    uint16_t dur = axes->repeat ? what->interval : (uint16_t)((uint16_t)what->delay * 10);
    if (timer_elapsed(axes->last_time) < dur) return dxdy_0;

    if (axes->repeat < UINT8_MAX) axes->repeat++;

    uint16_t unit = x11_unit(what, axes->repeat, delta);
    return bishop(axes->dxdy, unit > max ? max : (uint8_t)unit);
}

#elif MK_KIND(MK_TYPE_KINETIC)

static bool kinetic_step(const x11_t* what, axes_t* axes) {
    if (!axes->dxdy.dx && !axes->dxdy.dy) return false;

    uint16_t dur = axes->repeat ? what->interval : (uint16_t)what->delay * 10;
    if (timer_elapsed(axes->last_time) < dur) return false;

    if (axes->repeat < UINT8_MAX) axes->repeat++;
    return true;
}

#elif MK_KIND(MK_TYPE_3_SPEED)

static dxdy_t speed_step(const axes_t* axes, uint16_t interval) {
    if (!axes->dxdy.dx && !axes->dxdy.dy) return dxdy_0;
    if (timer_elapsed(axes->last_time) < interval) return dxdy_0;
    return axes->dxdy;
}

static void adjust_speed(void) {
    mouse_axes.dxdy = bishop(mouse_axes.dxdy, mouse.offset[speed]);
    wheel_axes.dxdy = bishop(wheel_axes.dxdy, wheel.offset[speed]);
}

#endif /* MK_KIND(MK_TYPE_X11 || MK_TYPE_KINETIC || MK_TYPE_3_SPEED) */

/**********************************************************************/

#define MD mouse_axes.dxdy
#define WD wheel_axes.dxdy

void mousekey_on(uint8_t code) {
#if MK_KIND(MK_TYPE_KINETIC)
    if (mouse_timer == 0) mouse_timer = timer_read();
#endif

    switch (code) {
            /* clang-format off */
        case KC_MS_UP:       MD.dy = -1; break;
        case KC_MS_DOWN:     MD.dy = +1; break;
        case KC_MS_LEFT:     MD.dx = -1; break;
        case KC_MS_RIGHT:    MD.dx = +1; break;

        case KC_MS_WH_UP:    WD.dy = +1; break;
        case KC_MS_WH_DOWN:  WD.dy = -1; break;
        case KC_MS_WH_LEFT:  WD.dx = -1; break;
        case KC_MS_WH_RIGHT: WD.dx = +1; break;
            /* clang-format on */

        case KC_MS_BTN1 ... KC_MS_BTN8: /* Cf. IS_MOUSEKEY_BUTTON() */
            btn_state |= MASK_BTN(code);
            break;

        case KC_MS_ACCEL0 ... KC_MS_ACCEL2:
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
            accel_state |= MASK_ACCEL(code);
#elif MK_KIND(MK_TYPE_3_SPEED)
            speed = (uint8_t)(speed_0 + code - KC_MS_ACCEL0);
#endif
            break;
    }

#if MK_KIND(MK_TYPE_3_SPEED)
    adjust_speed();
#endif
}

void mousekey_off(uint8_t code) {
    switch (code) {
            /* clang-format off */
        case KC_MS_UP:       if (MD.dy < 0) MD.dy = 0; break;
        case KC_MS_DOWN:     if (MD.dy > 0) MD.dy = 0; break;
        case KC_MS_LEFT:     if (MD.dx < 0) MD.dx = 0; break;
        case KC_MS_RIGHT:    if (MD.dx > 0) MD.dx = 0; break;

        case KC_MS_WH_UP:    if (WD.dy > 0) WD.dy = 0; break;
        case KC_MS_WH_DOWN:  if (WD.dy < 0) WD.dy = 0; break;
        case KC_MS_WH_LEFT:  if (WD.dx < 0) WD.dx = 0; break;
        case KC_MS_WH_RIGHT: if (WD.dx > 0) WD.dx = 0; break;
            /* clang-format on */

        case KC_MS_BTN1 ... KC_MS_BTN8: /* Cf. IS_MOUSEKEY_BUTTON() */
            btn_state &= (uint8_t)~MASK_BTN(code);
            break;

        case KC_MS_ACCEL0 ... KC_MS_ACCEL2:
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
            accel_state &= (uint8_t)~MASK_ACCEL(code);
#elif MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
            speed = speed_unmod;
#endif
            break;
    }

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    if (MD.dx == 0 && MD.dy == 0) {
        mouse_axes.repeat = 0;
#    if MK_KIND(MK_TYPE_KINETIC)
        mouse_timer = 0;
#    endif
    }
    if (WD.dx == 0 && WD.dy == 0) {
        wheel_axes.repeat = 0;
    }

#elif MK_KIND(MK_TYPE_3_SPEED)
    adjust_speed(); /* also (un)apply the bishop() adjustment */
#endif
}

#undef MD
#undef WD

/**********************************************************************/

#if MK_KIND(MK_TYPE_3_SPEED) && !defined(NO_PRINT) && !defined(USER_PRINT)
static const char *speed_enum[speed_COUNT] = {"unmod", "0", "1", "2"};
#endif

static void send_dxdy(dxdy_t m, dxdy_t w) {
    if (debug_config.mouse) {
        xprintf(/* clang-format off */
            "mousekey btn: %08b"
            ", mouse: %3d %3d"
            ", wheel: %3d %3d"

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
            ", repeat m:%2u w:%2u"
            ", accel: %03b"
#elif MK_KIND(MK_TYPE_3_SPEED)
            ", speed: %s"
#endif
            "\n"

            , btn_state
            , m.dx, m.dy
            , w.dx, w.dy

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
            , mouse_axes.repeat, wheel_axes.repeat
            , accel_state
#elif MK_KIND(MK_TYPE_3_SPEED)
            , speed_enum[speed]
#endif
            ); /* clang-format on */
    }

    uint16_t time = timer_read();
    if (m.dx || m.dy) mouse_axes.last_time = time;
    if (w.dx || w.dy) wheel_axes.last_time = time;

    report_mouse_t report = /* clang-format off */ {
        .buttons = btn_state,
        .x = m.dx,
        .y = m.dy,
        .h = w.dx,
        .v = w.dy
    }; /* clang-format on */
    host_mouse_send(&report);
}

void mousekey_send(void) {
    /* XXX: sends `report_mouse_t` immediately and
     * ignores movement algorithm's delay/interval &c.
     * Arguably the wrong external API. Deprecate w/ `mousekey_task()`?
     *
     * Seems to be used to send `btn_state` updates only?
     * In which case we should `send_dxdy(dxdy_0, dxdy_0)`.
     *
     * Here with the old behaviour in any case. */
    send_dxdy(mouse_axes.dxdy, wheel_axes.dxdy);
}

/* FIXME: what's the use-case for this? */
void mousekey_clear(void) {
    /* old behaviour was to preserve .last_time */
    btn_state = 0;
#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
    accel_state = 0;

    mouse_axes.repeat = 0;
    wheel_axes.repeat = 0;
#endif
    mouse_axes.dxdy = dxdy_0;
    wheel_axes.dxdy = dxdy_0;
}

/**********************************************************************/

/* Called regularly from keyboard_task(): compute mouse/wheel displacement,
 * speed, acceleration, &c. here, and host_mouse_send() any movements. */
void mousekey_task(void) {
#if MK_KIND(MK_TYPE_X11)
    dxdy_t m = x11_step(&mouse, &mouse_axes, MOUSEKEY_MOVE_DELTA, MOUSEKEY_MOVE_MAX);
    dxdy_t w = x11_step(&wheel, &wheel_axes, MOUSEKEY_WHEEL_DELTA, MOUSEKEY_WHEEL_MAX);

#elif MK_KIND(MK_TYPE_KINETIC)
    /* TODO: clean up {move,wheel}_unit() & fold into kinetic_step() */
    dxdy_t m = kinetic_step(&mouse, &mouse_axes) ? bishop(mouse_axes.dxdy, move_unit()) : dxdy_0;
    /* XXX previously we just redefined wheel.interval as a float */
    wheel.interval = (uint8_t)kinetic_wheel_interval;
    dxdy_t w = kinetic_step(&wheel, &wheel_axes) ? bishop(wheel_axes.dxdy, wheel_unit()) : dxdy_0;

#elif MK_KIND(MK_TYPE_3_SPEED)
    dxdy_t m = speed_step(&mouse_axes, (uint16_t)((uint16_t)mouse.interval[speed] << 2));
    dxdy_t w = speed_step(&wheel_axes, (uint16_t)((uint16_t)wheel.interval[speed] << 2));

#endif
    if (m.dx || m.dy || w.dx || w.dy) send_dxdy(m, w);
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

#        elif MK_KIND(MK_TYPE_3_SPEED)
static void print_3_speed(uint8_t n, char *name, const uint8_t what[speed_COUNT]) {
    for (int i = 0; i < speed_COUNT; i++) {
        xprintf("%u:	.%s[%s]: %u\n", n + i + 1, name, speed_enum[i], what[i]);
    }
}

#        endif /* MK_TYPE_X11, MK_TYPE_3_SPEED */

static void mousekey_param_print(void) {
#        if MK_KIND(MK_TYPE_X11)
    print_x11_t(0, "mouse", &mouse);
    print_x11_t(4, "wheel", &wheel);

#        elif MK_KIND(MK_TYPE_3_SPEED)
    print("\tmouse\n");
    print_3_speed(0, "interval(*4ms)", mouse.interval);
    print_3_speed(4, "offset", mouse.offset);
    print("\twheel\n");
    print_3_speed(0, "interval(*4ms)", wheel.interval);
    print_3_speed(4, "offset", wheel.offset);

#        else
    print("no knobs sorry\n");
#        endif
}

static void mousekey_console_help(void) {
    mousekey_param_print();
    xprintf(/* clang-format off */
        "p:	print values\n"
        "d:	set defaults\n"
#        if MK_KIND(MK_TYPE_3_SPEED)
        "x:	toggle mouse/wheel axes\n"
#        endif
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

#    if MK_KIND(MK_TYPE_3_SPEED)
    static bool axes = true;
#    endif

    bool switch_param(void) {
#    if MK_KIND(MK_TYPE_3_SPEED)
        pp = (void *)(axes ? &mouse : &wheel);
#    endif

        switch (param) {
#    if MK_KIND(MK_TYPE_X11) /* clang-format off */
#           define PARAM(n, v) case n: pp = &(v); break

            PARAM(1, mouse.delay);
            PARAM(2, mouse.interval);
            PARAM(3, mouse.max_speed);
            PARAM(4, mouse.time_to_max);
            PARAM(5, wheel.delay);
            PARAM(6, wheel.interval);
            PARAM(7, wheel.max_speed);
            PARAM(8, wheel.time_to_max);

#    elif MK_KIND(MK_TYPE_3_SPEED)
#           define PARAM(n, v) case n: pp += offsetof(speed_t, v); break

            PARAM(1, interval[speed_unmod]);
            PARAM(2, interval[speed_0]);
            PARAM(3, interval[speed_1]);
            PARAM(4, interval[speed_2]);
            PARAM(5, offset[speed_unmod]);
            PARAM(6, offset[speed_0]);
            PARAM(7, offset[speed_1]);
            PARAM(8, offset[speed_2]);

#    endif /* MK_TYPE_X11, MK_TYPE_3_SPEED */
#           undef PARAM /* clang-format on */

            default:
                param = 0;
#    if MK_KIND(MK_TYPE_3_SPEED)
                pp = NULL;
#    endif
                print("?\n");
                return false;
        }
        return true;
    }

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
            break;

        case KC_P:
#    if !defined(NO_PRINT) && !defined(USER_PRINT)
            print("\n\t- Values -\n");
            mousekey_param_print();
#    endif
            break;

#    if MK_KIND(MK_TYPE_3_SPEED)
        case KC_X:
            axes = !axes;
            if (switch_param()) {
                print("x\n");
            }
            break;

#    endif

        case KC_1 ... KC_0: /* KC_0 gives param = 10 */
            param = (uint8_t)(1 + code - KC_1);
            if (switch_param()) {
                xprintf("%u\n", param);
            }
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

#    elif MK_KIND(MK_TYPE_3_SPEED)
            mouse = (speed_t)MK_3_SPEED_MOUSE;
            wheel = (speed_t)MK_3_SPEED_WHEEL;

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

#    if !defined(NO_PRINT) && !defined(USER_PRINT)
    if (param) {
        int i = param - 1;
#        if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
        static const char *x11_field[4] = /* clang-format off */
            {"delay", "interval", "max_speed", "time_to_max"};
        xprintf(
            "M%u:%s.%s> "
            , param
            , i & 4 ? "wheel" : "mouse"
            , x11_field[i & 3]
            ); /* clang-format on */
#        elif MK_KIND(MK_TYPE_3_SPEED)
        xprintf(/* clang-format off */
            "M%u:%s.%s[%s]> "
            , param
            , axes ? "mouse" : "wheel"
            , i & 4 ? "offset" : "interval"
            , speed_enum[i & 3]
            ); /* clang-format on */
#        endif
    } else {
        print("M> ");
    }
#    endif /* !NO_PRINT && !USER_PRINT */

    return true;
}

#endif /* COMMAND_ENABLE */
