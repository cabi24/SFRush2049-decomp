# boostpatch.py FILE: oracle for uopt.spl.c -- SPL_BOOST=lr:save[,lr:save] overrides compute_save's priority ("save")
# of those live ranges (by lr id, = CDX web number; W5D_PROC-selected procedure only) so that a split piece uopt would
# drop (save <= 0, flag35=2) stays a p1 candidate. Diagnostic only, never match evidence.
import sys
p = sys.argv[1]; s = open(p).read()
a = 'L46f22c:\nf8.w[0] = MEM_U32(s1 + 48);\n'
assert s.count(a) == 1, s.count(a)
hook = ('L46f22c:\n'
        '{ const char *bv_ = getenv("SPL_BOOST"); if (bv_ && w5d_selected(mem)) { char t_[256]; char *q_; strncpy(t_, bv_, 255); t_[255] = 0;'
        ' for (q_ = strtok(t_, ","); q_; q_ = strtok(0, ",")) { int w_; float v_; if (sscanf(q_, "%d:%f", &w_, &v_) == 2 && w_ == (int)MEM_U32(s1 + 4)) { union { float f; uint32_t w; } u_; u_.f = v_; MEM_U32(s1 + 48) = u_.w; fprintf(stderr, "[SPL] boost lr=%d save=%f\\n", w_, v_); } } } }\n'
        'f8.w[0] = MEM_U32(s1 + 48);\n')
s = s.replace(a, hook)
open(p, 'w').write(s)
