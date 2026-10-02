/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern double modf(double, double *);
u8 *__round_helper(double value, s32 *exponent, u8 *start, u8 *end, u8 nextDigit, u8 *sign) {
    double integral;
    if (value != 0.0) {
        modf(value * 10.0, &integral);
    } else {
        integral = nextDigit - '0';
    }
    if (integral > 4.0) {
        for (;;) {
            if (*end == '.') end--;
            ++*end;
            if (*end <= '9') break;
            *end = '0';
            if (end == start) {
                if (exponent != NULL) {
                    *end = '1';
                    ++*exponent;
                } else {
                    *--end = '1';
                    --start;
                }
                break;
            }
            --end;
        }
    } else if (*sign == '-') {
        for (;;) {
            if (*end == '.') end--;
            if (*end != '0') break;
            if (end == start) *sign = 0;
            --end;
        }
    }
    return start;
}
