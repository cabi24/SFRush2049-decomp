#!/usr/bin/env python3
"""patch_spill.py IN.c OUT.c -- spill-temp / local-area hooks for the recompiled IDO 5.3 uopt.c.

Consolidates, unchanged in what they print, the lane patches that traced f_spilltemps:
  w6a patch_area2.py  AREA <fn>:<line> proc= area=   every write of the local-area size (0x1001c4b4),
                      tagged with the writing uopt function (f_readnxtinst = cfe locals, f_spilltemps = temp homes)
                      GETTEMP proc= size= found= area=  every f_gettemp call
  w6a patch_sp.py     SPILLTEMP proc= class= bit=    every expression bit f_spilltemps considers (1 int, 2 fp)
  w7a patch_home2.py  HOME proc= bit= home= reused= size=   one line per temp homed by f_spilltemps phase 2
  w7a patch_confl.py  CONFL proc= bit= other= blk=   temp `bit` meets lower-numbered homed temp `other` in block blk
  w7a patch_gt2.py    GETTEMP ... idx= flags=        chosen home index and free(F)/busy(b) flags of the home list
  w10h patch_tmp.py   [TMP] procinit/gettemp/<fn> disp a->b   per-procedure temp-area (tempdisp) changes
  w10h (hand edit)    [TMP] spill web= kind= size= temp= off=  per coloured web: the slot f_spilltemps gives it

Differences from the lane copies (deliberate):
  * every print is gated: the AREA/SPILLTEMP/HOME/CONFL/GETTEMP family by env SPLOG, the [TMP] family by env
    TMPLOG (w7a's uopt3 printed unconditionally). All prints go to stderr.
  * w10h's "spill web" hook was a hand edit of the generated file; it is anchored here at L46dbdc.
  * AREA lines name the original function (the [TMP] wrappers rename bodies to <fn>_X).
Every hook only reads emulated memory, so `opt` is byte-identical to the stock pass (install.sh checks it).
Apply after instrument_w5d.py (works on the raw uopt.c too).
"""
import re
import sys

MARK = 'WTK_UOPT_SPILL_V1'
NM = ('char nm[256]; uint32_t ln = MEM_U32(0x1001c8d0); uint32_t k; if (ln > 255) ln = 255; '
      'for (k = 0; k < ln; k++) nm[k] = MEM_S8(0x1001c4d0 + k); nm[ln] = 0; ')
HEADER = r'''
/* WTK_UOPT_SPILL_V1 -- spill-temp / area hooks (SPLOG, TMPLOG), cloud/work/frontier/tools/trace */
#include <stdio.h>
#include <stdlib.h>
static int wtk_splog = -1, tmplog_on = -1, tmplog_proc = 0;
#define SPON (wtk_splog < 0 ? (wtk_splog = getenv("SPLOG") != NULL) : wtk_splog)
#define TMPON (tmplog_on < 0 ? (tmplog_on = getenv("TMPLOG") != NULL) : tmplog_on)
'''


def once(s, old, new, label):
    n = s.count(old)
    if n != 1:
        raise SystemExit('patch_spill: anchor %s occurs %d times' % (label, n))
    return s.replace(old, new, 1)


def area2(src):
    """w6a patch_area2.py (line scan), gated."""
    lines = src.split('\n')
    out, fn, base = [], None, {}
    for i, l in enumerate(lines):
        if l.startswith('static ') and '(uint8_t *mem' in l and l.rstrip().endswith('{'):
            fn = l.split('(')[0].split()[-1]
            base = {}
        m = re.match(r'\s*(\w+) = 0x1001c4b4;', l)
        if m:
            base[m.group(1)] = i
        out.append(l)
        m = re.match(r'\s*MEM_U32\((\w+) \+ 0\) = (\w+);', l)
        if m and m.group(1) in base and i - base[m.group(1)] < 120:
            out.append('if (SPON) { ' + NM + 'fprintf(stderr, "AREA %s:%d proc=%s area=%d\\n", "' + fn + '", '
                       + str(i) + ', nm, (int)MEM_U32(0x1001c4b4)); }')
        if l == 'L47fd90:':
            # w6a GETTEMP + w7a patch_gt2 extension in one print
            out.append('if (SPON) { ' + NM + '{ char fl[64]; uint32_t e = MEM_U32(0x1001c4b8); int n = 0; '
                       "while (e && n < 12) { fl[n++] = MEM_U8(e + 12) ? 'F' : 'b'; e = MEM_U32(e + 16); } fl[n] = 0; "
                       'fprintf(stderr, "GETTEMP proc=%s size=%u found=%u area=%d idx=%d flags=%s\\n", nm, a2, v0, '
                       '(int)MEM_U32(0x1001c4b4), (v0 && v1) ? (int)MEM_U32(v1 + 0) : -1, fl); } }')
    return '\n'.join(out)


