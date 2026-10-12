# patch_ulr.py IN OUT: on top of patch_add.py output, print every live range's livbb list (bb, occurrences, f20..f23)
# and its lr+12 extra-block set at f_updatelivran exit (env ULR=sym, W5D_PROC procedure only). Diagnostic only.
import sys
s = open(sys.argv[1]).read()
a = 'static void f_updatelivran(uint8_t *mem, uint32_t sp, uint32_t a0) {\n'
assert s.count(a) == 1
s = s.replace(a, 'static void ulr_dump(uint8_t *mem, uint32_t sp, uint32_t lr, const char *w) { const char *e = getenv("ULR"); uint32_t l_; unsigned bb; if (!e || !w5d_selected(mem)) return; if (MEM_U16(MEM_U32(lr + 0) + 2) != (unsigned)atoi(e)) return; fprintf(stderr, "[ULR] %s lr=%u col=%d bbs:", w, MEM_U32(lr + 4), (int)MEM_S8(lr + 32)); for (l_ = MEM_U32(lr + 8); l_; l_ = MEM_U32(l_ + 4)) fprintf(stderr, " %u[o%u f20=%u f21=%u f22=%u f23=%u]", MEM_U16(MEM_U32(l_ + 0) + 8), MEM_U16(l_ + 16), MEM_U8(l_ + 20), MEM_U8(l_ + 21), MEM_U8(l_ + 22), MEM_U8(l_ + 23)); fprintf(stderr, " extra:"); for (bb = 0; bb < 200; bb++) if (f_bvectin(mem, sp - 0x100, bb, lr + 12)) fprintf(stderr, " %u", bb); fprintf(stderr, "\\n"); }\n' + a, 1)
# entry dump
s = s.replace(a + 'const uint32_t zero = 0;', a + 'const uint32_t zero = 0; uint32_t ulr_lr = a0; ulr_dump(mem, sp, a0, "in");', 1)
# exits: L46ebd0 / L46ebd4 return
old = 'L46ebd4:\n'
assert s.count(old) == 1, 'exit'
s = s.replace(old, old + 'ulr_dump(mem, sp, ulr_lr, "out");\n')
open(sys.argv[2], 'w').write(s)
