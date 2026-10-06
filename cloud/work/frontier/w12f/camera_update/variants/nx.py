O = """                    nx = v + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }"""
V = {
 'n1': [(O, """                    nx = v + 1;
                    if (sc->count <= nx) {
                        nx = 0;
                    }""")],
 'n2': [(O, """                    nx = v;
                    nx++;
                    if (nx >= sc->count) {
                        nx = 0;
                    }""")],
 'n4': [(O, """                    if ((nx = v + 1) >= sc->count) {
                        nx = 0;
                    }""")],
 'n5': [(O, """                    nx = (v + 1 < sc->count) ? v + 1 : 0;""")],
 'n6': [(O, """                    nx = v + 1;
                    if (nx >= sc->count) {
                        nx = 0;
                    }
                    if (nx) {}""")],
 'n7': [(O, """                    nx = v + 1;
                    if (nx < sc->count) {
                    } else {
                        nx = 0;
                    }""")],
}
