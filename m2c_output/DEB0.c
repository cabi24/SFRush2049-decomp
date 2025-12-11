s32 func_8000D2B0(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0_11;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 temp_v1_5;
    s32 temp_a3;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t0_3;
    s32 temp_t0_4;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t1_3;
    s32 temp_t1_4;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t4;
    s32 temp_t4_2;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_v0;
    s32 temp_v0_13;
    s32 temp_v0_14;
    s32 temp_v0_15;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v0_9;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s8 *var_a0_3;
    s8 temp_v0_10;
    s8 temp_v0_12;
    s8 temp_v0_16;
    s8 temp_v0_2;
    s8 temp_v0_4;
    s8 temp_v0_8;
    void *var_a1_3;

    var_a0 = arg0;
    var_a1 = arg1;
    var_a2 = arg2;
    temp_a3 = var_a1;
    if ((var_a2 != 0) && (var_a0 != var_a1)) {
        if ((var_a1 >= var_a0) && (var_a1 < (var_a0 + var_a2))) {
            var_a0_2 = var_a0 + var_a2;
            var_a1_2 = var_a1 + var_a2;
            if (var_a2 >= 0x10) {
                temp_v0 = var_a0_2 & 3;
                if (temp_v0 == (var_a1_2 & 3)) {
                    switch (temp_v0) {              /* switch 1; irregular */
                    case 0:                         /* switch 1 */
                        break;
                    default:                        /* switch 1 */
                        temp_v0_2 = var_a0_2->unk-1;
                        var_a0_2 -= 1;
                        var_a1_2 -= 1;
                        var_a2 -= 1;
                        *var_a1_2 = temp_v0_2;
                        break;
                    case 2:                         /* switch 1 */
                        temp_v0_3 = var_a0_2->unk-2;
                        var_a0_2 -= 2;
                        var_a1_2 -= 2;
                        var_a2 -= 2;
                        *var_a1_2 = temp_v0_3;
                        break;
                    case 3:                         /* switch 1 */
                        temp_v0_4 = var_a0_2->unk-1;
                        temp_v1 = var_a0_2->unk-3;
                        var_a0_2 -= 3;
                        var_a1_2 -= 3;
                        var_a2 -= 3;
                        var_a1_2->unk2 = temp_v0_4;
                        var_a1_2->unk0 = temp_v1;
                        break;
                    }
loop_35:
                    if (var_a2 >= 0x20) {
                        temp_v0_5 = var_a0_2->unk-4;
                        temp_v1_2 = var_a0_2->unk-8;
                        temp_t0 = var_a0_2->unk-C;
                        temp_t1 = var_a0_2->unk-10;
                        temp_t2 = var_a0_2->unk-14;
                        temp_t3 = var_a0_2->unk-18;
                        temp_t4 = var_a0_2->unk-1C;
                        temp_t5 = var_a0_2->unk-20;
                        var_a0_2 -= 0x20;
                        var_a1_2 -= 0x20;
                        var_a2 -= 0x20;
                        var_a1_2->unk1C = temp_v0_5;
                        var_a1_2->unk18 = temp_v1_2;
                        var_a1_2->unk14 = temp_t0;
                        var_a1_2->unk10 = temp_t1;
                        var_a1_2->unkC = temp_t2;
                        var_a1_2->unk8 = temp_t3;
                        var_a1_2->unk4 = temp_t4;
                        var_a1_2->unk0 = temp_t5;
                        goto loop_35;
                    }
loop_37:
                    if (var_a2 >= 0x10) {
                        temp_v0_6 = var_a0_2->unk-4;
                        temp_v1_3 = var_a0_2->unk-8;
                        temp_t0_2 = var_a0_2->unk-C;
                        temp_t1_2 = var_a0_2->unk-10;
                        var_a0_2 -= 0x10;
                        var_a1_2 -= 0x10;
                        var_a2 -= 0x10;
                        var_a1_2->unkC = temp_v0_6;
                        var_a1_2->unk8 = temp_v1_3;
                        var_a1_2->unk4 = temp_t0_2;
                        var_a1_2->unk0 = temp_t1_2;
                        goto loop_37;
                    }
loop_39:
                    if (var_a2 >= 4) {
                        temp_v0_7 = var_a0_2->unk-4;
                        var_a0_2 -= 4;
                        var_a1_2 -= 4;
                        var_a2 -= 4;
                        *var_a1_2 = temp_v0_7;
                        goto loop_39;
                    }
                }
            }
            if (var_a2 != 0) {
                var_a0_3 = var_a0_2 - 1;
                var_a1_3 = var_a1_2 - 1;
                temp_v1_4 = var_a0_3 - var_a2;
                do {
                    temp_v0_8 = *var_a0_3;
                    var_a0_3 -= 1;
                    var_a1_3 -= 1;
                    var_a1_3->unk1 = temp_v0_8;
                } while (var_a0_3 != temp_v1_4);
                return temp_a3;
            }
            /* Duplicate return node #10. Try simplifying control flow for better match */
            return temp_a3;
        }
        if (var_a2 >= 0x10) {
            temp_v0_9 = var_a0 & 3;
            if (temp_v0_9 == (var_a1 & 3)) {
                switch (temp_v0_9) {                /* irregular */
                case 0:
                    break;
                default:
                    temp_v0_10 = var_a0->unk0;
                    var_a0 += 1;
                    var_a1 += 1;
                    var_a2 -= 1;
                    var_a1->unk-1 = temp_v0_10;
                    break;
                case 2:
                    temp_v0_11 = (s16) var_a0->unk0;
                    var_a0 += 2;
                    var_a1 += 2;
                    var_a2 -= 2;
                    var_a1->unk-2 = temp_v0_11;
                    break;
                case 1:
                    temp_v0_12 = var_a0->unk0;
                    temp_v1_5 = var_a0->unk1;
                    var_a0 += 3;
                    var_a1 += 3;
                    var_a2 -= 3;
                    var_a1->unk-3 = temp_v0_12;
                    var_a1->unk-2 = temp_v1_5;
                    break;
                }
loop_17:
                if (var_a2 >= 0x20) {
                    temp_v0_13 = var_a0->unk0;
                    temp_v1_6 = var_a0->unk4;
                    temp_t0_3 = var_a0->unk8;
                    temp_t1_3 = var_a0->unkC;
                    temp_t2_2 = var_a0->unk10;
                    temp_t3_2 = var_a0->unk14;
                    temp_t4_2 = var_a0->unk18;
                    temp_t5_2 = var_a0->unk1C;
                    var_a0 += 0x20;
                    var_a1 += 0x20;
                    var_a2 -= 0x20;
                    var_a1->unk-20 = temp_v0_13;
                    var_a1->unk-1C = temp_v1_6;
                    var_a1->unk-18 = temp_t0_3;
                    var_a1->unk-14 = temp_t1_3;
                    var_a1->unk-10 = temp_t2_2;
                    var_a1->unk-C = temp_t3_2;
                    var_a1->unk-8 = temp_t4_2;
                    var_a1->unk-4 = temp_t5_2;
                    goto loop_17;
                }
loop_19:
                if (var_a2 >= 0x10) {
                    temp_v0_14 = var_a0->unk0;
                    temp_v1_7 = var_a0->unk4;
                    temp_t0_4 = var_a0->unk8;
                    temp_t1_4 = var_a0->unkC;
                    var_a0 += 0x10;
                    var_a1 += 0x10;
                    var_a2 -= 0x10;
                    var_a1->unk-10 = temp_v0_14;
                    var_a1->unk-C = temp_v1_7;
                    var_a1->unk-8 = temp_t0_4;
                    var_a1->unk-4 = temp_t1_4;
                    goto loop_19;
                }
loop_21:
                if (var_a2 >= 4) {
                    temp_v0_15 = *var_a0;
                    var_a0 += 4;
                    var_a1 += 4;
                    var_a2 -= 4;
                    var_a1->unk-4 = temp_v0_15;
                    goto loop_21;
                }
            }
        }
        if (var_a2 != 0) {
            temp_v1_8 = var_a0 + var_a2;
            do {
                temp_v0_16 = *var_a0;
                var_a0 += 1;
                var_a1 += 1;
                var_a1->unk-1 = temp_v0_16;
            } while (var_a0 != temp_v1_8);
        }
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return temp_a3;
    }
    return temp_a3;
}
