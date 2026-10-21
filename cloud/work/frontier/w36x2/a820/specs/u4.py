BASE = 'work/usc_sc'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
VARIANTS = {'sc_u12': []}
for nu in (11, 10, 9, 8):
    VARIANTS['sc_u%d' % nu] = [(U, "    s32 " + ", ".join("u%d" % k for k in range(nu)) + ";\n")]
