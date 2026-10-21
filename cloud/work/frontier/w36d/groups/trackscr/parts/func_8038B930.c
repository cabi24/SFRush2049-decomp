/* clearTrackMaps (MAT3 trackscr.c, GC 0x8008bfac), inlined into _UninitScreen */
void func_8038B930(void)
{
    s32 i,model;
    RomObjDef *def;
    if (D_803BAAE0) {
        for (i = 0; i < 20; i++) {
            if (D_803B7CE4[i].handle != -1) {
                func_80090254((s16)D_803B7CE4[i].handle);
                D_803B7CE4[i].handle = -1;
            }
        }
        while (D_8002EB70) {}
        for (i = 0; i < 7; i++) {
            model = string_copy_format(D_803B7738[i < 6 ? i : 19],0,D_80140BDC - 1,1);
            def = &D_801161F4[model >> 10].romObjDefs[model & 0x3FF];
            def->lod[0] = D_803BAB58[i];
            if (D_803BABC8[i]) func_800960D4(D_803BABC8[i]);
            D_803BABC8[i] = 0;
            if (D_803BABE8[i]) func_800960D4(D_803BABE8[i]);
            D_803BABE8[i] = 0;
            D_803BAAE8[i].displayList = 0;
            D_803BAAE8[i].verts = 0;
        }
        D_803BAAE0 = 0;
    }
}
