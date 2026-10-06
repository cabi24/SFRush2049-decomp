S1 = "} else if (idx + 1 == sc->count) {\n                        if (sc->flags & 0x80) {"
S2 = "} else if (sc->count == idx + 1) {"
V = {
 'y1': [(S1, "} else if (sc->count - 1 == idx) {\n                        if (sc->flags & 0x80) {")],
 'y2': [(S1, "} else if (sc->count - 1 == idx) {\n                        if (sc->flags & 0x80) {"), (S2, "} else if (sc->count - 1 == idx) {")],
 'y3': [(S2, "} else if (sc->count - 1 == idx) {")],
 'y4': [(S1, "} else if (idx == sc->count - 1) {\n                        if (sc->flags & 0x80) {"), (S2, "} else if (idx == sc->count - 1) {")],
}
