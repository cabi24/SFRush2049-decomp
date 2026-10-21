BASE = 'work/usc_best'
U = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11;\n"
U11 = "    s32 u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;\n"
B = [(U, U11)]
def v(*e): return B + list(e)
VARIANTS = {
 'b112': v(),
 'idx_s32': v(("    s16 idx;\n", "    s32 idx;\n")),
 'idx_late': v(("    idx = slot % 13;\n    if (D_80151AD0 < 3) {", "    if (D_80151AD0 < 3) {"), ("    if (alpha == 0) {\n        if (!D_803BA2F0", "    idx = slot % 13;\n    if (alpha == 0) {\n        if (!D_803BA2F0")),
 'use_rem1': v(("func_80390BC0(slot, idx,", "func_80390BC0(slot, slot % 13,")),
 'use_rem2': v(("sfx_volume_set(slot, idx,", "sfx_volume_set(slot, slot % 13,")),
 'use_rem3': v(("sound_bank_unload(slot, idx,", "sound_bank_unload(slot, slot % 13,")),
 'cam_s32': v(("    s16 cam;\n", "    s32 cam;\n")),
 'sw_if': v(("    switch (player) {\n    case 0: cam = 1; break;\n    case 1: cam = 2; break;\n    case 2: cam = 4; break;\n    case 3: cam = 8; break;\n    }\n", "    if (player == 0) cam = 1;\n    else if (player == 1) cam = 2;\n    else if (player == 2) cam = 4;\n    else if (player == 3) cam = 8;\n")),
 'pl_else': v(("        if (player == 2) player = 0;\n        if (player == 3) player = 1;\n", "        if (player == 2) player = 0;\n        else if (player == 3) player = 1;\n")),
 'ac_after': v(("    Color alphaCol = D_803B65B4;\n", "    Color alphaCol;\n"), ("    idx = slot % 13;\n", "    alphaCol = D_803B65B4;\n    idx = slot % 13;\n")),
 'idx_u16': v(("    s16 idx;\n", "    u16 idx;\n")),
}
