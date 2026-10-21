#!/usr/bin/env python3
"""patch_fsplit.py IN.c OUT.c : add [CDX] fs* debug lines inside f_split of the traced uopt (diagnostic only)."""
import sys
s = open(sys.argv[1]).read()
def rep(old, new):
    global s
    assert s.count(old) == 1, (old, s.count(old))
    s = s.replace(old, new, 1)
LOG = 'DKWB_CDX_LOG(dkwb_cdx_globalcolor_ordinal, %s);'
# seed search result
rep("L46fbbc:\nif (a0 != 0) {\n//nop;\ngoto L46fc34;}",
    "L46fbbc:\n{ int dkwb_pw = (int)MEM_U32(MEM_U32(s2 + 0) + 4); " + (LOG % '"[CDX] fs_seedsearch parentbit=%d found=%d\\n", dkwb_pw, (int)(a0 != 0)') + " }\nif (a0 != 0) {\n//nop;\ngoto L46fc34;}")
rep("L46fc34:\n", "L46fc34:\n{ uint32_t dkwb_b = MEM_U32(s0 + 0); " + (LOG % '"[CDX] fs_seed parentbit=%d seedbb=%d occ16=%d occ18=%d occ19=%d occ20=%d\\n", (int)MEM_U32(MEM_U32(s2 + 0) + 4), (int)MEM_U16(dkwb_b + 8), (int)MEM_U16(s0 + 16), (int)MEM_U8(s0 + 18), (int)MEM_U8(s0 + 19), (int)MEM_U8(s0 + 20)') + " }\n")
rep("L46ff40:\nt8 = MEM_U32(s2 + 0);", "L46ff40:\n{ " + (LOG % '"[CDX] fs_emptychild parentbit=%d\\n", (int)MEM_U32(MEM_U32(s2 + 0) + 4)') + " }\nt8 = MEM_U32(s2 + 0);")
rep("L4700a0:\nt9 = 0x1001eaf8;", "L4700a0:\n{ " + (LOG % '"[CDX] fs_child parentbit=%d childbit=%d\\n", (int)MEM_U32(MEM_U32(s2 + 0) + 4), (int)MEM_U32(MEM_U32(s1 + 0) + 4)') + " }\nt9 = 0x1001eaf8;")

# growth loop tracing
def LG(fmt, args): return 'DKWB_CDX_LOG(dkwb_cdx_globalcolor_ordinal, "%s", %s);' % (fmt, args)
rep("L46fdc0:\n// bdead 400c000b gp = MEM_U32(sp + 32);\nt2 = MEM_U8(v0 + 23);\nt0 = 0x100226ac;\ns0 = 0x100226a8;\nif (t2 != 0) {\n//nop;\ngoto L46fe70;}",
    "L46fdc0:\n// bdead 400c000b gp = MEM_U32(sp + 32);\nt2 = MEM_U8(v0 + 23);\nt0 = 0x100226ac;\ns0 = 0x100226a8;\n{ uint32_t dkwb_bl = MEM_U32(MEM_U32(0x100226a8) + (MEM_U32(0x100226ac) << 2)); " + LG("[CDX] fs_grow idx=%d bb=%d flag23=%d cupchk=%d\\n", "(int)MEM_U32(0x100226ac), (int)MEM_U16(dkwb_bl + 8), (int)t2, (int)MEM_U8(sp + 127)") + " }\nif (t2 != 0) {\n//nop;\ngoto L46fe70;}")
rep("v0 = f_is_cup_affecting_regs(mem, sp, a0);\ngoto L46fe08;\n//nop;\nL46fe08:\n// bdead 400e000b gp = MEM_U32(sp + 32);\n//nop;\nt0 = 0x100226ac;\nif (v0 != 0) {",
    "v0 = f_is_cup_affecting_regs(mem, sp, a0);\ngoto L46fe08;\n//nop;\nL46fe08:\n// bdead 400e000b gp = MEM_U32(sp + 32);\n//nop;\nt0 = 0x100226ac;\n" + LG("[CDX] fs_cup result=%d\\n", "(int)v0") + "\nif (v0 != 0) {")
rep("a3 = MEM_U32(sp + 120);\nt1 = MEM_U8(t9 + 0);\n//nop;\nif (t1 == at) {",
    "a3 = MEM_U32(sp + 120);\nt1 = MEM_U8(t9 + 0);\n//nop;\n" + LG("[CDX] fs_op op=%d\\n", "(int)t1") + "\nif (t1 == at) {")

rep("t2 = t6 ^ t5;\nt7 = t4 ^ t8;\nt1 = t2 | t7;\nif (t1 != 0) {\nv1 = s0;\ngoto L46fb84;}",
    "t2 = t6 ^ t5;\nt7 = t4 ^ t8;\nt1 = t2 | t7;\n{ uint32_t dkwb_b2 = MEM_U32(s0 + 0); " + LG("[CDX] fs_occ parentbit=%d bb=%d fullmask=%d occ16=%d occ18=%d occ19=%d occ20=%d\\n", "(int)MEM_U32(MEM_U32(s2 + 0) + 4), (int)MEM_U16(dkwb_b2 + 8), (int)(t1 == 0), (int)MEM_U16(s0 + 16), (int)MEM_U8(s0 + 18), (int)MEM_U8(s0 + 19), (int)MEM_U8(s0 + 20)") + " }\nif (t1 != 0) {\nv1 = s0;\ngoto L46fb84;}")

rep("L46fc34:\n{ uint32_t dkwb_b = MEM_U32(s0 + 0);",
    "L46fc34:\n{ uint32_t dkwb_o; int dkwb_n = 0; for (dkwb_o = MEM_U32(MEM_U32(s1 + 0) + 8); dkwb_o && dkwb_n < 80; dkwb_o = MEM_U32(dkwb_o + 4), dkwb_n++) { " + LG("[CDX] fs_list parentbit=%d bb=%d occ16=%d occ18=%d occ19=%d occ20=%d\\n", "(int)MEM_U32(MEM_U32(s2 + 0) + 4), (int)MEM_U16(MEM_U32(dkwb_o + 0) + 8), (int)MEM_U16(dkwb_o + 16), (int)MEM_U8(dkwb_o + 18), (int)MEM_U8(dkwb_o + 19), (int)MEM_U8(dkwb_o + 20)") + " } }\n{ uint32_t dkwb_b = MEM_U32(s0 + 0);")

rep("L460280:\n//insertlda:\n//nop;\n//nop;\n//nop;\nsp = sp + 0xffffffb8;",
    "L460280:\n//insertlda:\n//nop;\n//nop;\n//nop;\nsp = sp + 0xffffffb8;\n{ static FILE *fsf; if (!fsf) fsf = fopen(\"/home/cburnes/rush2049/scratch/frontier/w36a/dbg/lda.txt\", \"a\"); if (fsf) { fprintf(fsf, \"insertlda a0=%08x a1=%08x a2=%08x\\\\n\", (unsigned)a0, (unsigned)a1, (unsigned)a2); fflush(fsf);} }")
rep('#include "header.h"\n', '#include "header.h"\nstatic FILE *dkwb_cdx_output;\nstatic int dkwb_cdx_log;\n')

open(sys.argv[2], 'w').write(s)
