import re,sys
def invert(src, cond):
    a=src.index("            if (curve->level[0] * scale <= magnitude) {\n")
    b=src.index("            } else if (car->level[i] != 0.0f) {\n")
    c=src.index("        }\n    }\n}\n", b)
    then=src[a+len("            if (curve->level[0] * scale <= magnitude) {\n"):b]
    els=src[b+len("            } else if (car->level[i] != 0.0f) {\n"):c]
    # els ends with "            }\n"
    assert els.endswith("            }\n")
    els=els[:-len("            }\n")]
    new=("            if (%s) {\n                if (car->level[i] != 0.0f) {\n" % cond +
         ''.join('    '+l+'\n' if l else '\n' for l in els.rstrip('\n').split('\n')) +
         "                }\n            } else {\n" + then + "            }\n")
    return src[:a]+new+src[c:]
for name in ['../v2/a_lo.c','../v3/hi_cv.c','../base.c','../v3/lohi_lo.c']:
    s=open('v5/'+name).read() if False else open(name.replace('../','')).read()
    for cn,cond in [('gt','curve->level[0] * scale > magnitude'),('nle','!(curve->level[0] * scale <= magnitude)'),('lt','magnitude < curve->level[0] * scale')]:
        open('v5/%s_%s.c'%(name.split('/')[-1][:-2],cn),'w').write(invert(s,cond))
