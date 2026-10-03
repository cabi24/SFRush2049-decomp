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

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
/* Adapted directly from Berkeley vfprintf.c 5.47 (3/22/91).
 * Native output copying, actual N64 ABI, original globals and binary
 * conversion follow protected assembly; no GNU parser is imported. */
extern void *memcpy(void *, const void *, u32);
extern s32 strlen(const void *);
extern u8 *memchr(u8 *, u32, s32);
extern int __isnan(double), __isinf(double);
extern int __ecvt_internal(double, int, int, u8 *, int, u8 *, u8 *);
extern u8 gFcvtSign[], gFcvtDecpt[], gFcvtTemp1[], gFcvtTemp2[], gFcvtTemp3[], gFcvtTemp4[], gFcvtTemp5[];
/* Canonical SDK IDO o32 va_arg implementation. */
#define _VA_ALIGN(p,a) (((unsigned int)(((char *)(p)) + ((a)>4?(a):4)-1)) & -((a)>4?(a):4))
#define __va_stack_arg(list,mode) (((list)=(char *)_VA_ALIGN(list,__builtin_alignof(mode)) + _VA_ALIGN(sizeof(mode),4)), ((char *)list)-(_VA_ALIGN(sizeof(mode),4)-sizeof(mode)))
#define __va_double_arg(list,mode) (((long)list & 1) ? (list=(char *)((long)list+7),(char *)((long)list-6-16)) : (((long)list & 2) ? (list=(char *)((long)list+10),(char *)((long)list-24-16)) : __va_stack_arg(list,mode)))
#define va_arg(list,mode) (((mode *)((__builtin_classof(mode)==1 && __builtin_alignof(mode)==sizeof(double)) ? __va_double_arg(list,mode) : __va_stack_arg(list,mode)))[-1])
#define LONGINT 1
#define LONGDBL 2
#define SHORTINT 4
#define ALT 8
#define LADJUST 16
#define ZEROPAD 32
#define HEXPREFIX 64
#define MAXEXP 308
#define MAXFRACT 39
#define BUF (MAXEXP+MAXFRACT+1)
#define DEFPREC 6
#define to_digit(c) ((c)-'0')
#define is_digit(c) ((unsigned)to_digit(c)<=9)
#define to_char(n) ((n)+'0')
#define SARG() (flags&LONGINT ? va_arg(ap,long) : flags&SHORTINT ? (long)(short)va_arg(ap,int) : va_arg(ap,int))
#define UARG() (flags&LONGINT ? va_arg(ap,unsigned long) : flags&SHORTINT ? (unsigned long)(unsigned short)va_arg(ap,int) : va_arg(ap,unsigned int))
#define PRINT(ptr,len) {memcpy(fp,(ptr),(len)); fp += (len);}
#define PADSIZE 16
#define PAD(howmany,with) {if((n=(howmany))>0) {while(n>PADSIZE) {PRINT(with,PADSIZE); n-=PADSIZE;} PRINT(with,n);}}
#define FLUSH() (*fp = 0)
int fcvt(char *fp, const char *fmt0, char *ap) {
    register u8 *fmt;
    register int ch, n;
    register u8 *cp;
    register int flags;
    int ret, width, prec;
    u8 sign;
    u8 softsign;
    double _double;
    int fpprec;
    unsigned long _ulong;
    enum {OCT,DEC,HEX,BIN} base;
    int dprec, fieldsz, realsz, size;
    u8 *xdigs = NULL;
    u8 buf[BUF];
    u8 ox[2];
    char *original_output = fp;
    u8 blanks[16] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    u8 zeroes[16] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    fmt = (u8 *)fmt0;
    ret = 0;
	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if ((n = fmt - cp) != 0) {
			PRINT(cp, n);
			ret += n;
		}
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		fpprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			if ((width = va_arg(ap, int)) >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				n = va_arg(ap, int);
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + ch - '0';
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *	-- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + ch - '0';
				ch = *fmt++;
			} while (is_digit(ch));
			width = n;
			goto reswitch;
		case 'L':
			flags |= LONGDBL;
			goto rflag;
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			flags |= LONGINT;
			goto rflag;
		case 'c':
			*(cp = buf) = va_arg(ap, int);
			size = 1;
			sign = '\0';
			break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
			_ulong = SARG();
			if ((long)_ulong < 0) {
				_ulong = -_ulong;
				sign = '-';
			}
			base = DEC;
			goto number;
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			_double = va_arg(ap, double);
			/* do this before tricky precision changes */
			if (__isnan(_double)) {
				if (_double < 0)
					sign = '-';
				cp = gFcvtSign;
				size = 3;
				break;
			}
			if (__isinf(_double)) {
				cp = gFcvtDecpt;
				size = 3;
				break;
			}
			/*
			 * don't do unrealistic precision; just pad it with
			 * zeroes later, so buffer size stays rational.
			 */
			if (prec > MAXFRACT) {
				if (ch != 'g' && ch != 'G' || (flags&ALT))
					fpprec = prec - MAXFRACT;
				prec = MAXFRACT;
			} else if (prec == -1)
				prec = DEFPREC;
			/*
			 * cvt may have to round up before the "start" of
			 * its buffer, i.e. ``intf("%.2f", (double)9.999);'';
			 * if the first character is still NUL, it did.
			 * softsign avoids negative 0 if _double < 0 but
			 * no significant digits will be shown.
			 */
			cp = buf;
			*cp = '\0';
			size = __ecvt_internal(_double, prec, flags, &softsign, ch,
			    cp, buf + sizeof(buf));
			if (softsign)
				sign = '-';
			if (*cp == '\0')
				cp++;
			break;
		case 'n':
			if (flags & LONGINT)
				*va_arg(ap, long *) = ret;
			else if (flags & SHORTINT)
				*va_arg(ap, short *) = ret;
			else
				*va_arg(ap, int *) = ret;
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
			_ulong = UARG();
			base = OCT;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *	-- ANSI X3J11
			 */
			/* NOSTRICT */
			_ulong = (unsigned long)va_arg(ap, void *);
			base = HEX;
			xdigs = gFcvtTemp1;
			flags |= HEXPREFIX;
			ch = 'x';
			goto nosign;
		case 's':
			if ((cp = va_arg(ap, char *)) == NULL)
				cp = gFcvtTemp2;
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				u8 *p = memchr(cp, 0, prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				} else
					size = prec;
			} else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
			_ulong = UARG();
			base = DEC;
			goto nosign;
		case 'B':
		case 'b':
			_ulong = UARG();
			base = BIN;
			goto nosign;
		case 'X':
			xdigs = gFcvtTemp3;
			goto hex;
		case 'x':
			xdigs = gFcvtTemp4;
