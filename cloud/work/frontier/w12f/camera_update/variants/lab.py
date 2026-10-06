O = """                        m = (&D_801427C0)[na];
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, m);
                        }"""
V = {
 'g1': [(O, """                        m = (&D_801427C0)[na];
                        if (!(sc->flags & 0x8000)) {
lbl:
                            func_800C15FC(cam->slot, m);
                        }""")],
 'g2': [(O, """                        m = (&D_801427C0)[na];
lbl:
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, m);
                        }""")],
}
