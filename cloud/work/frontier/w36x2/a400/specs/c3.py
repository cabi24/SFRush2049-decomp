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
C = "    Controller *c;\n"
L = "    for (c = D_80156CF0; c < &D_80156CF0[4]; c++) {\n        if (c->attached)\n            break;\n    }\n"
VARIANTS = {
 'q1': h(C, L + "    return (c - D_80156CF0) < 4;\n"),
 'q2': h(C + "    s32 i;\n", L + "    i = c - D_80156CF0;\n    return i < 4;\n"),
 'q3': h(C + "    s32 i;\n", "    for (i = 0, c = D_80156CF0; i < 4; i++, c++) {\n        if (c->attached)\n            break;\n    }\n    return i < 4;\n"),
 'q5': h(C + "    Controller *end = &D_80156CF0[4];\n", "    for (c = D_80156CF0; c < end; c++) {\n        if (c->attached)\n            break;\n    }\n    return c < &D_80156CF0[4];\n"),
 'q6': h(C, L + "    if (c >= &D_80156CF0[4]) return 0;\n    return 1;\n"),
 'q7': h("    s32 i;\n", "    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached) {\n            break;\n        }\n    }\n    return i < 4 ? 1 : 0;\n"),
}
