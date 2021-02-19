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
#include <stdlib.h>
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

#elif MK_KIND(MK_TYPE_KINETIC)

#    ifndef MK_KINETIC_MOUSE_ACCN
#        define MK_KINETIC_MOUSE_ACCN 0x18
#    endif
#    ifndef MK_KINETIC_MOUSE_DRAG
#        define MK_KINETIC_MOUSE_DRAG 0x10
#    endif
#    ifndef MK_KINETIC_MOUSE_FRIC
#        define MK_KINETIC_MOUSE_FRIC 0x20
#    endif
#    ifndef MK_KINETIC_MOUSE_MAXS
#        define MK_KINETIC_MOUSE_MAXS 0x80
#    endif

#    ifndef MK_KINETIC_WHEEL_ACCN
#        define MK_KINETIC_WHEEL_ACCN 0x10
#    endif
#    ifndef MK_KINETIC_WHEEL_DRAG
#        define MK_KINETIC_WHEEL_DRAG 0x08
#    endif
#    ifndef MK_KINETIC_WHEEL_FRIC
#        define MK_KINETIC_WHEEL_FRIC 0x08
#    endif
#    ifndef MK_KINETIC_WHEEL_MAXS
#        define MK_KINETIC_WHEEL_MAXS 0x0c
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

#if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_3_SPEED)
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

#elif MK_KIND(MK_TYPE_KINETIC)
static inline uint8_t cardinal(dxdy_t d, uint8_t unit) {
    if (!unit) return 0;
    if (!d.dx && !d.dy) return 0;
    if (!d.dx || !d.dy) return unit;
    uint8_t move = div_sqrt2(unit);
    return move ? move : 1;
}

/* Saturated signed addition ⊆ `[MIN+1, MAX]`. */
static inline int16_t ssadd16(int16_t a, int16_t b) {
    bool    pos = a > 0; /* note: MAX + 2 ≡ MIN + 1 */
    int16_t sat = (int16_t)(INT16_MAX + (pos ? 0 : 2));
    return (pos == (b < sat - a)) ? (int16_t)(a + b) : sat;
}

/* TODO Try these:
 * __builtin_add_overflow from GCC 5.5.0 onwards
 * __builtin_add_overflow_p from GCC 7.1.0 onwards
 */

/* Fixed-point normⁿ: discard `n` bits & round towards ∓, nearest, 0, ∞ */
#    define NORM_NEGA(n, x) ((x) >> (n))
#    define NORM_POSI(n, x) (((x) + ((typeof(x))1 << (n)) - 1) >> (n))
#    define NORM_NEAR(n, x) (((x) + ((typeof(x))1 << ((n)-1))) >> (n))
#    define NORM_ZERO(n, x) NORM_TO(< 0, (n), (x))
#    define NORM_INFI(n, x) NORM_TO(> 0, (n), (x))

#    define NORM_TO(op, n, x)                                 \
        ({                                                    \
            typeof(x) __x__ = (x);                            \
            if (__x__ op) __x__ += ((typeof(x))1 << (n)) - 1; \
            __x__ >> (n);                                     \
        })

#endif

/**********************************************************************/

typedef struct {
    uint16_t last_time;
    /* dxdy is {,re}set by mousekey_o{n,ff}() to {±d,0}—but never updated
     * by mousekey_task()—where `d` is usually 1, representing the resolved
     * direction from the current state of KC_MS_{,WH_}{UP,DOWN,LEFT,RIGHT}.
     *
     * Except MK_TYPE_3_SPEED which uses d = speed_t.offset[speed]. */
    dxdy_t dxdy;
#if MK_KIND(MK_TYPE_X11)
    uint8_t repeat;
#elif MK_KIND(MK_TYPE_KINETIC)
    dxdy_t  rem;
    int16_t v_x, v_y; /* velocity */
#endif
} axes_t;

static axes_t mouse_axes;
static axes_t wheel_axes;

#define MASK_BTN(kc) (uint8_t) MOUSE_BTN_MASK((kc) - (KC_MS_BTN1))
static uint8_t btn_state = 0;

