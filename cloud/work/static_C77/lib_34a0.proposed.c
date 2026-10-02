#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern s16 gViewportX, gViewportScaleX, gViewportScaleY, gViewportPendingFrames;
extern s16 gViewportOffsetX[], gViewportOffsetY[];
extern OSViMode *gViewportStruct, *gViewportDataPtr;
extern int fcvt(char *,const char *,char *);
extern s32 lzss_decode(void *,void *);
extern s32 inflate_entry(void *,void *,s32);
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
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

int __isinf(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value != 0.0; } return 0; }

int __isnan(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value == 0.0; } return 0; }

#pragma GLOBAL_ASM("build/C77/lib_34a0/fcvt.s")
#pragma GLOBAL_ASM("build/C77/lib_34a0/__ecvt_internal.s")
u8 *__round_helper(double value, s32 *exponent, u8 *start, u8 *end, u8 nextDigit, u8 *sign) {
    double integral;
    if (value != 0.0) {
        modf(value * 10.0, &integral);
    } else {
        integral = nextDigit - '0';
    }
    if (integral > 4.0) {
        for (;; --end) {
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
        }
    } else if (*sign == '-') {
        for (;; --end) {
            if (*end == '.') end--;
            if (*end != '0') break;
            if (end == start) *sign = 0;
        }
    }
    return start;
}

#pragma GLOBAL_ASM("build/C77/lib_34a0/__write_exponent.s")
int sprintf(char *output, const char *format, ...) {
    int result;
    char *arguments;
    arguments = (char *)&format + sizeof(format);
    result = fcvt(output, format, arguments);
    return result;
}

