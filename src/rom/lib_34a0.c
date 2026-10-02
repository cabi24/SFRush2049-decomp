/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/* GENERATED ROM-aligned TU — segment 0x34a0 (rom/lib_34a0)
 * layout map f54dc176ce4855b86d3262eb2d2e0a66fa10aa93d8447be4d46e417ab6c9dc75; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

/* PROMOTED 2026-10-02 — modf
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/modf.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/modf.c:modf (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — modff
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/modff.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/modff.c:modff (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — __isinf
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/__isinf.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/__isinf.c:__isinf (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int __isinf(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value != 0.0; } return 0; }

/* PROMOTED 2026-10-02 — __isnan
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/__isnan.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/__isnan.c:__isnan (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int __isnan(register double x) { DoubleUnion v; v.value=x; if(v.bits.exponent==2047) { v.bits.exponent=0; return v.value == 0.0; } return 0; }

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/fcvt.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/__ecvt_internal.s")
/* PROMOTED 2026-10-02 — __round_helper
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/__round_helper.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/__round_helper.c:__round_helper (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_34a0/__write_exponent.s")
/* PROMOTED 2026-10-02 — sprintf
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/sprintf.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/sprintf.c:sprintf (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int sprintf(char *output, const char *format, ...) {
    int result;
    char *arguments;
    arguments = (char *)&format + sizeof(format);
    result = fcvt(output, format, arguments);
    return result;
}