/**********************************************************************/

#if MK_KIND(MK_TYPE_X11)

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

#elif MK_KIND(MK_TYPE_KINETIC)

/*
 * Kinetic movement: cursor follows classical mechanics with
 * fluid (but not viscous) drag and Coulombian kinetic friction.
 *
 * Parameters:
 *
 *  accn: acceleration when `key_down`
 *  drag: drag coefficient
 *  fric: friction coefficient
 *  maxs: max speed in `report_mouse_t` units / 15ms
 *
 * Non-linear ODE (Riccati w/ q₁ = 0) model:
 *
 *  a(t) = fric + (key_down(t) ? diagonal(t) ? accn/√2 : accn : 0)
 *  dv(t)/dt = dir(t) × a(t) - sgn(v(t)) × (fric + drag × v(t)²)
 *
 * where `v(t)` is velocity, with `dir(t)` ∈ {0,±1}, `key_down(t)`, and
 * `diagonal(t)` corresponding to the state of the mousekey arrows.
 *
 * Note the `fric` term in the definition of `a(t)`, such that we
 * are _barely_ able to overcome friction when `accn` is set to 0.
 * This feels like the more intuitive behaviour for the end-user,
 * rather than requiring `accn > fric` for any acceleration at all.
 *
 * For `v(t) > 0`, `a(t) - fric = accn`, and starting with `v(0) = 0`,
 * there is a closed-form solution for `v(t)`:
 *
 *  v(t) = √(accn/drag) × tanh(√(accn × drag) × t)
 *
 * Numerical approximation:
 *
 * Playing fast & loose with the Euler method, we can rewrite
 * the above as a recurrence relation with time step `dt`:
 *
 *  v(t+dt) = v(t) + a(t) × dt
 *      - sgn(v) × (fric + drag × v(t)²) × dt
 *
 * from which we derive the displacement for the current step:
 *
 *  ds(t) = ½(v(t+dt) + v(t)) × dt
 *
 * We scale this by `maxs` to give `report_mouse_t` units.
 *
 * References:
 *
 *  https://en.wikipedia.org/wiki/Drag_equation
 *  https://en.wikipedia.org/wiki/Coulomb_damping
 *  https://en.wikipedia.org/wiki/Ordinary_differential_equation
 *  https://en.wikipedia.org/wiki/Euler_method
 *  https://en.wikipedia.org/wiki/Riccati_equation with q₁ = 0
 *  https://www.wolframalpha.com/ solve dv/dt = a - d × v(t)²; v(0) = 0
 */

typedef struct {
    uint8_t accn; /* L×T⁻² */
    uint8_t drag; /* L⁻¹ */
    uint8_t fric; /* L×T⁻² */
    uint8_t maxs; /* `report_mouse_t` units per 15ms */
} kinetic_t;

#    ifndef MK_KINETIC_MOUSE
#        define MK_KINETIC_MOUSE /* clang-format off */ \
             { .accn = MK_KINETIC_MOUSE_ACCN \
             , .drag = MK_KINETIC_MOUSE_DRAG \
             , .fric = MK_KINETIC_MOUSE_FRIC \
             , .maxs = MK_KINETIC_MOUSE_MAXS \
             } /* clang-format on */
#    endif

#    ifndef MK_KINETIC_WHEEL
#        define MK_KINETIC_WHEEL /* clang-format off */ \
             { .accn = MK_KINETIC_WHEEL_ACCN \
             , .drag = MK_KINETIC_WHEEL_DRAG \
             , .fric = MK_KINETIC_WHEEL_FRIC \
             , .maxs = MK_KINETIC_WHEEL_MAXS \
             } /* clang-format on */
#    endif

static kinetic_t mouse = MK_KINETIC_MOUSE;
static kinetic_t wheel = MK_KINETIC_WHEEL;

#    define MK_KINETIC_EXPS 8
static uint8_t   exps  = MK_KINETIC_EXPS;

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

#endif /* MK_TYPE_X11, MK_TYPE_KINETIC, MK_TYPE_3_SPEED */

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

