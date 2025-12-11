void func_80008590(s32 arg0, s32 arg1) {
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_a3_4;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a1;

    var_a0 = arg0;
    var_a1 = arg1;
    if (var_a1 >= 0xC) {
        temp_v1 = -var_a0 & 3;
        temp_a1 = var_a1 - temp_v1;
        if (temp_v1 != 0) {
            *var_a0 = (unaligned s32) 0;
            var_a0 += temp_v1;
        }
        temp_a3 = temp_a1 & ~0x1F;
        temp_a1_2 = temp_a1 - temp_a3;
        if (temp_a3 != 0) {
            temp_a3_2 = temp_a3 + var_a0;
            do {
                var_a0 += 0x20;
                var_a0->unk-20 = 0;
                var_a0->unk-1C = 0;
                var_a0->unk-18 = 0;
                var_a0->unk-14 = 0;
                var_a0->unk-10 = 0;
                var_a0->unk-C = 0;
                var_a0->unk-8 = 0;
                var_a0->unk-4 = 0;
            } while (var_a0 != temp_a3_2);
        }
        temp_a3_3 = temp_a1_2 & ~3;
        var_a1 = temp_a1_2 - temp_a3_3;
        if (temp_a3_3 != 0) {
            temp_a3_4 = temp_a3_3 + var_a0;
            do {
                var_a0 += 4;
                var_a0->unk-4 = 0;
            } while (var_a0 != temp_a3_4);
        }
    }
    if (var_a1 > 0) {
        temp_a1_3 = var_a1 + var_a0;
        do {
            var_a0 += 1;
            var_a0->unk-1 = 0;
        } while (var_a0 != temp_a1_3);
    }
}
