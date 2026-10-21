BASE = 'work/cd14_g5'
A = "            size[1] = D_803B7CD8 * 64 + 64;\n"
VARIANTS = {
 'both_sz': [(A, "            size[1] = (D_803B7CD8 + 1) * 4 * sizeof(Vtx);\n")],
 'both_sz2': [(A, "            size[1] = (D_803B7CD8 * 4 + 4) * sizeof(Vtx);\n")],
 'both_sz3': [(A, "            size[1] = (D_803B7CD8 + 1) * (4 * sizeof(Vtx));\n")],
 'both_sz4': [(A, "            size[1] = 4 * (D_803B7CD8 + 1) * sizeof(Vtx);\n")],
 'u_cast': [(A, "            size[1] = (u32)(D_803B7CD8 * 64 + 64);\n")],
}
