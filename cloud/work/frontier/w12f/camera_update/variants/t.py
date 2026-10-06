O = """                    if (ctl->mode & 8) {
                        tt = k->dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * (tt / k->dur) + f;
                    }"""
T1 = """                    tt = ((ctl->mode & 8) ? k->dur - ctl->t : ctl->t) / k->dur;
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * tt + f;
                    }"""
NX = """                    nx = na + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }
"""
V = {
 't1': [(O, T1)],
 't2': [(O, T1), (NX, ""), ("sc->keys[nx]", "sc->keys[(na + 1 >= sc->count) ? 0 : na + 1]"), ("    s32 nx;\n", "")],
 't3': [(O, """                    if (ctl->mode & 8) {
                        tt = k->dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    tt = tt / k->dur;
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * tt + f;
                    }""")],
}
