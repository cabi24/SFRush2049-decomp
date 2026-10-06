O = """                    if (ctl->mode & 8) {
                        tt = k->dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * (tt / k->dur) + f;
                    }"""
D = [("    f32 tt;\n", "")]
V = {
 'a': D + [(O, """                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * (((ctl->mode & 8) ? k->dur - ctl->t : ctl->t) / k->dur) + f;
                    }""")],
 'b': D + [(O, """                    for (i = 0; i < 3; i++) {
                        f = sc->keys[ctl->idx].scale[i];
                        sv[i] = (sc->keys[nx].scale[i] - f) * (((ctl->mode & 8) ? (k->dur - ctl->t) / k->dur : ctl->t / k->dur)) + f;
                    }""")],
 'c': D + [(O, """                    if (ctl->mode & 8) {
                        f = k->dur - ctl->t;
                    } else {
                        f = ctl->t;
                    }
                    f = f / k->dur;
                    for (i = 0; i < 3; i++) {
                        sv[i] = (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) * f + sc->keys[ctl->idx].scale[i];
                    }""")],
}
