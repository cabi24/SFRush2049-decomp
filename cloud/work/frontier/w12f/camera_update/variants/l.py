O = """                    for (i = 0; i < 3; i++) {
                        sv[i] = sc->keys[ctl->idx].scale[i]"""
V = {
 'l1': [(O, O.replace("i < 3", "&sv[i] < &sv[3]"))],
}
