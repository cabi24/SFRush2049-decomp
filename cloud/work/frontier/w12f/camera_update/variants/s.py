O = """                    na = cam->s58 + v;
                    if (na != cam->s50) {
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, (&D_801427C0)[na]);
                        }
                        cam->s50 = na;
                    }"""
V = {
 's1': [("    s16 na;\n", "    s32 na;\n"), (O, """                    na = cam->s58 + v;
                    na = (s16) na;
                    if (na != cam->s50) {
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, (&D_801427C0)[na]);
                        }
                        cam->s50 = na;
                    }""")],
 's2': [(O, O.replace("na = cam->s58 + v;", "na = (s16)(cam->s58 + v);"))],
 's3': [("    s16 na;\n", "    s32 na;\n"), (O, """                    na = cam->s58 + v;
                    if ((s16) na != cam->s50) {
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, (&D_801427C0)[(s16) na]);
                        }
                        cam->s50 = na;
                    }""")],
 's4': [(O, """                    na = cam->s58;
                    na += v;
                    if (na != cam->s50) {
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, (&D_801427C0)[na]);
                        }
                        cam->s50 = na;
                    }""")],
 's5': [(O, """                    if ((na = cam->s58 + v) != cam->s50) {
                        if (!(sc->flags & 0x8000)) {
                            func_800C15FC(cam->slot, (&D_801427C0)[na]);
                        }
                        cam->s50 = na;
                    }""")],
}
