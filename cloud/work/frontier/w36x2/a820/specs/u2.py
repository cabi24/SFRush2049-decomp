BASE = 'work/usc_best'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;"
I = "    s16 idx;\n"
VARIANTS = {}
for nu in (12, 11, 10, 9):
    us = "    s32 " + ", ".join("u%d" % k for k in range(nu)) + ";"
    VARIANTS['i32_u%d' % nu] = [(I, "    s32 idx;\n"), (U, us)]
