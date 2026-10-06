OLD = """        poly = &D_8015B268[v->objnum];
        i = ~(1 << i);
        poly->flags = i & poly->flags;"""
P = "        poly = &D_8015B268[v->objnum];\n        "
forms = {
 'x1': "poly->flags = ((1 << i) ^ -1) & poly->flags;",
 'x2': "poly->flags = ((1 << i) ^ 0xFFFF) & poly->flags;",
 'x3': "poly->flags = (u16)~(1 << i) & poly->flags;",
 'x4': "poly->flags = (s16)~(1 << i) & poly->flags;",
 'x5': "poly->flags = ~(u32)(1 << i) & poly->flags;",
 'x6': "poly->flags = ~(1U << i) & poly->flags;",
 'x7': "poly->flags = (0xFFFF - (1 << i)) & poly->flags;",
 'x8': "poly->flags = ~(1 << i) & (u32)poly->flags;",
 'x9': "poly->flags = ~(1 << i) & (u16)poly->flags;",
 'y1': "poly->flags = (~(1 << i)) & (poly->flags);",
 'y2': "poly->flags = (poly->flags | (1 << i)) ^ (1 << i);",
 'y3': "poly->flags = ~(~poly->flags | (1 << i));",
 'y4': "poly->flags = ~((1 << i) | ~poly->flags);",
 'y5': "poly->flags = poly->flags - (poly->flags & (1 << i));",
 'y6': "poly->flags &= (u16)~(1 << i);",
 'y7': "poly->flags &= ~(u16)(1 << i);",
 'y8': "poly->flags &= 0xFFFF ^ (1 << i);",
 'y9': "poly->flags &= ~(1 << (s32)i);",
}
V = {k: P + f for k, f in forms.items()}
