# patch_fbd.py FILE: after every p1dec record (env FBD=1, W5D_PROC procedure) print each livbb block of the
# decided live range with its per-block register masks (b+44+8*class, b+48+8*class for class 1..2):
# "[FBD] web=W bb=N m0=... m1=..." and at every updateforbidden call "[UPF] lr=W bb=N excl=C cls=K mask=..".
# Diagnostic only.
import sys
p = sys.argv[1]; s = open(p).read()
anchor = '"[CDX] p1dec phase=p1 proc=%d'
i = s.index(anchor); j = s.index(';\n', i) + 2
add = ('    if (getenv("FBD") && w5d_selected(mem)) { uint32_t l_ = MEM_U32(s5 + 8); while (l_) { uint32_t b_ = MEM_U32(l_ + 0); '
       'fprintf(stderr, "[FBD] web=%d bb=%u c1=%08x/%08x c2=%08x/%08x\\n", dkwb_web, MEM_U16(b_ + 8), MEM_U32(b_ + 52), MEM_U32(b_ + 56), MEM_U32(b_ + 60), MEM_U32(b_ + 64)); '
       'l_ = MEM_U32(l_ + 4); } }\n')
s = s[:j] + add + s[j:]
a = '//updateforbidden:\n'
assert s.count(a) == 1
s = s.replace(a, a + 'if (getenv("UPF") && w5d_selected(mem)) fprintf(stderr, "[UPF] lr=%u bb=%u excl=%u cls=%u m=%08x/%08x\\n", MEM_U32(a2 + 4), MEM_U16(a0 + 8), a1, a3, MEM_U32(a0 + 44 + 8*a3), MEM_U32(a0 + 48 + 8*a3));\n')
open(p, 'w').write(s); print('patched')
