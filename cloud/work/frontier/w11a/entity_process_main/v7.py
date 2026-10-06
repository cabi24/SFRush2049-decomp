OLD = """        poly = &D_8015B268[v->objnum];
        i = ~(1 << i);
        poly->flags = i & poly->flags;"""
P = "        poly = &D_8015B268[v->objnum];\n        "
V = {
 'v1': P + "poly->flags = *(volatile u16 *)&poly->flags & ~(1 << i);",
 'v2': P + "poly->flags = ~(1 << i) & *(volatile u16 *)&poly->flags;",
 'v3': P + "*(volatile u16 *)&poly->flags &= ~(1 << i);",
 'v4': P + "poly->flags = ~(1 << i) & poly->flags;\n        if (i) {}",
 'v5': P + "poly->flags = ~(1 << i) & poly->flags;\n        if (poly) {}",
 'v6': "        if (i) {}\n" + P + "poly->flags = ~(1 << i) & poly->flags;",
 'v7': P + "if (i) {}\n        poly->flags = ~(1 << i) & poly->flags;",
 'v8': P + "poly->flags &= ~(1 << i);\n        if (i) {}",
}
