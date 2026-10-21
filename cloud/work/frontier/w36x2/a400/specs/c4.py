BASE = 'a400/base'
H = """static s32 anyAttached(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (D_80156CF0[i].attached)
            break;
    }
    return i < 4;
}
"""
def h(decl, body): return [(H, "static s32 anyAttached(void)\n{\n" + decl + body + "}\n")]
I = "    s32 i;\n"
VARIANTS = {
 'contbrk': h(I, "    for (i = 0; i < 4; i++) {\n        if (!D_80156CF0[i].attached)\n            continue;\n        break;\n    }\n    return i < 4;\n"),
 'deadst': h(I + "    s32 f;\n", "    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached) {\n            f = 1;\n            break;\n        }\n    }\n    return i < 4;\n"),
 'holder': h(I, "    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached) {\n            if (i) {}\n            break;\n        }\n    }\n    return i < 4;\n"),
 'dw0': h(I, "    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached) {\n            do {} while (0);\n            break;\n        }\n    }\n    return i < 4;\n"),
 'ifelse_brk': h(I, "    for (i = 0; i < 4; i++) {\n        if (!D_80156CF0[i].attached) {\n        } else break;\n    }\n    return i < 4;\n"),
}
