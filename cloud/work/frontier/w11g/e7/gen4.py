src=open('base.c').read()
OLD=src[src.index("                lo = curve->level;\n"):src.index("                car->level[i] = f;\n")]
F_cv="""                f = ((f32)(sample - curve->level[segment]) / (f32)(curve->level[segment + 1] - curve->level[segment]) + segment) * 0.25f;\n"""
F_lo="""                f = ((f32)(sample - lo[segment]) / (f32)(lo[segment + 1] - lo[segment]) + segment) * 0.25f;\n"""
V={}
pres={'s_h':"                segment = 0;\n                hi = curve->level[4];\n",
      'h_s':"                hi = curve->level[4];\n                segment = 0;\n",
      's_h_l':"                segment = 0;\n                hi = curve->level[4];\n                lo = curve->level;\n",
      'h_s_l':"                hi = curve->level[4];\n                segment = 0;\n                lo = curve->level;\n",
      's_l_h':"                segment = 0;\n                lo = curve->level;\n                hi = lo[4];\n",
}
for pn,pre in pres.items():
  for fl in ['for','while']:
    for idx in ['cv','lo']:
        if idx=='lo' and '_l' not in pn: continue
        base = 'curve->level' if idx=='cv' else 'lo'
        if fl=='for':
            loop="""                for (; segment < 4; segment++) {
                    if (%s[segment + 1] >= sample) {
                        break;
                    }
                }
""" % base
        else:
            loop="""                while (segment < 4 && %s[segment + 1] < sample) {
                    segment++;
                }
""" % base
        body=pre+"""                if (hi < sample) {
                    sample = hi;
                }
"""+loop+(F_cv if idx=='cv' else F_lo)
        open('v4/%s_%s_%s.c'%(pn,fl,idx),'w').write(src.replace(OLD,body))
