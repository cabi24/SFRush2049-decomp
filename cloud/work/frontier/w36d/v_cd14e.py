D='parts/func_8038CCEC.c'
VARIANTS = {
 'base': [],
 'nested_if': [(D, "        if (func_800F7644(D_8014978C) && D_803B7CE4[D_8014978C].handle == -1) {", "        if (func_800F7644(D_8014978C)) if (D_803B7CE4[D_8014978C].handle == -1) {")],
 'while_loop': [(D, "    for (D_8014978C = 14; D_8014978C <= 17; D_8014978C++) {", "    D_8014978C = 14;\n    while (D_8014978C <= 17) {"), (D, "        }\n    }\n    D_8014978C = saveTrackNum;", "        }\n        D_8014978C++;\n    }\n    D_8014978C = saveTrackNum;")],
 'lt18': [(D, "D_8014978C <= 17;", "D_8014978C < 18;")],
}
