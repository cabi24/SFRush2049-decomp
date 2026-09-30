import sys,re
sys.path.insert(0,'.')
import gen7g,harness
base=gen7g.gen(dict(sty='idx'))
# variant: only tie loop of mode 4 modified
def variant(tie_new, extra_decl=''):
    s=base
    old=gen7g.tie('K4')
    assert old in s
    s=s.replace(old,tie_new)
    s=s.replace("void func_800F7F3C(void)\n{\n","void func_800F7F3C(void)\n{\n"+extra_decl)
    return s
V={}
V['ptrs']=("""    for (i = 1; i < D_8014A108; i++) {
      if (K4(IDX(D_80143F54[0])) == K4(IDX(D_80143F54[i]))) {
        fl[i] = 1;
        (*cn)++;
      }
    }
""","  s8 *fl = D_80150B68;\n  s32 *cn = &D_80150B60;\n")
V['cnt_ptr_only']=("""    for (i = 1; i < D_8014A108; i++) {
      if (K4(IDX(D_80143F54[0])) == K4(IDX(D_80143F54[i]))) {
        D_80150B68[i] = 1;
        (*cn)++;
      }
    }
""","  s32 *cn = &D_80150B60;\n")
V['keyptr']=("""    for (i = 1; i < D_8014A108; i++) {
      if (*kp(IDX(D_80143F54[0])) == *kp(IDX(D_80143F54[i]))) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
""","")
for k,(t,d) in V.items():
    s=variant(t,d)
    if k=='keyptr': s=s.replace("#define IDX","#define kp(i) ((s32 *)(D_80152038 + (i) * 120 + 20))\n#define IDX")
    open('tmp.c','w').write(s)
    print(k,harness.evaluate('tmp.c','func_800F7F3C'))
