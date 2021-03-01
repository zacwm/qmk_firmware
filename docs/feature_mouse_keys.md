# Mouse keys

Mouse keys is a feature that allows you to emulate a mouse using your keyboard. You can move the pointer at different speeds, press 8 buttons and scroll in 8 directions.

## Adding mouse keys to your keyboard

To use mouse keys, you must at least enable mouse keys support and map mouse actions to keys on your keyboard.

### Enabling mouse keys

To enable mouse keys, add the following line to your keymap’s `rules.mk`:

```c
MOUSEKEY_ENABLE = yes
```

### Mapping mouse actions

In your keymap you can use the following keycodes to map key presses to mouse actions:

|Key             |Aliases  |Description      |
|----------------|---------|-----------------|
|`KC_MS_UP`      |`KC_MS_U`|Move cursor up   |
|`KC_MS_DOWN`    |`KC_MS_D`|Move cursor down |
|`KC_MS_LEFT`    |`KC_MS_L`|Move cursor left |
|`KC_MS_RIGHT`   |`KC_MS_R`|Move cursor right|
|`KC_MS_BTN1`    |`KC_BTN1`|Press button 1   |
|`KC_MS_BTN2`    |`KC_BTN2`|Press button 2   |
|`KC_MS_BTN3`    |`KC_BTN3`|Press button 3   |
|`KC_MS_BTN4`    |`KC_BTN4`|Press button 4   |
|`KC_MS_BTN5`    |`KC_BTN5`|Press button 5   |
|`KC_MS_BTN6`    |`KC_BTN6`|Press button 6   |
|`KC_MS_BTN7`    |`KC_BTN7`|Press button 7   |
|`KC_MS_BTN8`    |`KC_BTN8`|Press button 8   |
|`KC_MS_WH_UP`   |`KC_WH_U`|Move wheel up    |
|`KC_MS_WH_DOWN` |`KC_WH_D`|Move wheel down  |
|`KC_MS_WH_LEFT` |`KC_WH_L`|Move wheel left  |
|`KC_MS_WH_RIGHT`|`KC_WH_R`|Move wheel right |
|`KC_MS_ACCEL0`  |`KC_ACL0`|Set speed to 0   |
|`KC_MS_ACCEL1`  |`KC_ACL1`|Set speed to 1   |
|`KC_MS_ACCEL2`  |`KC_ACL2`|Set speed to 2   |

## Configuring mouse keys

Mouse keys supports several movement modes:

* **Accelerated `MK_TYPE_X11` (default):** Holding movement keys accelerates the cursor linearly until it reaches its maximum speed. Additionally holding down acceleration keys immediately moves the cursor at quarter, half, or full speed.
* **Combined `MK_TYPE_X11_COMBINED`:** As above, but acceleration keys select between the minimum, half, or the maximum possible (≥ full) speed.
* **Kinetic:** Holding movement keys accelerates the cursor with its speed following a quadratic curve until it reaches its maximum speed.
* **Constant `MK_TYPE_3_SPEED`:** Holding movement keys moves the cursor at a constant speed. Acceleration keys select between different predetermined speeds.
* **Momentary `MK_TYPE_3_SPEED_MOMENTARY`:** As above, but acceleration keys act only while held down.

The same principle applies to the scroll wheel.

Configuration options that are times, intervals or delays are given in milliseconds. Scroll speed is given as multiples of the default scroll step. For example, a scroll speed of 8 means that each scroll action covers 8 times the length of the default scroll step as defined by your operating system or application.

To select all but the default mode, you _must_ define `MK_VARIANT` accordingly in your keymap’s `config.h` file:

```c
#define MK_VARIANT MK_TYPE_X11
#define MK_VARIANT MK_TYPE_X11_COMBINED
#define MK_VARIANT MK_TYPE_KINETIC
#define MK_VARIANT MK_TYPE_3_SPEED
#define MK_VARIANT MK_TYPE_3_SPEED_MOMENTARY
```

### Accelerated & combined modes: `MK_TYPE_X11` & `MK_TYPE_X11_COMBINED`

This is the default mode when `MK_VARIANT` is not defined. You can adjust the cursor and scrolling acceleration using the following settings in your keymap’s `config.h` file:

