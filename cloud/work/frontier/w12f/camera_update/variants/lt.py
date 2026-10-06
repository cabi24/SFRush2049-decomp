L0 = """                if (sc->keys[idx].dur <= ctl->t) {
                    ctl->t = ctl->t - sc->keys[idx].dur;"""
V = {
 'a': [(L0, """                if (sc->keys[idx].dur) {}
""" + L0)],
 'b': [(L0, """                if (sc->keys[idx].dur <= ctl->t) {
                    ctl->t -= sc->keys[idx].dur;""")],
 'c': [(L0, """                if (ctl->t >= sc->keys[idx].dur) {
                    ctl->t = ctl->t - sc->keys[idx].dur;""")],
 'd': [(L0, L0 + "\n                    if (sc->keys[idx].dur) {}")],
 'e': [(L0, """                if (!(sc->keys[idx].dur > ctl->t)) {
                    ctl->t = ctl->t - sc->keys[idx].dur;""")],
}
