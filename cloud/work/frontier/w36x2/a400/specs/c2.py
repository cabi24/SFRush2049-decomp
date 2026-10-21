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
def h(body): return [(H, "static s32 anyAttached(void)\n{\n    Controller *c;\n" + body + "}\n")]
VARIANTS = {
 'p1': h("    for (c = D_80156CF0; c < &D_80156CF0[4]; c++) {\n        if (c->attached)\n            break;\n    }\n    return c < &D_80156CF0[4];\n"),
 'p2': h("    for (c = &D_80156CF0[0]; c < &D_80156CF0[4]; c++) {\n        if (c->attached)\n            break;\n    }\n    return c < &D_80156CF0[4];\n"),
 'p3': h("    for (c = D_80156CF0; c < D_80156CF0 + 4; c++) {\n        if (c->attached)\n            break;\n    }\n    return c < D_80156CF0 + 4;\n"),
 'p4': h("    for (c = D_80156CF0; c < &D_80156CF0[4]; c++) {\n        if (c->attached)\n            return 1;\n    }\n    return 0;\n"),
 'p5': h("    for (c = D_80156CF0; c < &D_80156CF0[4]; c++) {\n        if (c->attached)\n            break;\n    }\n    if (c < &D_80156CF0[4]) return 1;\n    return 0;\n"),
 'p6': h("    for (c = D_80156CF0; c < &D_80156CF0[4]; c++) {\n        if (c->attached)\n            break;\n    }\n    return c != &D_80156CF0[4];\n"),
}