|Define                      |Default|Description                                              |
|:---------------------------|------:|:--------------------------------------------------------|
|`MOUSEKEY_DELAY`            |    300|Delay between pressing a movement key and cursor movement|
|`MOUSEKEY_INTERVAL`         |     50|Time between cursor movements in milliseconds            |
|`MOUSEKEY_MOVE_DELTA`       |      5|Step size                                                |
|`MOUSEKEY_MAX_SPEED`        |     10|Maximum cursor speed at which acceleration stops         |
|`MOUSEKEY_TIME_TO_MAX`      |     20|Time until maximum cursor speed is reached               |
|`MOUSEKEY_WHEEL_DELAY`      |    300|Delay between pressing a wheel key and wheel movement    |
|`MOUSEKEY_WHEEL_INTERVAL`   |    100|Time between wheel movements                             |
|`MOUSEKEY_WHEEL_MAX_SPEED`  |      8|Maximum number of scroll steps per scroll action         |
|`MOUSEKEY_WHEEL_TIME_TO_MAX`|     40|Time until maximum scroll speed is reached               |

Tips:

* Setting `MOUSEKEY_DELAY` too low makes the cursor unresponsive. Setting it too high makes small movements difficult.
* For smoother cursor movements, lower the value of `MOUSEKEY_INTERVAL`. If the refresh rate of your display is 60Hz, you could set it to `16` (1/60). As this raises the cursor speed significantly, you may want to lower `MOUSEKEY_MAX_SPEED`.
* Setting `MOUSEKEY_TIME_TO_MAX` or `MOUSEKEY_WHEEL_TIME_TO_MAX` to `0` will disable acceleration for the cursor or scrolling respectively. This way you can make one of them constant while keeping the other accelerated, which is not possible in constant speed mode.
* Setting `MOUSEKEY_WHEEL_INTERVAL` too low will make scrolling too fast. Setting it too high will make scrolling too slow when the wheel key is held down.