static dxdy_t x11_step(const x11_t *what, axes_t *axes, uint8_t delta, uint8_t max) {
    if (!axes->dxdy.dx && !axes->dxdy.dy) return dxdy_0;

    uint16_t now = timer_read();
    uint16_t dur = axes->repeat ? what->interval : (uint16_t)((uint16_t)what->delay * 10);
    if (TIMER_DIFF_16(now, axes->last_time) < dur) return dxdy_0;
    axes->last_time = now;

    if (axes->repeat < UINT8_MAX) axes->repeat++;

    uint16_t unit = x11_unit(what, axes->repeat, delta);
    return bishop(axes->dxdy, unit > max ? max : (uint8_t)unit);
}

#elif MK_KIND(MK_TYPE_KINETIC)

#    ifdef __AVR__
/* Ever-so-slightly smaller, faster, and better register allocation. */
typedef __int24  int24_t;
typedef __uint24 uint24_t;
#    else
typedef int32_t  int24_t;
typedef uint32_t uint24_t;
#    endif

dxdy_t kinetic_step(const kinetic_t *what, axes_t *axes) {
    uint16_t now = timer_read();
    uint16_t t_d = TIMER_DIFF_16(now, axes->last_time);
    if (t_d == 0) return dxdy_0;
    axes->last_time = now;

    /* Potential overflow if `dt` > 15ms: we only allow for 4 bits.
     * We expect `dt` to usually be 1–3ms, very rarely longer. */
    uint8_t dt = t_d > 15 ? 15 : (uint8_t)t_d;

    uint16_t maxs = (uint16_t)NORM_POSI(MK_KINETIC_EXPS, (uint24_t)what->maxs << exps);
    if (maxs > UINT8_MAX) maxs = UINT8_MAX;
    int16_t maxs_dt = (int16_t)(maxs * dt); /* 12 bits */

    uint8_t accn = cardinal(axes->dxdy, what->accn);

    int8_t kinetic(int8_t dir, int16_t * pv, int8_t * rem) {
        int16_t v = *pv;

        /* 9 bits; ensure acceleration ≥ friction */
        uint16_t a_fric = (uint16_t)((uint16_t)accn + what->fric);
        /* ±15 bits; dir: 1 bit, dt: 4 bits, a_fric: 9+2=11 bits */
        int16_t a_dt = (int16_t)(dir * (int16_t)(a_fric << 2) * dt);
        /* v(t) + a(t) × dt: ±15 bits; v: ±15 bits, a_dt: ±15 bits */
        int16_t v_a = ssadd16(v, a_dt);

        /* 16 bits post-normⁿ; v_a²: 30 bits */
        uint16_t v2 = (uint16_t)NORM_POSI(14, (int32_t)v_a * v_a);
        /* 15 bits post-normⁿ; what→drag: 8 bits, v2: 16 bits */
        uint16_t drag_v2 = (uint16_t)NORM_POSI(9, what->drag * (uint24_t)v2);
        /* 16 bits; drag_v2: 15 bits, what→fric: 8+6=14 bits */
        uint16_t loss = (uint16_t)(drag_v2 + ((uint16_t)what->fric << 6));

        /* 16 bits post-normⁿ; dt: 4 bits, loss: 16 bits */
        uint16_t loss_dt = (uint16_t)NORM_POSI(4, (uint24_t)loss * dt);

        /*
         * Approximating ODEs with recurrence relations—particularly for
         * our fairly large values of `dt`—can lead to unphysical results.
         * It is not possible for drag & friction _losses_ to cause a rigid
         * body in the real world to reverse direction, but for sufficiently
         * large `dt`, `drag`, and `fric`, our numbers could.
         *
         * Fudge this by limiting `loss_dt`. :)
         */

        /* *pv ≡ v(t+dt): ±15 bits; velocity after all losses */
        *pv = loss_dt > (uint16_t)abs(v_a) ? 0 : (int16_t)(v_a - signum(v_a) * (int16_t)loss_dt);

        /* v_avg ≡ ½(v(t) + v(t+dt)); carefulling because v + *pv might overflow. */
        int16_t v_avg = (int16_t)(v + ((*pv - v) >> 1));

        /* displacement: ±15 bits post-normⁿ; maxs_dt: 12 bits, v_avg: ±15 bits */
        int16_t ds = (int16_t)(*rem + (int16_t)NORM_ZERO(12, (int32_t)maxs_dt * v_avg));

        /* Treat as ±7.8 fixed point: return ±7 now, save .8 for later */
        int8_t d = (int8_t)NORM_NEAR(8, ds);
        /* Reading the .8 as a ±7 gives the desired remainder. Coincidence?
         * No, only because we rounded to nearest. Convenient however. :) */
        *rem = (int8_t)ds; /* ≡ ds & 0xff */

#    ifdef DEBUG_MOUSE
        if (debug_config.enable && (v || dir)) {
            xprintf(/* clang-format off */
                "dt: %u, v: %04X"
                ", a_dt: %04X"
                ", drag_v2: %04X"
/*                ", loss: %04X"*/
                ", loss_dt: %04X"
/*                ", *pv: %04X"*/
/*                ", maxs_dt: %04X"*/
/*                ", ds: %04X"*/
                ", d: %02X, rem: %02X"
                "\n"

                , dt, v
                , a_dt
                , drag_v2
/*                , loss*/
                , loss_dt
/*                , *pv*/
/*                , maxs_dt*/
/*                , ds*/
                , 0xff & d, 0xff & *rem
                ); /* clang-format on */
        }
#    endif

        return d;
    }

    return (dxdy_t) /* clang-format off */
        { .dx = kinetic(axes->dxdy.dx, &axes->v_x, &axes->rem.dx)
        , .dy = kinetic(axes->dxdy.dy, &axes->v_y, &axes->rem.dy)
        }; /* clang-format on */
}

