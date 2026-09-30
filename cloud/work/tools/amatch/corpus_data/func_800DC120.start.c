s32 func_800DC120(void) {
    s32 var_a0;
    u32 temp_a3;
    u32 var_a2;
    u32 var_v1;

    if (D_801170F0 == 0) {
        return 0;
    }
    var_v1 = 0;
    var_a0 = 0;
    if (D_801170EC != 0) {
        temp_a3 = 1 << D_801170F0;
        do {
            var_a2 = *((s32 *) ((u8 *) &D_8012E618 + (var_v1 >> 3))) & (1 << (var_v1 & 7));
            var_v1 += 1;
            if (var_a2 >= temp_a3) {
                do {
                    var_a2 = var_a2 >> D_801170F0;
                } while (var_a2 >= temp_a3);
            }
            var_a0 += var_a2;
        } while (var_v1 < (u16) D_801170EC);
    }
    return var_a0;
}