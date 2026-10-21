D='parts/func_8038DFEC.c'
L1="    D_803BA898 = 0;\n    for (i = 0; i < 21; i++) if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;"
VARIANTS = {
 'base': [],
 'sameline': [(D, L1, "    D_803BA898 = 0; for (i = 0; i < 21; i++) if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;")],
 'bodyline': [(D, L1, "    D_803BA898 = 0;\n    for (i = 0; i < 21; i++)\n        if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;")],
 'bodyline2': [(D, L1, "    D_803BA898 = 0;\n    for (i = 0; i < 21; i++) if (func_8038ACB0(i))\n        D_803BA8B0[D_803BA898++] = i;")],
 'stmt_same': [(D, "    D_803BAC78 = -1;\n    D_803BA898 = 0;", "    D_803BAC78 = -1; D_803BA898 = 0;")],
 'all_one': [(D, "    D_803BA85A = 0;\n    D_803BA878 = 0;\n    D_803BA910 = 0;\n    D_803BAC78 = -1;\n    D_803BA898 = 0;", "    D_803BA85A = 0; D_803BA878 = 0; D_803BA910 = 0; D_803BAC78 = -1; D_803BA898 = 0;")],
}
