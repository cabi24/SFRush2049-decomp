H='parts/header.c'
VARIANTS = {
 'base': [],
 'tn_static': [(H, "extern s8 D_8014978C;", "static s8 D_8014978C;")],
 'tn_init': [(H, "extern s8 D_8014978C;", "s8 D_8014978C = 0;")],
 'tm_static_init': [('parts/decls.c', "extern Slot72 D_803B7CE4[20];", "static Slot72 D_803B7CE4[20] = {0};")],
}
