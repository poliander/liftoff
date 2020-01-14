/*
 *  LIFT-OFF: BEYOND GLAXIUM
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

float mix(float v1, float v2, float b) {
    return v1 + b * (v2 - v1);
}

float ease_start2(float v) {
    return v * v;
}

float ease_start3(float v) {
    return v * v * v;
}

float ease_start4(float v) {
    return v * v * v * v;
}

float ease_start5(float v) {
    return v * v * v * v * v;
}

float ease_stop2(float v) {
    return 1.0f - ((1.0f - v) * (1.0f - v));
}

float ease_stop3(float v) {
    return 1.0f - ((1.0f - v) * (1.0f - v) * (1.0f - v));
}

float ease_stop4(float v) {
    return 1.0f - ((1.0f - v) * (1.0f - v) * (1.0f - v) * (1.0f - v));
}

float ease_stop5(float v) {
    return 1.0f - ((1.0f - v) * (1.0f - v) * (1.0f - v) * (1.0f - v) * (1.0f - v));
}

float ease_step2(float t) {
    return (t * t) / ((t * t) + ((1.0f - t) * (1.0f - t)));
}

float ease_step3(float t) {
    return (t * t * t) / ((t * t * t) + ((1.0f - t) * (1.0f - t) * (1.0f - t)));
}

float ease_step4(float t) {
    return (t * t * t * t) / ((t * t * t * t) + ((1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t)));
}

float ease_step5(float t) {
    return (t * t * t * t * t) / ((t * t * t * t * t) + ((1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t)));
}
