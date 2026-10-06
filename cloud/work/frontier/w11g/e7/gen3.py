src=open('base.c').read()
OLD=src[src.index("                lo = curve->level;\n"):src.index("                car->level[i] = f;\n")]
F_lo="""                f = ((f32)(sample - lo[segment]) / (f32)(lo[segment + 1] - lo[segment]) + segment) * 0.25f;\n"""
F_cv="""                f = ((f32)(sample - curve->level[segment]) / (f32)(curve->level[segment + 1] - curve->level[segment]) + segment) * 0.25f;\n"""
V={}
for pre_name,pre in [('lohi',"                lo = curve->level;\n                hi = lo[4];\n"),
                     ('hilo',"                hi = curve->level[4];\n                lo = curve->level;\n"),
                     ('hi',"                hi = curve->level[4];\n"),
                     ('dseg',"                segment = 0;\n                lo = curve->level;\n                hi = lo[4];\n")]:
    for loopv,cond in [('lo','lo'),('cv','curve->level')]:
        if loopv=='lo' and pre_name=='hi': continue
        body=pre+"""                if (hi < sample) {
                    sample = hi;
                }
                for (segment = 0; segment < 4; segment++) {
                    if (%s[segment + 1] >= sample) {
                        break;
                    }
                }
""" % cond + (F_lo if loopv=='lo' else F_cv)
        V[pre_name+'_'+loopv]=body
for k,v in V.items():
    open('v3/%s.c'%k,'w').write(src.replace(OLD,v))
