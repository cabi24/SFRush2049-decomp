O = """                    if (ctl->mode & 8) {
                        f = sc->keys[idx].dur;
                        tt = f - ctl->t;
                    } else {
                        tt = ctl->t;
                        f = sc->keys[idx].dur;
                    }"""
V = {
 'z1': [(O, """                    f = sc->keys[idx].dur;
                    if (ctl->mode & 8) {
                        tt = f - ctl->t;
                    } else {
                        tt = ctl->t;
                    }""")],
 'z2': [(O, """                    if (ctl->mode & 8) {
                        tt = sc->keys[idx].dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    f = sc->keys[idx].dur;""")],
 'z3': [(O, """                    if (ctl->mode & 8) {
                        tt = (f = sc->keys[idx].dur) - ctl->t;
                    } else {
                        tt = ctl->t;
                        f = sc->keys[idx].dur;
                    }""")],
}
