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
