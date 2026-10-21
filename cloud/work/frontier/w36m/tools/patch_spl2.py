# patch_spl.py IN OUT: add [SPL] prints (env SPL=1, W5D_PROC-selected procedure) to uopt.trace.c's f_split/addadjacents
import sys
s = open(sys.argv[1]).read()
def ins(anchor, text, after=True):
    global s
    assert s.count(anchor) == 1, anchor
    s = s.replace(anchor, anchor + text if after else text + anchor)
G = 'if (getenv("SPL") && w5d_selected(mem)) '
# start block chosen (L46fc34: s0 = livbb of the start block, s2 = &old lr, s1 = &new lr)
ins('L46fc34:\n', G + '{ uint32_t b_ = MEM_U32(s0 + 0); fprintf(stderr, "[SPL] split lr=%u sym=%u start bb=%u\\n", MEM_U32(MEM_U32(s2 + 0) + 4), MEM_U16(MEM_U32(MEM_U32(s2 + 0) + 0) + 2), MEM_U16(b_ + 8)); }\n')
# BFS: each worklist block before expansion
ins('L46fdc0:\n', G + '{ uint32_t e_ = MEM_U32(MEM_U32(0x100226a8) + (MEM_U32(0x100226ac) << 2)); fprintf(stderr, "[SPL]  visit bb=%u f23=%u cup=%u op=%u\\n", MEM_U16(e_ + 8), MEM_U8(v0 + 23), f_is_cup_affecting_regs(mem, sp, e_), MEM_U8(MEM_U32(e_ + 32))); }\n')
# addadjacents: after updatecolorsleft, show candidate and regsleft
ins('L46e2e0:\n', G + '{ fprintf(stderr, "[SPL]   adj bb=%u regsleft=%u old=%u shared=%d numintf=%u f0=%08x f1=%08x\\n", MEM_U16(MEM_U32(s1 + 0) + 8), MEM_U8(s2 + 33), s4, (int)s6, MEM_U32(s2 + 36), MEM_U32(s2 + 40), MEM_U32(s2 + 44)); }\n')
ins('L46e34c:\n', G + '{ fprintf(stderr, "[SPL]   add\\n"); }\n')
ins('L46e47c:\n', G + '{ fprintf(stderr, "[SPL]   reject\\n"); }\n')
# at f_split entry (after the new lr is allocated, L46f980): dump the old lr's livbb list
ins('L46f980:\n', G + '{ uint32_t l_ = MEM_U32(MEM_U32(s2 + 0) + 8); fprintf(stderr, "[SPL] list lr=%u:", MEM_U32(MEM_U32(s2 + 0) + 4)); while (l_) { uint32_t b_ = MEM_U32(l_ + 0); fprintf(stderr, " %u[o%u,%u f19=%u f20=%u f23=%u fb=%08x/%08x]", MEM_U16(b_ + 8), MEM_U16(l_ + 16), MEM_U8(l_ + 18), MEM_U8(l_ + 19), MEM_U8(l_ + 20), MEM_U8(l_ + 23), MEM_U32(b_ + 44 + 8 * MEM_U32(sp + 120)), MEM_U32(b_ + 48 + 8 * MEM_U32(sp + 120))); l_ = MEM_U32(l_ + 4); } fprintf(stderr, "\\n"); }\n')
# oracle: SPL_SKIP=a,b,c makes the second start-block search skip those bb numbers (W5D_PROC procedure only)
ins('L46fb84:\n', 'if (spl_skip(mem, MEM_U16(MEM_U32(s0 + 0) + 8))) goto L46fba4;\n')
ins('static void f_split(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {\n', '', after=False)
s = s.replace('static void f_split(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {\nconst', 'static int spl_skip(uint8_t *mem, unsigned bb) { const char *v = getenv("SPL_SKIP"); char b[16]; if (!v || !w5d_selected(mem)) return 0; snprintf(b, sizeof b, ",%u,", bb); { char t[512]; snprintf(t, sizeof t, ",%s,", v); return strstr(t, b) != 0; } }\nstatic void f_split(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {\nconst', 1)
# trace livbb creation order (formlivbb entry): SPLF=1
ins('L4641cc:\n', 'if (getenv("SPLF") && w5d_selected(mem)) fprintf(stderr, "[SPLF] formlivbb sym=%u bb=%u\\n", MEM_U16(a0 + 2), MEM_U16(a1 + 8));\n')
# oracle: SPL_REJ=bb,... with SPL_SYM=sym rejects adding those blocks to a split piece of that symbol
ins('L46e2e0:\n', 'if (getenv("SPL_REJ") && w5d_selected(mem) && (!getenv("SPL_SYM") || (unsigned)atoi(getenv("SPL_SYM")) == MEM_U16(MEM_U32(s5 + 0) + 2))) { char b_[16], t_[512]; snprintf(b_, sizeof b_, ",%u,", MEM_U16(MEM_U32(s1 + 0) + 8)); snprintf(t_, sizeof t_, ",%s,", getenv("SPL_REJ")); if (strstr(t_, b_)) { t1 = MEM_U32(sp + 92); goto L46e47c; } }\n')

# w36m: dump the old lr's interference list at each adjacency decision (SPLI=1): node lr id/sym, counted (f8), shared-here (f9)
ins('L46e2e0:\n', 'if (getenv("SPLI") && w5d_selected(mem)) { uint32_t n_ = MEM_U32(s5 + 56); fprintf(stderr, "[SPLI] bb=%u:", MEM_U16(MEM_U32(s1 + 0) + 8)); while (n_) { uint32_t l_ = MEM_U32(n_ + 0); fprintf(stderr, " %u/%u%s%s", l_ ? MEM_U32(l_ + 4) : 0, l_ ? MEM_U16(MEM_U32(l_ + 0) + 2) : 0, MEM_U8(n_ + 8) ? "*" : "", MEM_U8(n_ + 9) ? "+" : ""); n_ = MEM_U32(n_ + 4); } fprintf(stderr, "\\n"); }\n')
open(sys.argv[2], 'w').write(s)
