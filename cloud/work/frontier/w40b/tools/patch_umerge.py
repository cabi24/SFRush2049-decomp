#!/usr/bin/env python3
"""w40b lane tool: instrument the recompiled umerge.c (ido-static-recomp) pack_knapsack with stderr prints.
usage: patch_umerge.py IN.c OUT.c
Prints (stderr, prefix UMK):
  K1  every edge after estimate_inline (loop 1): callee#, caller#, line, benefit(e+24), size incr(e+28), callee/caller +28, +32,
      callee ncallers(+88)
  K1T loop-1 threshold result T vs C+c
  K2  loop-2 candidate: T, C (+28), c, then K2c the final s1/s2 (after multiplier/leaf) used in the space_time test.
Proc numbers are the '#N' of umerge -debug's call-graph dump (field +8)."""
import sys
src = open(sys.argv[1]).read()
P = 'fprintf(stderr, '
ins = {
 'L406f78:\n': P + '"UMK K1 callee=%u caller=%u line=%u ben=%d sz=%d c28=%u C28=%u c32=%u C32=%u ncall=%u inl=%u\\n", '
     'MEM_U32(MEM_U32(s0+16)+8), MEM_U32(MEM_U32(s0+8)+8), MEM_U32(s0+12), (int)MEM_U32(s0+24), (int)MEM_U32(s0+28), '
     'MEM_U32(MEM_U32(s0+16)+28), MEM_U32(MEM_U32(s0+8)+28), MEM_U32(MEM_U32(s0+16)+32), MEM_U32(MEM_U32(s0+8)+32), '
     'MEM_U32(MEM_U32(s0+16)+88), MEM_U32(MEM_U32(s0+16)+96));\n',
 'L406fd4:\n': P + '"UMK K1T line=%u T=%u C=%u c=%u\\n", MEM_U32(s0+12), v0, MEM_U32(MEM_U32(s0+8)+28), MEM_U32(MEM_U32(s0+16)+28));\n',
 'L407148:\n': P + '"UMK K2 callee=%u caller=%u line=%u T=%u C=%u c=%u cinl=%u cnc=%u\\n", MEM_U32(MEM_U32(s0+16)+8), '
     'MEM_U32(MEM_U32(s0+8)+8), MEM_U32(s0+12), v0, MEM_U32(MEM_U32(s0+8)+28), MEM_U32(MEM_U32(s0+16)+28), '
     'MEM_U32(MEM_U32(s0+8)+96), MEM_U32(MEM_U32(s0+8)+88));\n',
 'L407234:\n': P + '"UMK K2c line=%u s1=%d s2=%d\\n", MEM_U32(s0+12), (int)s1, (int)s2);\n',
}
for k, v in ins.items():
    n = src.count(k)
    assert n == 1, (k, n)
    src = src.replace(k, k + v)
if '#include <stdio.h>' not in src:
    src = '#include <stdio.h>\n' + src
open(sys.argv[2], 'w').write(src)
print('patched', len(ins))
