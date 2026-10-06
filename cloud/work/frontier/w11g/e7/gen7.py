import glob
C="""                if (hi < sample) {
                    sample = hi;
                }
"""
for f in ['v3/hi_cv.c','v3/lohi_lo.c','v2/a_lo.c','base.c']:
    s=open(f).read(); assert C in s
    n=f.split('/')[-1][:-2]
    open('v7/%s_tern.c'%n,'w').write(s.replace(C,"                sample = (hi < sample) ? hi : sample;\n"))
    open('v7/%s_tern2.c'%n,'w').write(s.replace(C,"                sample = (sample > hi) ? hi : sample;\n"))
    open('v7/%s_gt.c'%n,'w').write(s.replace(C,"                if (sample > hi) {\n                    sample = hi;\n                }\n"))
