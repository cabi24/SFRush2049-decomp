"""Build the complete native parser from the original Berkeley BSD ancestor.
The pinned original is read privately; no previous GNU packet is copied.
"""
from pathlib import Path
import re, hashlib
bsd = Path('build/C80/bsd386_printf.c').read_text()
assert hashlib.sha256(bsd.encode()).hexdigest() == 'b8dd888df9dc8466724aee4a70d7d186f640ce74e35002d8fd7ce9beac26f358'
notice = bsd[:bsd.index('*/')+2]
a = bsd.index('\t/*\n\t * Scan the format for conversions')
b = bsd.index('\n#ifdef FLOATING_POINT\n#include <math.h>', a)
body = bsd[a:b]
body = re.sub(r'^#ifdef FLOATING_POINT\n|^#endif /\* FLOATING_POINT \*/\n', '', body, flags=re.M)
body = body.replace('#else\n\t\tfieldsz = size;\n#endif\n','')
body = body.replace('\t\t\t_double = va_arg(ap, double);','\t\t\t_double = va_arg(ap, double);')
body = body.replace('isinf(_double)', '__isnan(_double)').replace('isnan(_double)', '__isinf(_double)')
body = body.replace('cp = "Inf";', 'cp = gFcvtSign;').replace('cp = "NaN";', 'cp = gFcvtDecpt;')
body = body.replace('size = cvt(', 'size = __ecvt_internal(')
body = body.replace('xdigs = "0123456789abcdef";', 'xdigs = gFcvtTemp1;', 1)
body = body.replace('xdigs = "0123456789ABCDEF";', 'xdigs = gFcvtTemp3;').replace('xdigs = "0123456789abcdef";', 'xdigs = gFcvtTemp4;')
body = body.replace('cp = "(null)";', 'cp = gFcvtTemp2;').replace('cp = "bug in vfprintf: bad base";', 'cp = gFcvtTemp5;')
body = body.replace('char *p = memchr', 'u8 *p = memchr')
body = body.replace("\t\tcase 'O':", "\t\tcase 'b':\n\t\t\t_ulong = UARG();\n\t\t\tbase = BIN;\n\t\t\tgoto nosign;\n\t\tcase 'O':")
body = body.replace('\t\t\t\tdefault:\n', "\t\t\t\tcase BIN:\n\t\t\t\t\tdo {\n\t\t\t\t\t\t*--cp = to_char(_ulong & 1);\n\t\t\t\t\t\t_ulong >>= 1;\n\t\t\t\t\t} while (_ulong);\n\t\t\t\t\tbreak;\n\t\t\t\tdefault:\n")
body = re.sub(r'^#endif\n', '', body, flags=re.M)
body = body.replace('error:\n\treturn (__sferror(fp) ? EOF : ret);','\treturn strlen(original_output);')
pre = '''/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
'''+notice+'''
/* Adapted directly from Berkeley vfprintf.c 5.47 (3/22/91).
 * Native output copying, actual N64 ABI, original globals and binary
 * conversion follow protected assembly; no GNU parser is imported. */
#include "rom_tu.h"
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
int fcvt(u8 *fp, const u8 *fmt0, char *ap) {
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
    u8 *original_output = fp;
    u8 blanks[16] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    u8 zeroes[16] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    fmt = (u8 *)fmt0;
    ret = 0;
'''
Path('cloud/work/static_C82/fcvt.bsd_native.c').write_text(pre+body)
