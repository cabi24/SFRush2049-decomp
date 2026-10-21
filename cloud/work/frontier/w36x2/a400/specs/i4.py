BASE = 'a400/base3'
L = "        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i < 4) {\n"
def v(s): return [(L, s)]
VARIANTS = {
 'le3': v("        for (i = 0; i <= 3; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i <= 3) {\n"),
 'le3b': v("        for (i = 0; i <= 3; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i < 4) {\n"),
 'ne4': v("        for (i = 0; i != 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i != 4) {\n"),
 'gt': v("        for (i = 0; 4 > i; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (4 > i) {\n"),
 'dw': v("        i = 0;\n        do {\n            if (D_80156CF0[i].attached)\n                break;\n            i++;\n        } while (i < 4);\n        if (i < 4) {\n"),
 'wh': v("        i = 0;\n        while (i < 4) {\n            if (D_80156CF0[i].attached)\n                break;\n            i++;\n        }\n        if (i < 4) {\n"),
 'wh2': v("        i = 0;\n        while (i < 4 && !D_80156CF0[i].attached)\n            i++;\n        if (i < 4) {\n"),
 'ge4': v("        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (!(i >= 4)) {\n"),
 'inc_pre': v("        for (i = 0; i < 4; ++i) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n        if (i < 4) {\n"),
 'goto': v("        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                goto found;\n        }\n        goto done;\nfound:\n        {\n"),
}
