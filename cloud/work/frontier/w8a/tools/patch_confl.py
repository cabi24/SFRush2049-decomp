import sys
# adds: one line per (homed temp, other temp in a common block's +0x15c set) seen by f_spilltemps phase 2
src, dst = sys.argv[1], sys.argv[2]
s = open(src).read()
NM = ('char nm[256]; uint32_t ln = MEM_U32(0x1001c8d0); uint32_t k; if (ln > 255) ln = 255; '
      'for (k = 0; k < ln; k++) nm[k] = MEM_S8(0x1001c4d0 + k); nm[ln] = 0; ')
old = "L46d8a0:\n// bdead c0ee000b gp = MEM_U32(sp + 60);\nif (v0 == 0) {"
assert s.count(old) == 1, s.count(old)
s = s.replace(old, "L46d8a0:\nif (v0 != 0) { " + NM + 'fprintf(stderr, "CONFL proc=%s bit=%u other=%u blk=%x\\n", nm, MEM_U32(sp + 188), s0, fp); }\n' + old[len("L46d8a0:\n"):])
open(dst, "w").write(s)
