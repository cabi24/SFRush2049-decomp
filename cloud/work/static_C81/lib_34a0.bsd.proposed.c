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
extern double gPerspFov, gPerspAspect;
extern u8 *__round_helper(double, s32 *, u8 *, u8 *, u8, u8 *);
extern u8 *__write_exponent(u8 *, s32, s32);
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

#pragma GLOBAL_ASM("build/C81/lib_34a0/fcvt.s")
int __ecvt_internal(double number, register int prec, int flags, u8 *signp,
                     int fmtch, u8 *startp, u8 *endp)
{
	register u8 *p, *t;
	register double fract;
	int dotrim, expcnt, gformat;
	double integer, tmp;

	gformat = 0;
	expcnt = 0;
	dotrim = 0;
	if (number < 0) {
		number = -number;
		*signp = '-';
	} else
		*signp = 0;

	fract = modf(number, &integer);

	/* get an extra slot for rounding. */
	t = ++startp;

	/*
	 * get integer portion of number; put into the end of the buffer; the
	 * .01 is added for modf(356.0 / 10, &integer) returning .59999999...
	 */
	if (number >= 1.0) {
	for (p = endp - 1; integer; ++expcnt) {
		tmp = modf(integer / 10, &integer);
		*p-- = ('0' + (int)((tmp + gPerspFov) * 10));
	}
	} else p = endp - 1;
	switch (fmtch) {
	case 'f':
		/* reverse integer into beginning of buffer */
		if (expcnt)
			for (; ++p < endp; *t++ = *p);
		else
			*t++ = '0';
		/*
		 * if precision required or alternate flag set, add in a
		 * decimal point.
		 */
		if (prec || flags & 8)
			*t++ = '.';
		/* if requires more precision and some fraction left */
		if (fract) {
			if (prec)
				do {
					fract = modf(fract * 10, &tmp);
					*t++ = ('0' + (int)tmp);
				} while (--prec && fract);
			if (fract)
				startp = __round_helper(fract, (int *)NULL, startp,
				    t - 1, (char)0, signp);
		}
		for (; prec--; *t++ = '0');
		break;
	case 'e':
	case 'E':
eformat:	if (expcnt) {
			*t++ = *++p;
			if (prec || flags & 8)
				*t++ = '.';
			/* if requires more precision and some integer left */
			for (; prec && ++p < endp; --prec)
				*t++ = *p;
			/*
			 * if done precision and more of the integer component,
			 * round using it; adjust fract so we don't re-round
			 * later.
			 */
			if (!prec && ++p < endp) {
				fract = 0;
				startp = __round_helper((double)0, &expcnt, startp,
				    t - 1, *p, signp);
			}
			/* adjust expcnt for digit in front of decimal */
			--expcnt;
		}
		/* until first fractional digit, decrement exponent */
		else if (fract) {
			/* adjust expcnt for digit in front of decimal */
			for (expcnt = -1;; --expcnt) {
				fract = modf(fract * 10, &tmp);
				if (tmp)
					break;
			}
			*t++ = ('0' + (int)tmp);
			if (prec || flags & 8)
				*t++ = '.';
		}
		else {
			*t++ = '0';
			if (prec || flags & 8)
				*t++ = '.';
		}
		/* if requires more precision and some fraction left */
		if (fract) {
			if (prec)
				do {
					fract = modf(fract * 10, &tmp);
					*t++ = ('0' + (int)tmp);
				} while (--prec && fract);
			if (fract)
				startp = __round_helper(fract, &expcnt, startp,
				    t - 1, (char)0, signp);
		}
		/* if requires more precision */
		for (; prec--; *t++ = '0');

		/* unless alternate flag, trim any g/G format trailing 0's */
		if (gformat && !(flags & 8)) {
			while (t > startp && *--t == '0');
			if (*t == '.')
				--t;
			++t;
		}
		t = __write_exponent(t, expcnt, fmtch);
		break;
	case 'g':
	case 'G':
		/* a precision of 0 is treated as a precision of 1. */
		if (!prec)
			++prec;
		/*
		 * ``The style used depends on the value converted; style e
		 * will be used only if the exponent resulting from the
		 * conversion is less than -4 or greater than the precision.''
		 *	-- ANSI X3J11
		 */
		if (expcnt > prec || (!expcnt && fract && fract < gPerspAspect)) {
			/*
			 * g/G format counts "significant digits, not digits of
			 * precision; for the e/E format, this just causes an
			 * off-by-one problem, i.e. g/G considers the digit
			 * before the decimal point significant and e/E doesn't
			 * count it as precision.
			 */
			--prec;
			fmtch -= 2;		/* G->E, g->e */
			gformat = 1;
			goto eformat;
		}
		/*
		 * reverse integer into beginning of buffer,
		 * note, decrement precision
		 */
		if (expcnt)
			for (; ++p < endp; *t++ = *p, --prec);
		else
			*t++ = '0';
		/*
		 * if precision required or alternate flag set, add in a
		 * decimal point.  If no digits yet, add in leading 0.
		 */
		if (prec || flags & 8) {
			dotrim = 1;
			*t++ = '.';
		}
		else
			dotrim = 0;
		/* if requires more precision and some fraction left */
		if (fract) {
			if (prec) {
				do {
					fract = modf(fract * 10, &tmp);
					*t++ = ('0' + (int)tmp);
				} while(!tmp);
				while (--prec && fract) {
					fract = modf(fract * 10, &tmp);
					*t++ = ('0' + (int)tmp);
				}
			}
			if (fract)
				startp = __round_helper(fract, (int *)NULL, startp,
				    t - 1, (char)0, signp);
		}
		/* alternate format, adds 0's for precision, else trim 0's */
		if (flags & 8)
			for (; prec--; *t++ = '0');
		else if (dotrim) {
			while (t > startp && *--t == '0');
			if (*t != '.')
				++t;
		}
	}
	return (t - startp);
}


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

u8 *__write_exponent(register u8 *output, register int exponent, register int format) {
    u8 *digit;
    u8 buffer[308];
    *output++ = format;
    if (exponent < 0) {exponent = -exponent; *output++ = '-';}
    else *output++ = '+';
    digit = buffer + sizeof(buffer);
    if (exponent > 9) {
        do {
            *--digit = exponent % 10 + '0';
        } while ((exponent /= 10) > 9);
        *--digit = exponent + '0';
        for (; digit < buffer + sizeof(buffer); *output++ = *digit++) {}
    } else {
        *output++ = '0';
        *output++ = exponent + '0';
    }
    return output;
}

int sprintf(char *output, const char *format, ...) {
    int result;
    char *arguments;
    arguments = (char *)&format + sizeof(format);
    result = fcvt(output, format, arguments);
    return result;
}

