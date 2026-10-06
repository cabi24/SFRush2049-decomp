V = {
 'k1': [("""                    nx = v + 1;""", """                    if (&sc->keys[v]) {}
                    nx = v + 1;""")],
 'k2': [("""                    for (i = 0; i < 3; i++) {
                        sv[i] = sc->keys[ctl->idx]""", """                    if (&sc->keys[v]) {}
                    for (i = 0; i < 3; i++) {
                        sv[i] = sc->keys[ctl->idx]""")],
 'k3': [("""                    if (ctl->mode & 8) {
                        f = sc->keys[v].dur;""", """                    if (&sc->keys[v]) {}
                    if (ctl->mode & 8) {
                        f = sc->keys[v].dur;""")],
}
