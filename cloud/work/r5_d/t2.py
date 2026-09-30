import sys,re,itertools
sys.path.insert(0,'.')
import gen7g,harness
base=gen7g.gen(dict(sty='idx'))
def mk(K,vt):
    return f"""    f = IDX(D_80143F54[0]);
    for (i = 1; i < D_8014A108; i++) {{
      if ({K}(f) == {K}(IDX(D_80143F54[i]))) {{
        D_80150B68[i] = 1;
        D_80150B60++;
      }}
    }}
"""
s=base
for K in ['K4','K6','K0']:
    s=s.replace(gen7g.tie(K),mk(K,'s16'))
for ft in ['s16','s32']:
    t=s.replace("void func_800F7F3C(void)\n{\n","void func_800F7F3C(void)\n{\n  %s f;\n"%ft)
    open('v10_%s.c'%ft,'w').write(t)
    print(ft,harness.evaluate('v10_%s.c'%ft,'func_800F7F3C'))
