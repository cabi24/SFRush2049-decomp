D='cd14.c'
VI=[(D, "            for (pointIdx = 0, vertIdx = 0; pointIdx < D_803B7CD8; pointIdx++, vertIdx += 4) {", "            for (pointIdx = 0; pointIdx < D_803B7CD8; pointIdx++) {"),
    (D, "GFX(dl, 0x01004008, (u32)((Vtx_t *)D_803BABE8[D_8014978C] + vertIdx));", "GFX(dl, 0x01004008, (u32)((Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4));"),
    (D, "GFX(dl, 0x01008010, (u32)((Vtx_t *)D_803BABE8[D_8014978C] + vertIdx));", "GFX(dl, 0x01008010, (u32)((Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4));")]
SZ=[(D, "            D_803BABE8[D_8014978C] = audio_dma_sync(0, D_803B7CD8 * 64 + 64 + 7);", "            vtxSize = D_803B7CD8 * 64 + 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, vtxSize + 7);"),
    (D, "            D_803BABC8[D_8014978C] = audio_dma_sync(0, D_803B7CD8 * 40 + 56 + 7);", "            dlSize = D_803B7CD8 * 40 + 56;\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);"),
    (D, "    f32 sectVerts[8][3];\n    saveTrackNum", "    f32 sectVerts[8][3];\n    s32 vtxSize, dlSize;\n    saveTrackNum")]
VARIANTS = {
 'base': [],
 'vi': VI,
 'vi_sz': VI+SZ,
}
