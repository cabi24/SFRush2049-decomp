BASE = 'work/w36d_best'
TAIL = "                                pStrip++;\n                            }\n                        }\n                    }\n                }\n            }\n        }\n        D_803B7CE0 +="
def at(pos, stmt):
    parts = ["                                pStrip++;\n                            }\n", "                        }\n", "                    }\n", "                }\n", "            }\n", "        }\n        D_803B7CE0 +="]
    ind = ["", "                        ", "                    ", "                ", "            ", "        "]
    out = parts[0]
    for k in range(1, 6):
        if k == pos: out += ind[k] + stmt + "\n"
        out += parts[k]
    return [(TAIL, out)]
VARIANTS = {}
for pos, nm in ((1, 'P1'), (2, 'P2'), (3, 'P3'), (4, 'P4')):
    for sn, st in (('pS', 'pStrip = 0;'), ('fd', 'fade = 0;'), ('hold', 'if (mapIdx) {}'), ('dw', 'do {} while (0);'), ('ii', 'i = 0;')):
        VARIANTS[nm + '_' + sn] = at(pos, st)
VARIANTS['base'] = []
