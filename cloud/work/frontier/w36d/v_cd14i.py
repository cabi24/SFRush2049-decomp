S='parts/func_8038CCEC.c'; B='parts/func_8038CCF4.c'; R='parts/func_8038CCE4.c'
C1=("        if (func_800F7644(D_8014978C) && D_803B7CE4[D_8014978C].handle == -1) {", "        if (!func_800F7644(D_8014978C)) continue;\n        if (D_803B7CE4[D_8014978C].handle == -1) {")
VARIANTS = {
 'base': [],
 'cont_sb': [(S,)+C1, (B,)+C1],
 'cont_all': [(S,)+C1, (B,)+C1, (R, "        if (func_800F7644(D_8014978C)) {\n            func_8038CD04();", "        if (!func_800F7644(D_8014978C)) continue;\n        {\n            func_8038CD04();")],
}
