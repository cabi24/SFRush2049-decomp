O = """                        sv[i] = sc->keys[ctl->idx].scale[i] + (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) * (tt / f);"""
V = {
 'i1': [(O, """                        sv[i] = (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) * (tt / f) + sc->keys[ctl->idx].scale[i];""")],
 'i2': [(O, """                        sv[i] = sc->keys[ctl->idx].scale[i] + (tt / f) * (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]);""")],
 'i3': [(O, """                        sv[i] = sc->keys[ctl->idx].scale[i] - (sc->keys[ctl->idx].scale[i] - sc->keys[nx].scale[i]) * (tt / f);""")],
 'i4': [(O, """                        sv[i] = (tt / f) * (sc->keys[nx].scale[i] - sc->keys[ctl->idx].scale[i]) + sc->keys[ctl->idx].scale[i];""")],
}