hex:			_ulong = UARG();
			base = HEX;
			/* leading 0x/X only if non-zero */
			if (flags & ALT && _ulong != 0)
				flags |= HEXPREFIX;

			/* unsigned conversions */
nosign:			sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *	-- ANSI X3J11
			 */
number:			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *	-- ANSI X3J11
			 */
			cp = buf + BUF;
			if (_ulong != 0 || prec != 0) {
				/*
				 * unsigned mod is hard, and unsigned mod
				 * by a constant is easier than that by
				 * a variable; hence this switch.
				 */
				switch (base) {
				case OCT:
					do {
						*--cp = to_char(_ulong & 7);
						_ulong >>= 3;
					} while (_ulong);
					/* handle octal leading 0 */
					if (flags & ALT && *cp != '0')
						*--cp = '0';
					break;

				case DEC:
					/* many numbers are 1 digit */
					while (_ulong >= 10) {
						*--cp = to_char(_ulong % 10);
						_ulong /= 10;
					}
					*--cp = to_char(_ulong);
					break;

				case HEX:
					do {
						*--cp = xdigs[_ulong & 15];
						_ulong >>= 4;
					} while (_ulong);
					break;

				case BIN:
					do {
						*--cp = to_char(_ulong & 1);
						_ulong >>= 1;
					} while (_ulong);
					break;
				default:
					cp = gFcvtTemp5;
					size = strlen(cp);
					goto skipsize;
				}
			}
			size = buf - cp + BUF;
		skipsize:
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			cp = buf;
			*cp = ch;
			size = 1;
			sign = '\0';
			break;
		}

		/*
		 * All reasonable formats wind up here.  At this point,
		 * `cp' points to a string which (if not flags&LADJUST)
		 * should be padded out to `width' places.  If
		 * flags&ZEROPAD, it should first be prefixed by any
		 * sign or other prefix; otherwise, it should be blank
		 * padded before the prefix is emitted.  After any
		 * left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print
		 * the string proper, then emit zeroes required by any
		 * leftover floating precision; finally, if LADJUST,
		 * pad with blanks.
		 */

		/*
		 * compute actual size, so we know how much to pad.
		 * fieldsz excludes decimal prec; realsz includes it
		 */
		fieldsz = size + fpprec;
		if (sign)
			fieldsz++;
		else if (flags & HEXPREFIX)
			fieldsz += 2;
		realsz = dprec > fieldsz ? dprec : fieldsz;

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0)
			PAD(width - realsz, blanks);

		/* prefix */
		if (sign) {
			PRINT(&sign, 1);
		} else if (flags & HEXPREFIX) {
			ox[0] = '0';
			ox[1] = ch;
			PRINT(ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
			PAD(width - realsz, zeroes);

		/* leading zeroes from decimal precision */
		PAD(dprec - fieldsz, zeroes);

		/* the string or number proper */
		PRINT(cp, size);

		/* trailing f.p. zeroes */
		PAD(fpprec, zeroes);

		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += width > realsz ? width : realsz;

		FLUSH();	/* copy out the I/O vectors */
	}
done:
	FLUSH();
	return strlen(original_output);
	/* NOTREACHED */
}

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

