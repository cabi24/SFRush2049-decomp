src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/f7f/s0/v0000.c').read()
a0 = src.index("  if (D_8014A110 == 4) {")
a1 = src.index("  } else if (D_8014A110 == 6) {")
pre, arm1, post = src[:a0], src[a0:a1], src[a1:]
def A(a,b,n=1):
    global arm1
    assert arm1.count(a)==n, (arm1.count(a), a[:60])
    arm1=arm1.replace(a,b)
A("    s16 ia; s16 ib; s16 f; s16 c;",
  "    /*@{fi*/s16 ia; s16 ib; s16 f; /*@| s16 ia; s16 ib; @}*/ /*@{ci*/s16 c;/*@| @}*/")
A("        ia = D_8014A250[a].idx;\n        ib = D_8014A250[b].idx;\n        if (D_80152038[ia].key < D_80152038[ib].key) {",
  "        /*@{ii*/ia = D_8014A250[a].idx;\n        ib = D_8014A250[b].idx;/*@| @}*/\n        if (/*@{ii2*/D_80152038[ia].key < D_80152038[ib].key/*@| D_80152038[D_8014A250[a].idx].key < D_80152038[D_8014A250[b].idx].key @}*/) {")
A("      f = D_8014A250[D_80143F54[0]].idx;\n      c = D_8014A250[D_80143F54[i]].idx;\n      if (D_80152038[f].key == D_80152038[c].key) {",
  "      /*@{fs*/f = D_8014A250[D_80143F54[0]].idx;/*@| @}*/\n      /*@{cs*/c = D_8014A250[D_80143F54[i]].idx;/*@| @}*/\n      if (/*@{eq*/D_80152038[f].key == D_80152038[c].key/*@| D_80152038[D_8014A250[D_80143F54[0]].idx].key == D_80152038[D_8014A250[D_80143F54[i]].idx].key @}*/) {")
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/f7f/t2.c','w').write(pre+arm1+post)
print('ok')
