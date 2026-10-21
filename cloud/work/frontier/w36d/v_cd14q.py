R='parts/func_8038CCF4.c'; H='parts/header.c'; C='parts/func_8038CD14.c'
A=("            vtxSize = D_803B7CD8 * 64 + 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, vtxSize + 7);\n            D_803BAAE8[D_8014978C].verts = vtx = (Vtx_t *)(((u32)D_803BABE8[D_8014978C] + 7) & ~7);\n            dlSize = D_803B7CD8 * 40 + 56;\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);\n            D_803BAAE8[D_8014978C].displayList = (u32 *)(dl = (Gfx *)(((u32)D_803BABC8[D_8014978C] + 7) & ~7));\n")
V1=A, ("            D_803BABE8[D_8014978C] = trk_Alloc(D_803B7CD8 * 64 + 64);\n            D_803BAAE8[D_8014978C].verts = vtx = (Vtx_t *)(((u32)D_803BABE8[D_8014978C] + 7) & ~7);\n            D_803BABC8[D_8014978C] = trk_Alloc(D_803B7CD8 * 40 + 56);\n            D_803BAAE8[D_8014978C].displayList = (u32 *)(dl = (Gfx *)(((u32)D_803BABC8[D_8014978C] + 7) & ~7));\n")
V2=A, ("            D_803BAAE8[D_8014978C].verts = vtx = trk_Alloc2(&D_803BABE8[D_8014978C], D_803B7CD8 * 64 + 64);\n            D_803BAAE8[D_8014978C].displayList = (u32 *)(dl = trk_Alloc2(&D_803BABC8[D_8014978C], D_803B7CD8 * 40 + 56));\n")
HD=(H, "void func_8038AEA8(void);", "void func_8038AEA8(void);\nvoid *trk_Alloc(s32 size);\nvoid *trk_Alloc2(void **raw, s32 size);")
DEF="void *trk_Alloc(s32 size)\n{\n    return audio_dma_sync(0, size + 7);\n}\n\nvoid *trk_Alloc2(void **raw, s32 size)\n{\n    *raw = audio_dma_sync(0, size + 7);\n    return (void *)(((u32)*raw + 7) & ~7);\n}\n\n"
DF=(R, "/* buildRaceTrackMaps", DEF + "/* buildRaceTrackMaps")
NOOB=(C, "    else if (D_8014A110 == 5) func_8038CCFC();\n    else func_8038CCF4();", "    else if (D_8014A110 != 5) func_8038CCF4();")
VARIANTS = {
 'base': [],
 'v1': [(R,)+V1, HD, DF],
 'v2': [(R,)+V2, HD, DF],
 'v1_noobs': [(R,)+V1, HD, DF, NOOB],
 'v2_noobs': [(R,)+V2, HD, DF, NOOB],
}
