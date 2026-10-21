R='parts/func_8038CCF4.c'
VARIANTS = {
 'vol': [(R, "    s32 vtxSize, dlSize;\n", "    volatile s32 vtxSize, dlSize;\n")],
 'addr': [(R, "            vtxSize = D_803B7CD8 * 64 + 64;\n", "            (void)&vtxSize; (void)&dlSize;\n            vtxSize = D_803B7CD8 * 64 + 64;\n")],
 'u32': [(R, "    s32 vtxSize, dlSize;\n", "    u32 vtxSize, dlSize;\n")],
}
