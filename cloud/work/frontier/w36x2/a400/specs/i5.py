BASE = 'a400/base3'
L = "        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i < 4) {\n"
def v(s): return [(L, s)]
VARIANTS = {
 'x1': v("        for (i = 0; i < 4; i++) {\n            Controller *c = &D_80156CF0[i];\n            if (c->attached)\n                break;\n        }\n        if (i < 4) {\n"),
 'x2': v("        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached != 0)\n                break;\n        }\n        if (i < 4) {\n"),
 'x3': v("        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached == 0) continue;\n            else break;\n        }\n        if (i < 4) {\n"),
 'x4': v("        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i != 4) {\n"),
 'x5': v("        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i < 4)\n        {\n"),
 'x6': v("        for (i = 0; i < 4; i++)\n            if (D_80156CF0[i].attached) break;\n        if (i < 4) {\n"),
}
