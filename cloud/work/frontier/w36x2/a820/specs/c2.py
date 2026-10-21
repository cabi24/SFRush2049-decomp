BASE = 'work/w36d_best'
D = "    s32 vtxSize, dlSize;\n"
A = "            vtxSize = D_803B7CD8 * 64 + 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, vtxSize + 7);\n"
B = "            dlSize = D_803B7CD8 * 40 + 56;\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);\n"
ARR = [(D, "    s32 size[2];\n")]
VARIANTS = {
 'g2': ARR + [(A, "            D_803BABE8[D_8014978C] = audio_dma_sync(0, (size[1] = D_803B7CD8 * 64 + 64) + 7);\n"), (B, B.replace('dlSize', 'size[0]'))],
 'g2b': ARR + [(A, "            D_803BABE8[D_8014978C] = audio_dma_sync(0, (size[1] = D_803B7CD8 * 64 + 64) + 7);\n"), (B, "            D_803BABC8[D_8014978C] = audio_dma_sync(0, (size[0] = D_803B7CD8 * 40 + 56) + 7);\n")],
 'g3': ARR + [(A, "            size[1] = (D_803B7CD8 + 1) * 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, size[1] + 7);\n"), (B, B.replace('dlSize', 'size[0]'))],
 'g4': ARR + [(A, "            size[1] = (D_803B7CD8 + 1) * 4 * sizeof(Vtx);\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, size[1] + 7);\n"), (B, B.replace('dlSize', 'size[0]'))],
 'g5': ARR + [(A, "            size[1] = D_803B7CD8 * 64 + 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, size[1] + 7);\n"), (B, "            size[0] = (D_803B7CD8 * 5 + 7) * sizeof(Gfx);\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, size[0] + 7);\n")],
}
