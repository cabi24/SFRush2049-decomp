L0 = """                f = sc->keys[idx].dur;
                if (f <= ctl->t) {
                    ctl->t = ctl->t - f;"""
V = {
 'a1': [(L0, """                if (sc->keys[idx].dur <= ctl->t) {
                    ctl->t = ctl->t - sc->keys[idx].dur;""")],
 'a2': [(L0, """                if (ctl->t >= sc->keys[idx].dur) {
                    ctl->t -= sc->keys[idx].dur;""")],
 'a3': [(L0, """                f = sc->keys[idx].dur;
                if (ctl->t >= f) {
                    ctl->t = ctl->t - f;""")],
 'a4': [(L0, """                f = sc->keys[idx].dur;
                if (f <= ctl->t) {
                    ctl->t -= f;""")],
 'a5': [(L0, """                if ((f = sc->keys[idx].dur) <= ctl->t) {
                    ctl->t -= f;""")],
}
