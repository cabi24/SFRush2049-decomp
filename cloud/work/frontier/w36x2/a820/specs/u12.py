BASE = 'work/usc_best'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
U11 = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;\n"
S = "    switch (player) {\n"
VARIANTS = {}
for nm, e in (('cast', '(s32)player'), ('plus0', 'player + 0'), ('u16', '(u16)player'), ('and', 'player & 0xFFFF')):
    VARIANTS[nm] = [(U, U11), (S, "    switch (%s) {\n" % e)]
    VARIANTS[nm + '_12'] = [(S, "    switch (%s) {\n" % e)]