#elif MK_KIND(MK_TYPE_3_SPEED)

static dxdy_t speed_step(axes_t *axes, uint16_t interval) {
    if (!axes->dxdy.dx && !axes->dxdy.dy) return dxdy_0;
    uint16_t now = timer_read();
    if (TIMER_DIFF_16(now, axes->last_time) < interval) return dxdy_0;
    axes->last_time = now;
    return axes->dxdy;
}

static void adjust_speed(void) {
    mouse_axes.dxdy = bishop(mouse_axes.dxdy, mouse.offset[speed]);
    wheel_axes.dxdy = bishop(wheel_axes.dxdy, wheel.offset[speed]);
}

#endif /* MK_TYPE_X11, MK_TYPE_KINETIC, MK_TYPE_3_SPEED */

/**********************************************************************/

#define MD mouse_axes.dxdy
#define WD wheel_axes.dxdy

void mousekey_on(uint8_t code) {
    bool m_off = !MD.dx && !MD.dy;
    bool w_off = !WD.dx && !WD.dy;

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

#if MK_KIND(MK_TYPE_KINETIC)
        case KC_MS_ACCEL0:
            exps = MK_KINETIC_EXPS - 1; /* ½ maxs */
            break;
        case KC_MS_ACCEL1:
            if (exps > 1) exps--;
            break;
        case KC_MS_ACCEL2:
            if (exps < 15) exps++;
            break;
#else /* MK_TYPE_KINETIC */
        case KC_MS_ACCEL0 ... KC_MS_ACCEL2:
#    if MK_KIND(MK_TYPE_X11) || MK_KIND(MK_TYPE_KINETIC)
            accel_state |= MASK_ACCEL(code);
#    elif MK_KIND(MK_TYPE_3_SPEED)
            speed = (uint8_t)(speed_0 + code - KC_MS_ACCEL0);
#    endif
            break;
#endif /* MK_TYPE_KINETIC */
    }

    if (m_off && (MD.dx || MD.dy)) mouse_axes.last_time = timer_read();
    if (w_off && (WD.dx || WD.dy)) wheel_axes.last_time = timer_read();

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

#if MK_KIND(MK_TYPE_KINETIC)
        case KC_MS_ACCEL0:
            exps = MK_KINETIC_EXPS;
            break;
