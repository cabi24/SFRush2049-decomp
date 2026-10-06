import sys
src, dst = sys.argv[1], sys.argv[2]
s = open(src).read()
NM = ('char nm[256]; uint32_t ln = MEM_U32(0x1001c8d0); uint32_t k; if (ln > 255) ln = 255; '
      'for (k = 0; k < ln; k++) nm[k] = MEM_S8(0x1001c4d0 + k); nm[ln] = 0; ')
old = "L46d668:\n// bdead c0e0000b gp = MEM_U32(sp + 60);\nif (v0 == 0) {\nt2 = s5 << 3;\ngoto L46d718;}"
assert s.count(old) == 1, s.count(old)
s = s.replace(old, "L46d668:\nif (v0 != 0) { " + NM + 'fprintf(stderr, "SPILLTEMP proc=%s class=%u bit=%u\\n", nm, MEM_U32(sp + 140), s5); }\n' + old[len("L46d668:\n"):])
open(dst, "w").write(s)
