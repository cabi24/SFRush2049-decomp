V = {
 'm1': [("    s16 na;\n", "    s16 na;\n    u16 m;\n"),
        ("""                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, (&D_801427C0)[na]);
                        }""", """                        m = (&D_801427C0)[na];
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, m);
                        }""")],
}
