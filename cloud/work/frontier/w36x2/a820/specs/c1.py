BASE = 'work/w36d_best'
D = "    s32 vtxSize, dlSize;\n"
A = "            vtxSize = D_803B7CD8 * 64 + 64;\n            D_803BABE8[D_8014978C] = audio_dma_sync(0, vtxSize + 7);\n"
B = "            dlSize = D_803B7CD8 * 40 + 56;\n            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);\n"
def sz(t, ord_):
    decl = "    struct { %s; } sz;\n" % ord_
    return [(D, decl), (A, A.replace('vtxSize', 'sz.vtxSize')), (B, B.replace('dlSize', 'sz.dlSize'))]
VARIANTS = {
 'st_dv': sz('s32', 's32 dlSize; s32 vtxSize'),
 'st_vd': sz('s32', 's32 vtxSize; s32 dlSize'),
 'arr': [(D, "    s32 size[2];\n"), (A, A.replace('vtxSize', 'size[1]')), (B, B.replace('dlSize', 'size[0]'))],
 'arr2': [(D, "    s32 size[2];\n"), (A, A.replace('vtxSize', 'size[0]')), (B, B.replace('dlSize', 'size[1]'))],
}
