#!/usr/bin/env python3
"""ugen71_addrs.py FILE.c -- retarget the workbench `instrument-ugen` globals (pinned to the 5.3 ugen) to 7.1.

The generic instrumenter hooks every f_* function and the free-list/emit helpers by name (those anchors exist
in the 7.1 ugen.c too), but it reads three ugen globals at fixed 5.3 addresses. The 7.1 addresses come from
the same functions in the 7.1 generated source (decompals/ido-static-recomp v1.2, ugen.c sha256 44c24d9d...):
  ibuffer forward cursor  0x10018e70 -> 0x10021238   (f_emit_ll/f_emit_rll/... `a2 = 0x10021238`)
  ibuffer backward cursor 0x10018e78 -> 0x10021240   (f_demit_itext/f_demit_rrll `t4/a1 = 0x10021240`)
  current source line     0x10018e00 -> 0x10021220   (f_warning: fprintf(..., "line %d", MEM_U32(...)))
"""
import sys
p = sys.argv[1]
s = open(p).read()
for old, new in (('0x10018e70u', '0x10021238u'), ('0x10018e78u', '0x10021240u'), ('0x10018e00u', '0x10021220u')):
    n = s.count('#define DKWB_') and s.count(old)
    if n != 1:
        raise SystemExit('ugen71_addrs: %s occurs %d times' % (old, n))
    s = s.replace(old, new)
open(p, 'w').write(s)
print('ugen71_addrs: retargeted 3 globals in', p)
