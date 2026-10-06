D = [("    CamKey *k;\n", ""), ("    s32 i;\n    s8 moved;", "    s32 i;\n    s32 nx;\n    s8 moved;"), ("    s32 v;\n    s32 nx;\n", "    s32 v;\n"),
     ("""                na = ctl->idx;
                k = &sc->keys[na];
                if (!(k->flags & 0x10)) {
                    nx = na + 1;""", """                idx = ctl->idx;
                if (!(sc->keys[idx].flags & 0x10)) {
                    nx = idx + 1;""")]
O = """                    if (ctl->mode & 8) {
                        tt = k->dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * (tt / k->dur) + f;
                    }"""
V = {
 'a': D + [(O, O.replace("k->", "sc->keys[idx]."))],
 'b': D + [(O, """                    if (ctl->mode & 8) {
                        tt = sc->keys[idx].dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    tt = tt / sc->keys[idx].dur;
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * tt + f;
                    }""")],
 'c': D + [(O, """                    tt = ((ctl->mode & 8) ? sc->keys[idx].dur - ctl->t : ctl->t) / sc->keys[idx].dur;
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * tt + f;
                    }""")],
}
