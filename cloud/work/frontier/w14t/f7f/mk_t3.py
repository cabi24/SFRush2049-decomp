src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/f7f/s0/v0000.c').read()
a0 = src.index("  if (D_8014A110 == 4) {")
a1 = src.index("  } else if (D_8014A110 == 6) {")
pre, arm1, post = src[:a0], src[a0:a1], src[a1:]
def A(a,b,n=1):
    global arm1
    assert arm1.count(a)==n, (arm1.count(a), a[:60])
    arm1=arm1.replace(a,b)
A("    s16 ia; s16 ib; s16 f; s16 c;",
  "    s16 ia; s16 ib; s16 f; s16 c;\n    /*@{lim*/ /*@| s16 lim; @}*/\n    /*@{lim*/ /*@| lim = D_8014A108 - 1; @}*/")
# inside arm1 sort loops: replace the bound with lim (linked) or the original
A("for (j = 0; j < D_8014A108 - 1; j++) {",
  "for (j = 0; j < /*@{lim*/D_8014A108 - 1/*@| lim @}*/; j++) {")
A("for (i = 0; i < D_8014A108 - 1; i++) {",
  "for (i = 0; i < /*@{lim*/D_8014A108 - 1/*@| lim @}*/; i++) {")
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/f7f/t3.c','w').write(pre+arm1+post)
print('ok')
