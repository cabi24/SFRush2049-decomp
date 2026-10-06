O = """                    if (ctl->mode & 8) {
                        tt = sc->keys[v].dur - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    tt = tt / sc->keys[v].dur;"""
V = {
 'b1': [(O, """                    f = sc->keys[v].dur;
                    if (ctl->mode & 8) {
                        tt = f - ctl->t;
                    } else {
                        tt = ctl->t;
                    }
                    tt = tt / f;""")],
 'b2': [(O, """                    if (ctl->mode & 8) {
                        f = sc->keys[v].dur;
                        tt = f - ctl->t;
                    } else {
                        tt = ctl->t;
                        f = sc->keys[v].dur;
                    }
                    tt = tt / f;""")],
 'b3': [(O, """                    if (ctl->mode & 8) {
                        tt = (f = sc->keys[v].dur) - ctl->t;
                    } else {
                        tt = ctl->t;
                        f = sc->keys[v].dur;
                    }
                    tt = tt / f;""")],
}
