D='parts/func_8038CCEC.c'
P="            D_803B7CE4[D_8014978C].position[0] = D_803B8370[0];\n            D_803B7CE4[D_8014978C].position[1] = D_803B8370[1];\n            D_803B7CE4[D_8014978C].position[2] = D_803B8370[2];\n"
VARIANTS = {
 'base': [],
 'const_idx': [(D, P, "            D_803B7CE4[3].position[0] = D_803B8370[0];\n            D_803B7CE4[3].position[1] = D_803B8370[1];\n            D_803B7CE4[3].position[2] = D_803B8370[2];\n")],
 'use_only': [(D, P, "            D_803B7CE4[D_8014978C].offset_x = 0;\n")],
 'load_only': [(D, P, "            D_803B8370[0] = D_803B7CE4[D_8014978C].position[0];\n")],
}
