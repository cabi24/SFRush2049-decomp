O = """                    nx = v + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }"""
V = {
 'n8': [(O, """                    nx = v;
                    if (++nx >= sc->count) {
                        nx = 0;
                    }""")],
 'n9': [(O, """                    nx = ctl->idx + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }""")],
 'n10': [(O, """                    nx = v + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }
                    if (sc->keys[v].flags) {}""")],
}
