BASE = 'work/usc_best'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
T = "typedef union Color {u32 word;u8 channel[4];} Color;"
A = "    Color alphaCol = D_803B65B4;\n"
VARIANTS = {}
for nu in (12, 11, 10, 9):
    us = ("    s32 " + ", ".join("u%d" % k for k in range(nu)) + ";\n") if nu else ""
    VARIANTS['init_u%d' % nu] = [(T, "typedef union Color {u8 channel[4];u32 word;} Color;"), (A, "    Color alphaCol = {255, 255, 255, 255};\n"), (U, us)]
