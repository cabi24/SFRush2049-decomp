# prio_patch.py FILE: add a p1 priority-scale oracle to uopt.trace.c (apply after w16d patch_spl.py).
# env PRIO="web:mul,web:mul" (W5D_PROC-selected procedure only) multiplies the p1 selection priority of those
# webs, so a web can be decided earlier/later without changing its uses. Diagnostic only.
import sys
p = sys.argv[1]; s = open(p).read()
a = 'f0.w[0] = MEM_U32(s5 + 48);\nDKWB_CDX_LOG(dkwb_cdx_ordinal, "[CDX] p1cand'
assert s.count(a) == 1
hook = ('f0.w[0] = MEM_U32(s5 + 48);\n'
        'if (getenv("PRIO") && w5d_selected(mem)) { char t_[512]; char *q_; snprintf(t_, sizeof t_, "%s", getenv("PRIO"));'
        ' for (q_ = strtok(t_, ","); q_; q_ = strtok(0, ",")) { int w_ = atoi(q_); char *c_ = strchr(q_, \':\');'
        ' if (c_ && w_ == (int)MEM_U32(sp + 272)) f0.f[0] = f0.f[0] * (float)atof(c_ + 1); } }\n'
        'DKWB_CDX_LOG(dkwb_cdx_ordinal, "[CDX] p1cand')
s = s.replace(a, hook)
open(p, 'w').write(s)
# compute_save logging (env SAVELOG=1, W5D_PROC procedure): per livbb bb/uses/defs/weight/flags, then card, nocs, tot, save
s = open(p).read()
a1 = 'L46f03c:\nt6 = MEM_U16(s0 + 16);\n'
assert s.count(a1) == 1
s = s.replace(a1, 'L46f03c:\nif (getenv("SAVELOG") && w5d_selected(mem)) fprintf(stderr, "[SAVE] lr=%u bb=%u u=%u d=%u w=%u f18=%u f21=%u f22=%u f23=%u lr34=%u\\n", MEM_U32(s1 + 4), MEM_U16(MEM_U32(s0 + 0) + 8), MEM_U16(s0 + 16), MEM_U8(s0 + 18), MEM_U32(MEM_U32(s0 + 0) + 44), MEM_U8(s0 + 18), MEM_U8(s0 + 21), MEM_U8(s0 + 22), MEM_U8(s0 + 23), MEM_U8(s1 + 34));\nt6 = MEM_U16(s0 + 16);\n')
a2 = 'L46f22c:\nf8.w[0] = MEM_U32(s1 + 48);\n'
assert s.count(a2) == 1
s = s.replace(a2, 'L46f22c:\nif (getenv("SAVELOG") && w5d_selected(mem)) fprintf(stderr, "[SAVE] lr=%u card=%u nocs=%u tot=%f save=%f\\n", MEM_U32(s1 + 4), v0, MEM_U32(s1 + 28), (double)f22.f[0], (double)f6.f[0]);\nif (getenv("SAVELOG") && w5d_selected(mem)) { int b_; fprintf(stderr, "[SAVEV] lr=%u:", MEM_U32(s1 + 4)); for (b_ = 0; b_ < 400; b_++) if (f_bvectin(mem, sp, b_, s1 + 0xc)) fprintf(stderr, " %d", b_); fprintf(stderr, "\\n"); }\nf8.w[0] = MEM_U32(s1 + 48);\n')
open(p, 'w').write(s)
