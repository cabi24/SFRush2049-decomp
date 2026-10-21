# patch_acc.py IN OUT (w36z): on top of w36m patch_spl2.py output (uopt.spl2.c), add an ACCEPT oracle:
# SPL_ACC=bb,... (with optional SPL_SYM=sym) forces f_addadjacents to ADD those blocks to a split piece of that symbol.
import sys
s = open(sys.argv[1]).read()
a = 'L46e2e0:\n'
assert s.count(a) == 1
s = s.replace(a, a + 'if (getenv("SPL_ACC") && w5d_selected(mem) && (!getenv("SPL_SYM") || (unsigned)atoi(getenv("SPL_SYM")) == MEM_U16(MEM_U32(s5 + 0) + 2))) { char b_[16], t_[512]; snprintf(b_, sizeof b_, ",%u,", MEM_U16(MEM_U32(s1 + 0) + 8)); snprintf(t_, sizeof t_, ",%s,", getenv("SPL_ACC")); if (strstr(t_, b_)) { if (getenv("SPL")) fprintf(stderr, "[SPL]   forced-accept bb=%u\\n", MEM_U16(MEM_U32(s1 + 0) + 8)); t1 = MEM_U32(sp + 92); goto L46e34c; } }\n', 1)
open(sys.argv[2], 'w').write(s)
# SPL_DEC=n (with SPL_SYM): evaluate the growth test with numintf-n for that symbol's pieces (one fewer interferer oracle)
s = open(sys.argv[2]).read()
s = s.replace(a, a + 'if (getenv("SPL_DEC") && w5d_selected(mem) && (!getenv("SPL_SYM") || (unsigned)atoi(getenv("SPL_SYM")) == MEM_U16(MEM_U32(s5 + 0) + 2))) { int d_ = atoi(getenv("SPL_DEC")); if ((int)s6 < (int)s4 && 2 * (int)MEM_U8(s2 + 33) >= (int)MEM_U32(s2 + 36) - d_ + (int)s6 && MEM_U8(0x1001eb10)) { if (getenv("SPL")) fprintf(stderr, "[SPL]   dec-accept bb=%u\\n", MEM_U16(MEM_U32(s1 + 0) + 8)); t1 = MEM_U32(sp + 92); goto L46e34c; } }\n', 1)
open(sys.argv[2], 'w').write(s)
# SPLFS=1: trace f_findsharedintf (every node: node addr, lr id, sym, colour byte, counted flag before)
s = open(sys.argv[2]).read()
b = 'L46e070:\n'
assert s.count(b) == 1
s = s.replace(b, b + 'if (getenv("SPLFS") && w5d_selected(mem)) { uint32_t l_ = MEM_U32(s0 + 0); fprintf(stderr, "[SPLFS] bb=%u node=%08x lr=%u sym=%u col=%d cnt=%u in=%u\\n", s2, s0, l_ ? MEM_U32(l_ + 4) : 0, l_ ? MEM_U16(MEM_U32(l_ + 0) + 2) : 0, l_ ? (int)MEM_S8(l_ + 32) : 99, MEM_U8(s0 + 8), l_ ? f_bvectin(mem, sp, s2, l_ + 0x14) : 0); }\n', 1)
open(sys.argv[2], 'w').write(s)
