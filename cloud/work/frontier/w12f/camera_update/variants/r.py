O = """                    tt = tt / f;
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * tt + f;
                    }"""
V = {
 'r1': [(O, """                    for (i = 0; i < 3; i++) {
                        sv[i] = (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) * (tt / f) + sc->keys[ctl->idx].scale[i];
                    }""")],
 'r2': [(O, """                    for (i = 0; i < 3; i++) {
                        sv[i] = sc->keys[ctl->idx].scale[i] + (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) * (tt / f);
                    }""")],
}