def sp_home_confl(s):
    old = 'L46d668:\n// bdead c0e0000b gp = MEM_U32(sp + 60);\nif (v0 == 0) {\nt2 = s5 << 3;\ngoto L46d718;}'
    s = once(s, old, 'L46d668:\nif (SPON && v0 != 0) { ' + NM + 'fprintf(stderr, "SPILLTEMP proc=%s class=%u bit=%u\\n", '
             'nm, MEM_U32(sp + 140), s5); }\n' + old[len('L46d668:\n'):], 'SPILLTEMP')
    old = 'L46dabc:\na2 = MEM_U32(sp + 160);\n'
    s = once(s, old, old + 'if (SPON) { ' + NM + 'fprintf(stderr, "HOME proc=%s bit=%u home=%u reused=%u size=%u\\n", '
             'nm, MEM_U32(sp + 188), s5, t5, s0); }\n', 'HOME')
    old = 'L46d8a0:\n// bdead c0ee000b gp = MEM_U32(sp + 60);\nif (v0 == 0) {'
    s = once(s, old, 'L46d8a0:\nif (SPON && v0 != 0) { ' + NM + 'fprintf(stderr, "CONFL proc=%s bit=%u other=%u blk=%x\\n", '
             'nm, MEM_U32(sp + 188), s0, fp); }\n' + old[len('L46d8a0:\n'):], 'CONFL')
    s = once(s, 'L46dbdc:\n', 'L46dbdc:\nif (TMPON) { uint32_t rec = MEM_U32(sp + 148); fprintf(stderr, '
             '"[TMP] spill web=%d kind=%d size=%d temp=%d off=%d\\n", (int)MEM_U32(sp + 188), '
             '(int)MEM_U8(MEM_U32(sp+160)), (int)s0, (int)MEM_U32(rec+0), (int)MEM_U32(rec+4)); }\n', 'spill web')
    return s


TMP_FNS = {
    'f_spilltemps': ('void', ['mem', 'sp'], '(uint8_t *mem, uint32_t sp)'),
    'func_424ddc': ('void', ['mem', 'sp', 'v0', 'a0', 'a1', 'a2', 'a3'],
                    '(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)'),
    'f_reemit': ('void', ['mem', 'sp'], '(uint8_t *mem, uint32_t sp)'),
    'func_464e50': ('void', ['mem', 'sp', 'v0', 'a0', 'a1'], '(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, uint32_t a1)'),
    'func_466790': ('uint32_t', ['mem', 'sp', 'v0', 'a0', 'a1', 'a2'],
                    '(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, uint32_t a1, uint32_t a2)'),
    'func_42c548': ('void', ['mem', 'sp', 'v0'], '(uint8_t *mem, uint32_t sp, uint32_t v0)'),
    'f_gettemp': ('void', ['mem', 'sp', 'a0', 'a1'], '(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1)'),
    'f_procinit': ('void', ['mem', 'sp'], '(uint8_t *mem, uint32_t sp)'),
}


def tmp(src):
    """w10h patch_tmp.py: wrap the temp-allocation functions, print tempdisp (0x1001c4b4) changes."""
    wr = []
    for fn, (rt, args, sig) in TMP_FNS.items():
        src = once(src, 'static %s %s%s {' % (rt, fn, sig),
                   'static %s %s_X%s;\nstatic %s %s_X%s {' % (rt, fn, sig, rt, fn, sig), fn)
        call = '%s_X(%s)' % (fn, ', '.join(args))
        body = 'uint32_t d0 = MEM_U32(0x1001c4b4);' + (call + ';' if rt == 'void' else 'uint32_t r = ' + call + ';')
        if fn == 'f_procinit':
            body += 'tmplog_proc++; if (TMPON) fprintf(stderr, "[TMP] procinit proc=%d\\n", tmplog_proc);'
        elif fn == 'f_gettemp':
            body += ('if (TMPON) { uint32_t rec = MEM_U32(a0); fprintf(stderr, "[TMP] gettemp proc=%d size=%d num=%d '
                     'off=%d recsize=%d disp %d->%d\\n", tmplog_proc, (int)a1, (int)MEM_U32(rec+0), (int)MEM_U32(rec+4), '
                     '(int)MEM_U32(rec+8), (int)d0, (int)MEM_U32(0x1001c4b4)); }')
        else:
            body += ('if (TMPON && d0 != MEM_U32(0x1001c4b4)) fprintf(stderr, "[TMP] %s proc=%%d disp %%d->%%d\\n", '
                     'tmplog_proc, (int)d0, (int)MEM_U32(0x1001c4b4));' % fn)
        if rt != 'void':
            body += 'return r;'
        wr.append('static %s %s%s { %s }\n' % (rt, fn, sig, body))
    return src + '\n' + ''.join(wr)


def main():
    src = open(sys.argv[1]).read()
    if MARK in src:
        raise SystemExit('patch_spill: already applied')
    src = once(src, '#include "header.h"\n', '#include "header.h"\n' + HEADER, 'header')
    src = area2(src)
    src = sp_home_confl(src)
    src = tmp(src)
    open(sys.argv[2], 'w').write(src)
    print('patch_spill ->', sys.argv[2])


if __name__ == '__main__':
    main()
