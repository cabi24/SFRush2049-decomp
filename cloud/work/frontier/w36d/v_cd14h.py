H='parts/header.c'; DE='parts/decls.c'; D='parts/func_8038CCEC.c'
VARIANTS = {
 'both_def': [(H, "extern s8 D_8014978C;", "s8 D_8014978C = 0;"), (DE, "extern Slot72 D_803B7CE4[20];", "Slot72 D_803B7CE4[20] = {0};")],
 'tm_def_init': [(DE, "extern Slot72 D_803B7CE4[20];", "Slot72 D_803B7CE4[20] = {0};")],
 'tm_def_mp_static': [(DE, "extern Slot72 D_803B7CE4[20];", "Slot72 D_803B7CE4[20] = {0};"), (D, "    s32 saveTrackNum;\n", "    static f32 mapPos[3] = {-100.0f, 30.0f, 200.0f};\n    s32 saveTrackNum;\n"), (D, "D_803B8370[0];", "mapPos[0];"), (D, "D_803B8370[1];", "mapPos[1];"), (D, "D_803B8370[2];", "mapPos[2];")],
}
