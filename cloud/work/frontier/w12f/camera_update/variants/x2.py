C = ("} else if (idx + 1 == sc->count) {\n                        if (sc->flags & 0x80) {", "} else if (sc->count == idx + 1) {\n                        if (sc->flags & 0x80) {")
V = {
 'x6': [C, ("ctl->idx = idx + 1;", "ctl->idx++;")],
 'x7': [C, ("ctl->idx = idx + 1;", "ctl->idx += 1;")],
 'x8': [("ctl->idx = idx + 1;", "ctl->idx++;")],
}