Cursor acceleration uses the same algorithm as the X11 Window System MouseKeysAccel feature, although the ‘`curve`’ parameter has not been implemented yet. You can read more about it [on Wikipedia](https://en.wikipedia.org/wiki/Mouse_keys).

#### Combined mode `MK_TYPE_X11_COMBINED`

This mode functions like **Accelerated** mode, however, you can hold `KC_ACL0`, `KC_ACL1` and `KC_ACL2`
to momentarily (while held) set the cursor and scroll speeds to constant speeds. When no acceleration
keys are held, this mode is identical to **Accelerated** mode, and can be modified using all of the
relevant settings.

* **KC_ACL0:** This acceleration sets your cursor to the slowest possible speed. This is useful for very
small and detailed movements of the cursor.
* **KC_ACL1:** This acceleration sets your cursor to half the full (user defined) speed.
* **KC_ACL2:** This acceleration sets your cursor to the maximum possible (127 pixels per `MOUSEKEY_INTERVAL`) speed. This is useful for moving the cursor large distances without much accuracy.

### Kinetic Mode `MK_TYPE_KINETIC`

This is an extension of the accelerated mode. The kinetic mode uses a quadratic curve on the cursor speed which allows precise movements at the beginning and allows to cover large distances by increasing cursor speed quickly thereafter.  You can adjust the cursor and scrolling acceleration using the following settings in your keymap’s `config.h` file:

|Define                                |Default|Description                                              |
|:-------------------------------------|------:|:--------------------------------------------------------|
|`MOUSEKEY_DELAY`                      |      8|Delay between pressing a movement key and cursor movement|
|`MOUSEKEY_INTERVAL`                   |      8|Time between cursor movements in milliseconds            |
|`MOUSEKEY_MOVE_DELTA`                 |     25|Step size for accelerating from initial to base speed    |
|`MOUSEKEY_INITIAL_SPEED`              |    100|Initial speed of the cursor in pixel per second          |
|`MOUSEKEY_BASE_SPEED`                 |   1000|Maximum cursor speed at which acceleration stops         |
|`MOUSEKEY_DECELERATED_SPEED`          |    400|Decelerated cursor speed                                 |
|`MOUSEKEY_ACCELERATED_SPEED`          |   3000|Accelerated cursor speed                                 |
|`MOUSEKEY_WHEEL_INITIAL_MOVEMENTS`    |     16|Initial number of movements of the mouse wheel           |
|`MOUSEKEY_WHEEL_BASE_MOVEMENTS`       |     32|Maximum number of movements at which acceleration stops  |
|`MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS`|     48|Accelerated wheel movements                              |
|`MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS`|      8|Decelerated wheel movements                              |

Tips:

* The smoothness of the cursor movement depends on the `MOUSEKEY_INTERVAL` setting. The shorter the interval is set the smoother the movement will be.  Setting the value too low makes the cursor unresponsive.  Lower settings are possible if the micro processor is fast enough. For example: At an interval of `8` milliseconds, `125` movements per second will be initiated.  With a base speed of `1000` each movement will move the cursor by `8` pixels.
* Mouse wheel movements are implemented differently from cursor movements. While it's okay for the cursor to move multiple pixels at once for the mouse wheel this would lead to jerky movements. Instead, the mouse wheel operates at step size `1`. Setting mouse wheel speed is done by adjusting the number of wheel movements per second.

### Constant & momentary modes

In this mode you can define multiple different speeds for both the cursor and the mouse wheel. There is no acceleration. `KC_ACL0`, `KC_ACL1` and `KC_ACL2` change the cursor and scroll speed to their respective setting.

You can choose whether speed selection is momentary or tap-to-select:

* **Momentary `MK_TYPE_3_SPEED_MOMENTARY`:** The chosen speed is only active while you hold the respective key. When the key is raised, mouse keys returns to the unmodified speed.
  Default speeds: `KC_ACL0` < `KC_ACL1` < *unmodified* < `KC_ACL2`
* **Tap-to-select `MK_TYPE_3_SPEED`:** The chosen speed is activated when you press the respective key and remains active even after the key has been raised. The default speed is that of `KC_ACL1`. There is no unmodified speed.
  Default speeds: `KC_ACL0` < `KC_ACL1` < `KC_ACL2`

The following settings are available for adjusting cursor movement or scrolling:

|Define               |Default|Description                                |
|:--------------------|------:|:------------------------------------------|
|`MK_C_OFFSET_UNMOD`  |     16|Cursor offset per movement (unmodified)    |
|`MK_C_INTERVAL_UNMOD`|     16|Time between cursor movements (unmodified) |
|`MK_C_OFFSET_0`      |      1|Cursor offset per movement (`KC_ACL0`)     |
|`MK_C_INTERVAL_0`    |     32|Time between cursor movements (`KC_ACL0`)  |
|`MK_C_OFFSET_1`      |      4|Cursor offset per movement (`KC_ACL1`)     |
|`MK_C_INTERVAL_1`    |     16|Time between cursor movements (`KC_ACL1`)  |
|`MK_C_OFFSET_2`      |     32|Cursor offset per movement (`KC_ACL2`)     |
|`MK_C_INTERVAL_2`    |     16|Time between cursor movements (`KC_ACL2`)  |
|`MK_W_OFFSET_UNMOD`  |      1|Scroll steps per scroll action (unmodified)|
|`MK_W_INTERVAL_UNMOD`|     40|Time between scroll steps (unmodified)     |
|`MK_W_OFFSET_0`      |      1|Scroll steps per scroll action (`KC_ACL0`) |
|`MK_W_INTERVAL_0`    |    360|Time between scroll steps (`KC_ACL0`)      |
|`MK_W_OFFSET_1`      |      1|Scroll steps per scroll action (`KC_ACL1`) |
|`MK_W_INTERVAL_1`    |    120|Time between scroll steps (`KC_ACL1`)      |
|`MK_W_OFFSET_2`      |      1|Scroll steps per scroll action (`KC_ACL2`) |
|`MK_W_INTERVAL_2`    |     20|Time between scroll steps (`KC_ACL2`)      |

## Use with PS/2 Mouse and Pointing Device

Mouse keys button state is shared with [PS/2 mouse](feature_ps2_mouse.md) and [pointing device](feature_pointing_device.md) so mouse keys button presses can be used for clicks and drags.
