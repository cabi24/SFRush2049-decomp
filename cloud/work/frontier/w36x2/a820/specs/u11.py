BASE = 'work/usc_best'
E = "extern LoadState D_803BA2F0[];"
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
U11 = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;\n"
VARIANTS = {
 'def52': [(E, "LoadState D_803BA2F0[52];")],
 'def52_u11': [(E, "LoadState D_803BA2F0[52];"), (U, U11)],
 'sdef52_u11': [(E, "static LoadState D_803BA2F0[52];"), (U, U11)],
}
