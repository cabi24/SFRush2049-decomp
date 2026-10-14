# patch_acc.py IN OUT: on top of w16d patch_spl.py output, add a force-ACCEPT oracle to f_addadjacents:
# SPL_ACC=bb,... (with optional SPL_SYM=sym) adds those blocks to a split piece of that symbol, bypassing the
# acceptance test `shared < old_regsleft && 2*regsleft >= numintf + shared` (W5D_PROC-selected procedure only).
import sys
s = open(sys.argv[1]).read()
anchor = 'L46e2e0:\n'
assert s.count(anchor) == 1
hook = ('if (getenv("SPL_ACC") && w5d_selected(mem) && (!getenv("SPL_SYM") || (unsigned)atoi(getenv("SPL_SYM")) == '
        'MEM_U16(MEM_U32(s5 + 0) + 2))) { char b_[16], t_[512]; snprintf(b_, sizeof b_, ",%u,", MEM_U16(MEM_U32(s1 + 0) + 8)); '
        'snprintf(t_, sizeof t_, ",%s,", getenv("SPL_ACC")); if (strstr(t_, b_)) { if (getenv("SPL")) fprintf(stderr, "[SPL]   force-accept bb=%u\\n", '
        'MEM_U16(MEM_U32(s1 + 0) + 8)); t1 = MEM_U32(sp + 92); goto L46e34c; } }\n')
s = s.replace(anchor, anchor + hook)
open(sys.argv[2], 'w').write(s)
