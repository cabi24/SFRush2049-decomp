# patch_lbb.py FILE: after every p1dec record, print the live range's livbb list (env LBB=1, W5D_PROC procedure)
# as "[LBB] web=W bbs=b1,b2,..." (block numbers, uopt numbering). Diagnostic only.
import sys
p = sys.argv[1]; s = open(p).read()
anchor = '"[CDX] p1dec phase=p1 proc=%d'
i = s.index(anchor); j = s.index(';\n', i) + 2
add = ('    if (getenv("LBB") && w5d_selected(mem)) { uint32_t l_ = MEM_U32(s5 + 8); fprintf(stderr, "[LBB] web=%d bbs=", dkwb_web); '
       'while (l_) { fprintf(stderr, "%u,", MEM_U16(MEM_U32(l_ + 0) + 8)); l_ = MEM_U32(l_ + 4); } fprintf(stderr, "\\n"); }\n')
s = s[:j] + add + s[j:]
open(p, 'w').write(s); print('patched')
