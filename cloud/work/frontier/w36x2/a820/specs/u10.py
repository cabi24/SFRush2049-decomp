BASE = 'work/usc_best'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
P = "void CreateSelectCar(s16 slot, s16 player, s32 cam)"
VARIANTS = {}
for nu in (11, 12, 10):
    us = "    s32 " + ", ".join("u%d" % k for k in range(nu)) + ";\n"
    for nm, sig in (('sss', 's16 slot, s16 player, s16 cam'), ('iis', 's32 slot, s32 player, s32 cam'), ('isi', 's32 slot, s16 player, s32 cam'), ('sii', 's16 slot, s32 player, s32 cam'), ('ssu', 's16 slot, s16 player, u32 cam'), ('ssus', 's16 slot, s16 player, u16 cam')):
        VARIANTS['%s_u%d' % (nm, nu)] = [(U, us), (P, "void CreateSelectCar(" + sig + ")")]
