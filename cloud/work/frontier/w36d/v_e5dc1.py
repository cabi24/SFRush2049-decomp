E='e5dc.c'
G1=("        if (D_8014A110 == 3 && (D_80154628 > 0 || D_803BA8B0[D_803BA85A] != 20)) goto controls_done;\n        if (add == 0 && !D_803BA910) goto controls_done;\n        if (D_8014A110 == 2 && D_803BA8B0[D_803BA85A] == 2) goto controls_done;\n",
    "        if ((D_8014A110 == 3 && (D_80154628 > 0 || D_803BA8B0[D_803BA85A] != 20)) || (add == 0 && !D_803BA910) || (D_8014A110 == 2 && D_803BA8B0[D_803BA85A] == 2)) goto controls_done;\n")
T13=("                else D_80146108[28] = !D_80146108[28];", "                else if (!D_80146108[28]) D_80146108[28] = 1;\n                else D_80146108[28] = 0;")
VARIANTS = {
 'base': [],
 'oneif': [(E,)+G1],
 't13': [(E,)+T13],
 'oneif_t13': [(E,)+G1, (E,)+T13],
}
