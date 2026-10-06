C = ("} else if (idx + 1 == sc->count) {\n                        if (sc->flags & 0x80) {", "} else if (sc->count == idx + 1) {\n                        if (sc->flags & 0x80) {")
V = {
 'x2': [C, ("ctl->idx = idx + 1;", "ctl->idx = 1 + idx;")],
 'x3': [C, ("ctl->idx = idx + 1;\n                        idx = ctl->idx;", "idx++;\n                        ctl->idx = idx;\n                        idx = ctl->idx;")],
 'x4': [C, ("ctl->idx = idx + 1;\n                        idx = ctl->idx;", "ctl->idx = ++idx;\n                        idx = ctl->idx;")],
 'x5': [("} else if (idx + 1 == sc->count) {\n                        if (sc->flags & 0x80) {", "} else if (sc->count == 1 + idx) {\n                        if (sc->flags & 0x80) {")],
}
