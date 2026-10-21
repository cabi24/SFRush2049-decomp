BASE = 'work/w36d_best'
D = "    s32 vtxSize, dlSize;\n"
A = "            vtxSize = D_803B7CD8 * 64 + 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, vtxSize + 7);\n"
B = "            dlSize = D_803B7CD8 * 40 + 56;\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);\n"
B5 = "            dlSize = (D_803B7CD8 * 5 + 7) * sizeof(Gfx);\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);\n"
def v(decl, va, da, bb=B5):
    return [(D, decl), (A, A.replace('vtxSize', va)), (B, bb.replace('dlSize', da))]
VARIANTS = {
 'one': v("    s32 vtxSize[1], dlSize[1];\n", "vtxSize[0]", "dlSize[0]"),
 'one_u': v("    u32 vtxSize[1], dlSize[1];\n", "vtxSize[0]", "dlSize[0]"),
 'arr_u': v("    u32 size[2];\n", "size[1]", "size[0]"),
 'arr3': v("    s32 size[3];\n", "size[2]", "size[1]"),
 'arr_v4': v("    s32 size[2];\n", "size[1]", "size[0]", B5.replace("(D_803B7CD8 * 5 + 7) * sizeof(Gfx)", "D_803B7CD8 * 40 + 56")),
}