#else /* MK_TYPE_KINETIC */
        case KC_MS_ACCEL0 ... KC_MS_ACCEL2:
#    if MK_KIND(MK_TYPE_X11)
            accel_state &= (uint8_t)~MASK_ACCEL(code);
#    elif MK_TYPE(MK_TYPE_3_SPEED_MOMENTARY)
            speed = speed_unmod;
#    endif
            break;
#endif /* MK_TYPE_KINETIC */
    }

#if MK_KIND(MK_TYPE_X11)
    if (MD.dx == 0 && MD.dy == 0) mouse_axes.repeat = 0;
    if (WD.dx == 0 && WD.dy == 0) wheel_axes.repeat = 0;

#elif MK_KIND(MK_TYPE_3_SPEED)
    adjust_speed(); /* also (un)apply the bishop() adjustment */
#endif
}

#undef MD
#undef WD

/**********************************************************************/

#if MK_KIND(MK_TYPE_3_SPEED) && !defined(NO_PRINT) && !defined(USER_PRINT)

static const char PROGMEM speed_enum_unmod[] = "unmod";
static const char PROGMEM speed_enum_0[]     = "0";
static const char PROGMEM speed_enum_1[]     = "1";
static const char PROGMEM speed_enum_2[]     = "2";

static PGM_P speed_enum[speed_COUNT] = {speed_enum_unmod, speed_enum_0, speed_enum_1, speed_enum_2};

#endif

static void send_dxdy(dxdy_t m, dxdy_t w) {
    if (debug_config.mouse) {
        xprintf(/* clang-format off */
            "mousekey btn: %08b"
            ", mouse: %3d %3d"
            ", wheel: %3d %3d"

#if MK_KIND(MK_TYPE_X11)
            ", repeat m:%2u w:%2u"
            ", accel: %03b"
#elif MK_KIND(MK_TYPE_KINETIC)
            ", rem m: %02X %02X"
                " w: %02X %02X"
            ", vel m: %04X %04X"
                " w: %04X %04X"
            ", exps: %u"
#elif MK_KIND(MK_TYPE_3_SPEED)
            ", speed: %S"
#endif
            "\n"

            , btn_state
            , m.dx, m.dy
            , w.dx, w.dy

#if MK_KIND(MK_TYPE_X11)
            , mouse_axes.repeat, wheel_axes.repeat
            , accel_state
#elif MK_KIND(MK_TYPE_KINETIC)
            , mouse_axes.dxdy.dx, mouse_axes.dxdy.dy
            , wheel_axes.dxdy.dx, wheel_axes.dxdy.dy
            , mouse_axes.v_x, mouse_axes.v_y
            , wheel_axes.v_x, wheel_axes.v_y
            , exps
#elif MK_KIND(MK_TYPE_3_SPEED)
            , speed_enum[speed]
#endif
            ); /* clang-format on */
    }

    report_mouse_t report = /* clang-format off */ {
        .buttons = btn_state,
        .x = m.dx,
        .y = m.dy,
        .h = w.dx,
        .v = w.dy
    }; /* clang-format on */
    host_mouse_send(&report);
}

/*
 * Send btn_state (only) immediately.
 *
 * Call this if you're so excited that you simply cannot wait until
 * the next regularly scheduled mousekey_task() to notify the host.
 *
 * This does not send any mouse/wheel movements.
 */
void mousekey_send(void) { send_dxdy(dxdy_0, dxdy_0); }

