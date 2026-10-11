# maskpatch.py FILE: after every p1dec record, when env LBB=1 and the procedure is W5D-selected, print the live
# range's livbb list with each block's register-in-use masks (node+44..+60) as
# "[LBB] web=W bb:m44/m48/m52/m56 ...". Diagnostic only (based on w17a patch_lbb.py).
import sys
p = sys.argv[1]; s = open(p).read()
anchor = '"[CDX] p1dec phase=p1 proc=%d'
i = s.index(anchor); j = s.index(';\n', i) + 2
add = ('    if (getenv("LBB") && w5d_selected(mem)) { uint32_t l_ = MEM_U32(s5 + 8); fprintf(stderr, "[LBB] web=%d", dkwb_web); '
       'while (l_) { uint32_t n_ = MEM_U32(l_ + 0); fprintf(stderr, " %u:%08x/%08x/%08x/%08x", MEM_U16(n_ + 8), MEM_U32(n_ + 44), MEM_U32(n_ + 48), MEM_U32(n_ + 52), MEM_U32(n_ + 56)); l_ = MEM_U32(l_ + 4); } fprintf(stderr, "\\n"); }\n')
s = s[:j] + add + s[j:]
open(p, 'w').write(s); print('patched')
