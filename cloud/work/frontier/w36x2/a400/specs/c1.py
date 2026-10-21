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
def h(body): return [(H, "static s32 anyAttached(void)\n{\n    s32 i;\n" + body + "}\n")]
VARIANTS = {
 'base': [],
 'ret1': h("    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached)\n            return 1;\n    }\n    return 0;\n"),
 'while': h("    i = 0;\n    while (i < 4) {\n        if (D_80156CF0[i].attached)\n            break;\n        i++;\n    }\n    return i < 4;\n"),
 'dowhile': h("    i = 0;\n    do {\n        if (D_80156CF0[i].attached)\n            break;\n    } while (++i < 4);\n    return i < 4;\n"),
 'forcond': h("    for (i = 0; i < 4 && !D_80156CF0[i].attached; i++)\n        ;\n    return i < 4;\n"),
 'ne4': h("    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached)\n            break;\n    }\n    return i != 4;\n"),
 'goto': h("    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached)\n            goto found;\n    }\n    return 0;\nfound:\n    return 1;\n"),
 'ifelse': h("    for (i = 0; i < 4; i++) {\n        if (D_80156CF0[i].attached)\n            break;\n    }\n    if (i < 4) return 1;\n    return 0;\n"),
}
