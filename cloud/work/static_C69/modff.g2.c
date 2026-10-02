/* flags: -g2 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
float modff(register float input, register float* ip) {
    register float ax;
    float t;
    float x = input;
    ax = x > 0.0f ? x : -x;
    if (ax >= 8388608.0f) {
        *ip = x;
        return 0.0f;
    } else {
        t = ax + 8388608.0f;
        t = t - 8388608.0f;
        if (ax < t) t = t - 1.0f;
        if (t > 0.0f) {} else t = -t;
    }
    if (x - t == 1.0f) t = t + 1.0f;
    if (x >= 0.0f) {
        *ip = t;
        return x - t;
    } else {
        *ip = -t;
        return x + t;
    }
}
