#!/usr/bin/env python3
"""spill71.py IN.c OUT.c -- spill-temp / local-area hooks for the recompiled IDO 7.1 uopt.c (SPLOG=1, stderr).

Port of ../../trace/patches/patch_spill.py (w6a/w7a/w10h, IDO 5.3) to the 7.1 generated source. f_spilltemps has the
same frame (sp+140 class, +148 home record, +155 reused flag, +160 web expr, +188 bit) in both releases; the globals
and labels moved:
    local-area size   5.3 0x1001c4b4 -> 7.1 0x1001c31c   (f_spilltemps/f_gettemp/f_procinit/f_readnxtinst... all match)
    procedure name    5.3 0x1001c4d0/len 0x1001c8d0 -> 7.1 0x1001eb00/len 0x1001c330
    SPILLTEMP  L46d668 -> L465f30     HOME  L46dabc -> L466320     CONFL  L46d8a0 -> L466148     spill web  L46dbdc -> L466438
Printed (every line carries proc=<name>):
    AREA <uopt fn>:<line> proc= area=N      every write of the local-area size (f_readnxtinst = cfe locals,
                                            f_spilltemps = spill homes)
    SPILLTEMP proc= class= bit=             each coloured expression web f_spilltemps considers (1 int, 2 fp)
    HOME proc= bit= home= reused= size=     slot chosen per web
    CONFL proc= bit= other= blk=            web `bit` meets lower-numbered homed web `other` in block blk
    SPILLWEB proc= web= kind= size= temp= off=   the slot (temp number, frame offset) each web gets
Not ported (5.3-only addresses): GETTEMP free-list flags and the [TMP] tempdisp wrappers.
Every hook only reads emulated memory, so the output is byte-identical to the stock pass (fidelity71.sh).
"""
import re
import sys

MARK = 'W22C_UOPT71_SPILL_V1'
NM = ('char nm[256]; uint32_t ln = MEM_U32(0x1001c330); uint32_t k; if (ln > 255) ln = 255; '
      'for (k = 0; k < ln; k++) nm[k] = MEM_S8(0x1001eb00 + k); nm[ln] = 0; while (ln > 0 && nm[ln - 1] == \' \') nm[--ln] = 0; ')
HEADER = r'''
/* W22C_UOPT71_SPILL_V1 -- spill-temp / area hooks (SPLOG), cloud/work/frontier/tools/trace71 */
#include <stdio.h>
#include <stdlib.h>
static int w71_splog = -1;
#define SPON (w71_splog < 0 ? (w71_splog = getenv("SPLOG") != NULL) : w71_splog)
'''
AREA = '0x1001c31c'


def once(s, old, new, label):
    n = s.count(old)
    if n != 1:
        raise SystemExit('spill71: anchor %s occurs %d times' % (label, n))
    return s.replace(old, new, 1)


def area(src):
    lines = src.split('\n')
    out, fn, base, hooks = [], None, {}, 0
    for i, l in enumerate(lines):
        if l.startswith('static ') and '(uint8_t *mem' in l and l.rstrip().endswith('{'):
            fn = l.split('(')[0].split()[-1]
            base = {}
        m = re.match(r'\s*(\w+) = %s;' % AREA, l)
        if m:
            base[m.group(1)] = i
        out.append(l)
        m = re.match(r'\s*MEM_U32\((\w+) \+ 0\) = (\w+);', l)
        if m and m.group(1) in base and i - base[m.group(1)] < 120:
            out.append('if (SPON) { ' + NM + 'fprintf(stderr, "AREA %s:%d proc=%s area=%d\\n", "' + fn + '", '
                       + str(i) + ', nm, (int)MEM_U32(' + AREA + ')); }')
            hooks += 1
    if hooks < 5:
        raise SystemExit('spill71: only %d AREA hooks' % hooks)
    return '\n'.join(out)


def hooks(s):
    s = once(s, 'L465f30:\nif (v0 == 0) {',
             'L465f30:\nif (SPON && v0 != 0) { ' + NM + 'fprintf(stderr, "SPILLTEMP proc=%s class=%u bit=%u\\n", '
             'nm, MEM_U32(sp + 140), s5); }\nif (v0 == 0) {', 'SPILLTEMP')
    s = once(s, 'L466320:\na2 = MEM_U32(sp + 160);\n',
             'L466320:\na2 = MEM_U32(sp + 160);\nif (SPON) { ' + NM + 'fprintf(stderr, "HOME proc=%s bit=%u home=%u '
             'reused=%u size=%u\\n", nm, MEM_U32(sp + 188), s5, t5, s0); }\n', 'HOME')
    s = once(s, 'L466148:\nif (v0 == 0) {',
             'L466148:\nif (SPON && v0 != 0) { ' + NM + 'fprintf(stderr, "CONFL proc=%s bit=%u other=%u blk=%x\\n", '
             'nm, MEM_U32(sp + 188), s0, fp); }\nif (v0 == 0) {', 'CONFL')
    s = once(s, 'L466438:\n', 'L466438:\nif (SPON) { ' + NM + 'uint32_t rec = MEM_U32(sp + 148); fprintf(stderr, '
             '"SPILLWEB proc=%s web=%d kind=%d size=%d temp=%d off=%d\\n", nm, (int)MEM_U32(sp + 188), '
             '(int)MEM_U8(MEM_U32(sp + 160)), (int)s0, (int)MEM_U32(rec + 0), (int)MEM_U32(rec + 4)); }\n', 'spill web')
    return s


def main():
    src = open(sys.argv[1]).read()
    if MARK in src:
        raise SystemExit('spill71: already applied')
    src = once(src, '#include "header.h"\n', '#include "header.h"\n' + HEADER, 'header')
    src = hooks(area(src))
    open(sys.argv[2], 'w').write(src)
    print('spill71 ->', sys.argv[2])


if __name__ == '__main__':
    main()
