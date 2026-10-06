import sys
# adds to an area/spilltemp-instrumented uopt.c: one line per spilled temp homed by f_spilltemps phase 2
# (bit index, chosen home index, reused=1/new=0, size)
src, dst = sys.argv[1], sys.argv[2]
s = open(src).read()
NM = ('char nm[256]; uint32_t ln = MEM_U32(0x1001c8d0); uint32_t k; if (ln > 255) ln = 255; '
      'for (k = 0; k < ln; k++) nm[k] = MEM_S8(0x1001c4d0 + k); nm[ln] = 0; ')
old = "L46dabc:\na2 = MEM_U32(sp + 160);\n"
assert s.count(old) == 1, s.count(old)
s = s.replace(old, old + "{ " + NM + 'fprintf(stderr, "HOME proc=%s bit=%u home=%u reused=%u size=%u\\n", nm, MEM_U32(sp + 188), s5, t5, s0); }\n')
open(dst, "w").write(s)
