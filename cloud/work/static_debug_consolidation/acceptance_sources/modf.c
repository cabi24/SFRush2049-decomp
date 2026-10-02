/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
double modf(register double input, register double* ip) {
    register double ax;
    double t;
    double x = input;
    ax = x > 0.0 ? x : -x;
    if (ax >= 4503599627370496.0) {
        *ip = x;
        return 0.0;
    } else {
        t = ax + 4503599627370496.0;
        t = t - 4503599627370496.0;
        if (ax < t) t = t - 1.0;
        if (t > 0.0) {} else t = -t;
    }
    if (x - t == 1.0) t = t + 1.0;
    if (x >= 0.0) {
        *ip = t;
        return x - t;
    } else {
        *ip = -t;
        return x + t;
    }
}
