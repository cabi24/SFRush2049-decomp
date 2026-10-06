OLD = """        poly = &D_8015B268[v->objnum];
        poly->flags = ~(1 << i) & poly->flags;"""
P = "        poly = &D_8015B268[v->objnum];\n"
S = "        poly->flags = ~(1 << i) & poly->flags;"
A = "        poly->flags &= ~(1 << i);"
V = {
 'k1': P + "        if (poly->flags) {}\n" + S,
 'k2': P + "        if (1 << i) {}\n" + S,
 'k3': P + "        if (~(1 << i)) {}\n" + S,
 'k4': P + "        if (poly->flags) {}\n" + A,
 'k5': P + "        if (1 << i) {}\n" + A,
 'k6': "        if (poly) {}\n" + P + S,
 'k7': P + "        if (poly) {}\n" + A,
 'k8': "        if (1 << i) {}\n" + P + A,
}
