O = """                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, m);
                        }"""
V = {
 'e1': [(O, """                        if (sc->flags & 0x8000) {
                        } else {
                            func_800C15FC(cam->slot, m);
                        }""")],
 'e2': [(O, """                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, m);
                        } else {
                        }""")],
 'e3': [(O, """                        if ((sc->flags & 0x8000) == 0) {
                            func_800C15FC(cam->slot, m);
                        }""")],
}
