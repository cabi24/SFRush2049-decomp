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
C = "        if (anyAttached()) {\n"
def inl(pre, cond, decl=""):
    e = [(H, ""), (C, pre + "        if (" + cond + ") {\n")]
    if decl: e.append(("    s32 numPaks;\n", "    s32 numPaks;\n" + decl))
    return e
L1 = "        for (pak = 0; pak < 4; pak++) {\n            if (D_80156CF0[pak].attached)\n                break;\n        }\n"
L2 = "        for (i = 0; i < 4; i++) {\n            if (D_80156CF0[i].attached)\n                break;\n        }\n"
VARIANTS = {
 'pak_lt': inl(L1, "pak < 4"),
 'pak_ne': inl(L1, "pak != 4"),
 'i_lt': inl(L2, "i < 4", "    s32 i;\n"),
 'i_ne': inl(L2, "i != 4", "    s32 i;\n"),
 'pak_lt_cont': inl("        for (pak = 0; pak < 4; pak++) {\n            if (!D_80156CF0[pak].attached) continue;\n            break;\n        }\n", "pak < 4"),
}
