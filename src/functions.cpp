/*
 *  Copyright (C) 2019 René Pollesch <poliander@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "functions.hpp"

float isqrt(float f) {
    float fhalf = 0.5f * f;

    asm(
        "mov %1, %%eax;"
        "sar %%eax;"
        "mov $0x5F3759DF, %%ebx;"
        "sub %%eax, %%ebx;"
        "mov %%ebx, %0"
        : "=g"(f)
        : "g"(f)
        : "%eax", "%ebx"
    );

    f *= 1.5f - fhalf*f*f;

    return f;
}
