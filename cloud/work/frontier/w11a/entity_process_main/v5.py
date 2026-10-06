OLD = """        poly = &D_8015B268[v->objnum];
        i = ~(1 << i);
        poly->flags = i & poly->flags;"""
P = "        poly = &D_8015B268[v->objnum];\n        "
L = ["poly->flags", "(u32)poly->flags", "(s32)poly->flags", "(u16)poly->flags"]
R = ["~(1 << i)", "(u32)~(1 << i)", "~(u32)(1 << i)", "~(1 << (u32)i)", "~((u32)1 << i)", "(s32)~(1 << i)"]
V = {}
n = 0
for a in L:
    for b in R:
        V['c%02d' % n] = P + "poly->flags = %s & %s;" % (a, b); n += 1
        V['c%02d' % n] = P + "poly->flags = %s & %s;" % (b, a); n += 1