/* FIXME: what's the use-case for this? */
void mousekey_clear(void) {
    /* old behaviour was to preserve .last_time */
    btn_state = 0;
#if MK_KIND(MK_TYPE_X11)
    accel_state = 0;
#endif

#if MK_KIND(MK_TYPE_X11)
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
    dxdy_t m = kinetic_step(&mouse, &mouse_axes);
    dxdy_t w = kinetic_step(&wheel, &wheel_axes);

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
static void print_x11_t(uint8_t n, PGM_P name, const x11_t *what) {
    xprintf(/* clang-format off */
        "\t%S\n"
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

#        elif MK_KIND(MK_TYPE_KINETIC)
static void print_kinetic_t(uint8_t n, PGM_P name, const kinetic_t *what) {
    xprintf(/* clang-format off */
        "\t%S\n"
        "%u: .accn: %u\n"
        "%u: .drag: %u\n"
        "%u: .fric: %u\n"
        "%u: .maxs: %u\n"

        , name
        , n + 1, what->accn
        , n + 2, what->drag
        , n + 3, what->fric
        , n + 4, what->maxs

        ); /* clang-format on */
}

#        elif MK_KIND(MK_TYPE_3_SPEED)
static void print_3_speed(uint8_t n, PGM_P name, const uint8_t what[speed_COUNT]) {
    for (int i = 0; i < speed_COUNT; i++) {
        xprintf("%u:	.%S[%S]: %u\n", n + i + 1, name, speed_enum[i], what[i]);
    }
}

#        endif /* MK_TYPE_X11, MK_TYPE_3_SPEED */

static void mousekey_param_print(void) {
#        if MK_KIND(MK_TYPE_X11)
    print_x11_t(0, PSTR("mouse"), &mouse);
    print_x11_t(4, PSTR("wheel"), &wheel);

#        elif MK_KIND(MK_TYPE_KINETIC)
    print_kinetic_t(0, PSTR("mouse"), &mouse);
    print_kinetic_t(4, PSTR("wheel"), &wheel);

#        elif MK_KIND(MK_TYPE_3_SPEED)
    print("\tmouse\n");
    print_3_speed(0, PSTR("interval(*4ms)"), mouse.interval);
    print_3_speed(4, PSTR("offset"), mouse.offset);
    print("\twheel\n");
    print_3_speed(0, PSTR("interval(*4ms)"), wheel.interval);
    print_3_speed(4, PSTR("offset"), wheel.offset);

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

#    elif MK_KIND(MK_TYPE_KINETIC)
#           define PARAM(n, v) case n: pp = &(v); break

            PARAM(1, mouse.accn);
            PARAM(2, mouse.drag);
            PARAM(3, mouse.fric);
            PARAM(4, mouse.maxs);
            PARAM(5, wheel.accn);
            PARAM(6, wheel.drag);
            PARAM(7, wheel.fric);
            PARAM(8, wheel.maxs);

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

#    elif MK_KIND(MK_TYPE_KINETIC)
            mouse = (kinetic_t)MK_KINETIC_MOUSE;
            wheel = (kinetic_t)MK_KINETIC_WHEEL;

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
        uint8_t i = (uint8_t)(param - 1);
#        if MK_KIND(MK_TYPE_X11)
        static const char PROGMEM x11_field_delay[]       = "delay";
        static const char PROGMEM x11_field_interval[]    = "interval";
        static const char PROGMEM x11_field_max_speed[]   = "max_speed";
        static const char PROGMEM x11_field_time_to_max[] = "time_to_max";

        static PGM_P x11_field[4] = /* clang-format off */
            {x11_field_delay, x11_field_interval, x11_field_max_speed, x11_field_time_to_max};
        xprintf(
            "M%u:%S.%S> "
            , param
            , i & 4 ? PSTR("wheel") : PSTR("mouse")
            , x11_field[i & 3]
            ); /* clang-format on */

#        elif MK_KIND(MK_TYPE_KINETIC)
        static const char PROGMEM kinetic_field[4][5] = /* clang-format off */
            {"accn", "drag", "fric", "maxs"};
        xprintf(/* clang-format off */
            "M%u:%S.%S> "
            , param
            , i & 4 ? PSTR("wheel") : PSTR("mouse")
            , kinetic_field[i & 3]
            ); /* clang-format on */

#        elif MK_KIND(MK_TYPE_3_SPEED)
        xprintf(/* clang-format off */
            "M%u:%S.%S[%S]> "
            , param
            , axes ? PSTR("mouse") : PSTR("wheel")
            , i & 4 ? PSTR("offset") : PSTR("interval")
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
