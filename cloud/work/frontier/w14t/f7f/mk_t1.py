src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/f7f/s0/v0000.c').read()
def R(a,b,n=1):
    global src
    assert src.count(a)==n, (src.count(a), a[:70])
    src=src.replace(a,b)
# arm 1 (mode 4) only: split at the else-if
head, rest = src.split("  } else if (D_8014A110 == 6) {",1)
arm1 = head[head.index("  if (D_8014A110 == 4) {"):]
head = head[:head.index("  if (D_8014A110 == 4) {")]
tail = "  } else {" + rest.split("  } else {",1)[1]
arm1o = arm1
def A(a,b,n=1):
    global arm1
    assert arm1.count(a)==n, (arm1.count(a), a[:70])
    arm1=arm1.replace(a,b)
A("    s16 ia; s16 ib; s16 f; s16 c;",
  "    /*@{ty*/s16 ia; s16 ib; s16 f; s16 c;/*@| s32 ia; s32 ib; s16 f; s16 c; @}*/")
A("      for (j = 0; j < D_8014A108 - 1; j++) {",
  "      for (j = 0; /*@{bj*/j < D_8014A108 - 1/*@| j + 1 < D_8014A108 @}*/; j++) {")
A("        if (D_80152038[ia].key < D_80152038[ib].key) {\n          D_80143F54[j + 1] = a;\n          D_80143F54[j] = b;\n        }",
  "        if (/*@{cmp*/D_80152038[ia].key < D_80152038[ib].key/*@| D_80152038[ib].key > D_80152038[ia].key @}*/) {\n          /*@{sw*/D_80143F54[j + 1] = a;\n          D_80143F54[j] = b;/*@| D_80143F54[j] = b;\n          D_80143F54[j + 1] = a; @}*/\n        }")
A("    for (i = 0; i < D_8014A108 - 1; i++) {\n      for",
  "    for (i = 0; i < D_8014A108 - 1; i++) {\n      /*@{ec*/if (i) {}/*@| @}*/\n      for")
A("        D_80150B60++;",
  "        /*@{inc*/D_80150B60++;/*@| D_80150B60 += 1; @}*/")
A("      if (D_80152038[f].key == D_80152038[c].key) {",
  "      if (/*@{eq*/D_80152038[f].key == D_80152038[c].key/*@| D_80152038[c].key == D_80152038[f].key @}*/) {")
src = head + arm1 + "  } else if (D_8014A110 == 6) {" + rest.split("  } else {",1)[0] + tail
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/f7f/t1.c','w').write(src)
print('ok')
