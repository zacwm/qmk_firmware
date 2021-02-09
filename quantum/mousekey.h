/*
Copyright 2011 Jun Wako <wakojun@gmail.com>
Copyright 2021 Liyang HU <qmk.tk@liyang.hu>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>

/* TYPEs of the same KIND differ only in the LSB. */
/* If we ever extend the hierarchy: UNIV ∋ SORT ∋ KIND ∋ TYPE */
/* clang-format off */

#define MK_TYPE_X11                  0x1100
#define MK_TYPE_X11_COMBINED         0x1101
#define MK_TYPE_3_SPEED              0x0300
#define MK_TYPE_3_SPEED_MOMENTARY    0x0301
#define MK_TYPE_KINETIC              0x0200

/* clang-format on */

#ifdef __cplusplus
extern "C" {
#endif

void mousekey_task(void);
void mousekey_on(uint8_t code);
void mousekey_off(uint8_t code);
void mousekey_clear(void);
void mousekey_send(void);

#ifdef __cplusplus
}
#endif
