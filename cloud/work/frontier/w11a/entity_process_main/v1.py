OLD = """        poly = &D_8015B268[v->objnum];
        i = ~(1 << i);
        poly->flags = i & poly->flags;"""
P = "        poly = &D_8015B268[v->objnum];\n"
V = {
 'A': P + "        poly->flags &= ~(1 << i);",
 'B': "        D_8015B268[v->objnum].flags &= ~(1 << i);",
 'C': P + "        poly->flags = poly->flags & ~(1 << i);",
 'E': P + "        poly->flags = ~(1 << i) & poly->flags;",
 'F': P + "        i = 1 << i;\n        poly->flags &= ~i;",
 'G': P + "        i = 1 << i;\n        poly->flags = ~i & poly->flags;",
 'H': P + "        poly->flags = (u16)(poly->flags & ~(1 << i));",
 'I': P + "        poly->flags = ~(1 << i) & (s32)poly->flags;",
 'J': P + "        i = ~(1 << i);\n        poly->flags &= i;",
}
