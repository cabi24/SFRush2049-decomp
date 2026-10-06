for basen,fn in [('ptr','base.c'),('idx','v3/hi_cv.c')]:
    src=open(fn).read()
    if basen=='ptr':
        blk="""                lo = curve->level;
                hi = lo[4];
                if (hi < sample) {
                    sample = hi;
                }
"""
        new_in="                lo = curve->level;\n"
        clamp="""            hi = curve->level[4];
            if (hi < sample) {
                sample = hi;
            }
"""
    else:
        blk="""                hi = curve->level[4];
                if (hi < sample) {
                    sample = hi;
                }
"""
        new_in=""
        clamp="""            hi = curve->level[4];
            if (hi < sample) {
                sample = hi;
            }
"""
    assert blk in src
    s=src.replace(blk,new_in)
    for an,anchor in [('m',"            magnitude = sample;\n"),('s',"            if (D_8014A110 == 6) {\n"),('c',"            if (curve->level[0] * scale <= magnitude) {\n")]:
        if an=='m':
            t=s.replace(anchor, anchor+clamp)
        else:
            t=s.replace(anchor, clamp+anchor)
        open('v9/%s_%s.c'%(basen,an),'w').write(t)
