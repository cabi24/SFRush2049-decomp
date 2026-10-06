L0 = """                f = sc->keys[idx].dur;
                if (f <= ctl->t) {
                    ctl->t = ctl->t - f;"""
V = {
 'a6': [(L0, """                if (ctl->t >= (f = sc->keys[idx].dur)) {
                    ctl->t = ctl->t - f;""")],
 'a7': [(L0, """                tt = ctl->t;
                f = sc->keys[idx].dur;
                if (f <= tt) {
                    ctl->t = tt - f;""")],
 'a8': [(L0, """                if (sc->keys[idx].dur <= ctl->t) {
                    f = sc->keys[idx].dur;
                    ctl->t = ctl->t - f;""")],
 'a9': [(L0, """                if (sc->keys[idx].dur <= ctl->t) {
                    ctl->t -= (f = sc->keys[idx].dur);""")],
}
