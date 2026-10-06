OLD = """        poly = &D_8015B268[v->objnum];
        i = ~(1 << i);
        poly->flags = i & poly->flags;"""
P = "        poly = &D_8015B268[v->objnum];\n        "
forms = {
 'z1': "xlu = poly->flags; poly->flags = xlu & ~(1 << i);",
 'z2': "xlu = 1 << i; poly->flags = ~xlu & poly->flags;",
 'z3': "xlu = 1 << i; poly->flags &= ~xlu;",
 'z4': "xlu = ~(1 << i); poly->flags = xlu & poly->flags;",
 'z5': "poly->flags = (-1 - (1 << i)) & poly->flags;",
 'z6': "poly->flags = poly->flags & (-1 - (1 << i));",
 'z7': "xlu = poly->flags; poly->flags = ~(1 << i) & xlu;",
 'z9': "poly->flags = ~(1 << i) & poly->flags & 0xFFFF;",
 'za': "poly->flags = ~(1 << i) & 0xFFFF & poly->flags;",
 'zb': "poly->flags = (~(1 << i) & 0xFFFF) & poly->flags;",
 'zc': "poly->flags = ~(1 << i) & (poly->flags & 0xFFFF);",
}
V = {k: P + f for k, f in forms.items()}
V['z8'] = "        i = 1 << i;\n" + P + "poly->flags &= ~i;"
V['zd'] = "        xlu = 1 << i;\n" + P + "poly->flags = ~xlu & poly->flags;"
