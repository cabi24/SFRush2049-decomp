void func_800F45F8(u8 *arg0, f32 arg1, s32 arg2, s32 arg3) {
    s32 sp30;
    s32 sp2C;
    void *sp1C;
    s32 temp_t2;
    s32 temp_t5;
    s32 temp_v0_3;
    s32 temp_v1_5;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_t1;
    s32 var_v1;
    u8 *var_a0;
    u8 temp_t7;
    u8 temp_v0;
    u8 temp_v0_10;
    u8 temp_v0_11;
    u8 temp_v0_2;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v0_6;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 temp_v0_9;
    u8 temp_v1;
    u8 temp_v1_3;
    u8 var_a3_2;
    void *temp_t2_2;
    void *temp_v1_2;
    void *temp_v1_4;

    var_a0 = arg0;
    var_a2 = arg2;
    var_a3 = arg3;
    temp_v0 = *(u8 *)0x801543D4;
    temp_t5 = *(*((temp_v0 * 0x4C) + 0x80150000)->unk-5EA0)->unk2C;
    if (*(s16 *)0x8014A108 == 1) {
        var_v1 = 0;
    } else {
        var_v1 = 2;
    }
    var_a1 = 6 - var_v1;
    temp_t2 = var_a1;
    if ((s32) temp_v0 > 0) {
        temp_v1 = var_a0->unk1;
        temp_v0_2 = var_a0->unk0 ^ temp_v1;
        temp_t7 = temp_v1 ^ temp_v0_2;
        var_a0->unk0 = temp_v0_2;
        var_a0->unk0 = temp_v0_2 ^ temp_t7;
        var_a0->unk1 = temp_t7;
    }
    var_t1 = 0;
    if (var_a1 > 0) {
        var_a1 = (s32) var_a0;
        var_a3 = temp_t5 + 0x6F4;
        do {
            var_t1 += 1;
            var_a2 = 0;
            var_a0 = (u8 *) (*(s8 *)0x80154628 * 3);
loop_8:
            temp_v1_2 = var_a3 + ((s32) var_a0 >> 3);
            temp_v0_3 = (s32) var_a0 & 7;
            var_a2 += 1;
            temp_v1_2->unk14 = (u8) (((*var_a1 & 1) << temp_v0_3) | (temp_v1_2->unk14 & ~(1 << temp_v0_3)));
            var_a0 += 1;
            *var_a1 = (u8) ((u8) *var_a1 >> 1);
            if (var_a2 != 3) {
                goto loop_8;
            }
            var_a1 += 1;
            var_a3 += 9;
        } while (var_t1 != temp_t2);
    }
    temp_t2_2 = temp_t5 + 0x6F4;
    temp_t2_2->unk9 = (s8) (temp_t2_2->unk9 + 1);
    temp_t2_2->unk10 = (f32) (temp_t2_2->unk10 + arg1);
    sp1C = temp_t2_2;
    (? (*)(f32, u8 *, s32, s32, s32))0x800F34D8(arg1, var_a0, var_a1, var_a2, var_a3);
    *(void *)0x80154628 = (s8) (*(void *)0x80154628 - 1);
    if (temp_t2_2->unk9 >= *(s8 *)0x80154640) {
        sp1C = temp_t2_2;
        (? (*)())0x800F43B8();
    }
    if (temp_t2_2->unk8 == 3) {
        if (*(void *)0x8014A108 == 1) {
            var_a3_2 = (void *)0x80154450->unk1;
        } else {
            temp_v0_4 = (void *)0x80154450->unk1;
            temp_v1_3 = (void *)0x80154450->unk4D;
            var_a3_2 = temp_v1_3;
            if ((s32) temp_v0_4 < (s32) temp_v1_3) {
                var_a3_2 = temp_v0_4;
            }
        }
        temp_v0_5 = *(u8 *)0x80154451;
        if (temp_v0_5 == 0) {
            sp30 = 0;
        } else if (temp_v0_5 == 1) {
            sp2C = 0;
        }
        temp_v0_6 = *(u8 *)0x8015449D;
        var_a0_2 = sp30;
        var_a1_2 = sp2C;
        if (temp_v0_6 == 0) {
            var_a0_2 = 1;
        } else if (temp_v0_6 == 1) {
            var_a1_2 = 1;
        }
        temp_v1_4 = (2 * 0x4C) + 0x80154450;
        temp_v0_7 = temp_v1_4->unk1;
        if (temp_v0_7 == 0) {
            var_a0_2 = 2;
        } else if (temp_v0_7 == 1) {
            var_a1_2 = 2;
        }
        temp_v0_8 = temp_v1_4->unk4D;
        if (temp_v0_8 == 0) {
            var_a0_2 = 3;
        } else if (temp_v0_8 == 1) {
            var_a1_2 = 3;
        }
        temp_v0_9 = temp_v1_4->unk99;
        if (temp_v0_9 == 0) {
            var_a0_2 = 4;
        } else if (temp_v0_9 == 1) {
            var_a1_2 = 4;
        }
        temp_v0_10 = temp_v1_4->unkE5;
        if (temp_v0_10 == 0) {
            var_a0_2 = 5;
        } else if (temp_v0_10 == 1) {
            var_a1_2 = 5;
        }
        temp_v0_11 = temp_t2_2->unk7;
        temp_v1_5 = ((var_a0_2 * 0x4C) + 0x80154450)->unk2 - ((var_a1_2 * 0x4C) + 0x80154450)->unk2;
        if (((var_a3_2 == 0) && ((s32) temp_v0_11 < 5) && (temp_v1_5 >= 0x14)) || ((var_a3_2 == 0) && ((s32) temp_v0_11 < 4) && (temp_v1_5 >= 6)) || (((s32) var_a3_2 < 2) && ((s32) temp_v0_11 < 3)) || (((s32) var_a3_2 < 3) && ((s32) temp_v0_11 < 2)) || (((s32) var_a3_2 < 4) && ((s32) temp_v0_11 <= 0))) {
            temp_t2_2->unk7 = (u8) (temp_v0_11 + 1);
        }
    }
    (? (*)(s32))0x800CD748(((*(void *)0x801543D4 * 0x4C) + 0x80150000)->unk-5EA0);
}
