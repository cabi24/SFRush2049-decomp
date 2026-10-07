/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH: func_800E56F8, N64 update_model-related controller/model step.
 * Canonical result 5/359 differing words; exact 1,436-byte extent and 24-byte
 * frame, no extra words, unresolved symbols, unverified sites, or errors.
 * Best archived baseline was 115/359, reproduced with real callers only.
 *
 * Genuine algorithm ancestry: rushtherock game/mdrive.c:update_model,
 * 845329d7b36f5a384c5625ed9a0aef584ab46139. N64 lifecycle, force feedback,
 * mode, and input-record behavior are native-led additions, not in that donor.
 * Complete prior implementation comes from the existing E56F8 archive.
 *
 * State bytes 0x358/0x359 use their signed GC2 fields for stores as well as
 * reads; native reads are lb and existing caller GC2 has these field types.
 * Actual controller-port loads are assigned to consumed word-sized locals.
 * The pending bonus uses a direct arithmetic expression before its real clear.
 * No new dummy locals, volatile carriers, padding, stand-ins, or assembly.
 * repro.py supplies real E6AF8/E4B58 and their actual path context, with all
 * historical stand-ins and artificial pad arrays removed. No context claims.
 */






void func_800E56F8(s32 ipa_s4) {
    GC2 *temp_s2;
    f32 temp_f0;
    f32 var_f0;
    s32 temp_a0;
    s32 idx;
    s32 port;
    s32 *var_v0_2;
    s32 temp_f6;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    s8 temp_v0;
    s32 var_v1;
    u8 temp_v0_3;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_s3;

    temp_s2 = &((GC2 *)player_array)[ipa_s4];
    temp_s3 = M2C_FIELD(temp_s2, void **, 0x380);
    if (((s8) temp_s2->b359 == 1) && ((temp_s0 = (D_8014A250_Record *) ((ipa_s4 * 0x808) + (u8 *) &D_8014A250), (M2C_FIELD(temp_s0, f32 *, 0x3F0) < 5.0f)) || (M2C_FIELD(temp_s0, s16 *, 0x71C) == 0))) {
        temp_s2->b359 = 2;
        temp_s2->b358 = 0;
        M2C_FIELD(temp_s0, s8 *, 0x640) = 0;
        M2C_FIELD(temp_s0, s16 *, 0x71C) = 0;
        M2C_FIELD(temp_s0, f32 *, 0x3F0) = 0.0f;
        if (temp_s3 != NULL) {
            if (D_80153F08 >= 7) {
                D_80153F08 -= 7;
            }
            port = M2C_FIELD(temp_s3, u8 *, 1);
            *(&D_801440AD + (port * 0x304)) = 0;
            return;
        }
        idx = D_80153F40;
        var_a0 = D_8015274C - 1;
        for (; idx < var_a0; idx++) {
            D_80152808[idx] = D_80152808[idx + 1];
        }
        D_8015274C = var_a0;
        D_80153F40 -= 1;
        D_80153F08 -= 1;
        return;
    }
    temp_s0_2 = (D_8014A250_Record *) ((ipa_s4 * 0x808) + (u8 *) &D_8014A250);
    car_select_handler(ipa_s4);
    func_800D1248(temp_s0_2);
    if (((s8) temp_s2->b358 == 2) && ((s8) temp_s2->b359 <= 0)) {
        func_800D5828(ipa_s4);
        func_800E543C(2, ipa_s4);
    } else if ((M2C_FIELD(temp_s0_2, s8 *, 0x640) != 0) && ((s8) temp_s2->b359 >= 2)) {
        M2C_FIELD(temp_s0_2, s8 *, 0x640) = 0;
    }
    temp_f0 = (M2C_FIELD(temp_s0_2, f32 *, 0x400) * M2C_FIELD(temp_s0_2, f32 *, 0x7F0)) * (((f32) M2C_FIELD(temp_s0_2, s16 *, 0x624) * 0.0199999996f) + 1.0f);
    M2C_FIELD(temp_s0_2, f32 *, 0x7F4) = temp_f0;
    M2C_FIELD(temp_s0_2, f32 *, 0x718) = (f32) (M2C_FIELD(temp_s0_2, f32 *, 0x718) * temp_f0);
    if (M2C_FIELD(temp_s0_2, s16 *, 0x6C4) >= 0) {
        if (temp_s3 != NULL) {
            track_select_handler(temp_s0_2);
            if (state_word_a & 0x400000) {
                port = M2C_FIELD(temp_s3, u8 *, 1);
            *(&D_801440AD + (port * 0x304)) = 0;
            }
        } else {
            track_select_handler(temp_s0_2);
        }
        func_800D4D84((s32) ipa_s4, M2C_FIELD(temp_s0_2, s32 *, 0x714));
    } else if (M2C_FIELD(temp_s0_2, s16 *, 0x71C) != 0) {
        if (temp_s3 != NULL) {
            func_800E3724(temp_s0_2);
            if ((state_word_a & 0x400000) && ((gameplay_mode != 2) || (temp_v1 = (&D_80152698)[M2C_FIELD(temp_s0_2, s16 *, 0x7C6)], (temp_v1 == 0)) || (M2C_FIELD(*M2C_FIELD(M2C_FIELD(temp_v1, void **, 0), void ***, 0x28), s8 *, 5) < 0))) {
                temp_f6 = (s32) ((fabsf(M2C_FIELD(temp_s0_2, f32 *, 0x10)) + fabsf(M2C_FIELD(temp_s0_2, f32 *, 0x14))) + fabsf(M2C_FIELD(temp_s0_2, f32 *, 0x18)));
                var_a0 = temp_f6;
                if (gameplay_mode == 6) {
                    var_a0 /= 2;
                    if ((s8) temp_s2->b358 == 1) {
                        var_a0 += 0x186A0;
                    } else {
                        var_a0 += (M2C_FIELD(temp_s2, s16 *, 0x388) * 0x186A0) / 800;
                        M2C_FIELD(temp_s2, s16 *, 0x388) = 0;
                    }
                }
                var_v0_2 = &D_801163F8;
                var_v1 = 0;
loop_34:
                if (var_a0 >= *var_v0_2) {
                    var_v1 += 1;
                    var_v0_2 = var_v0_2 + 1;
                    if (var_v1 < 0x24) {
                        goto loop_34;
                    }
                }
                temp_v0 = M2C_FIELD(*M2C_FIELD(temp_s3, void ***, 0x48), s8 *, 0x2B);
                if (temp_v0 == 0) {
                    goto block_40;
                }
                if (1 == temp_v0) {
                    var_v1 -= 4;
                    if (var_v1 < 0) {
block_40:
                        var_v1 = 0;
                    }
                }
                temp_a0 = M2C_FIELD(temp_s3, u8 *, 1);
                *(&D_801440AD + (temp_a0 * 0x304)) = var_v1;
            }
        } else {
            func_800E4B58(temp_s0_2);
            func_800E3724(temp_s0_2);
        }
        func_800D4D84((s32) ipa_s4, M2C_FIELD(temp_s0_2, s32 *, 0x714));
    }
    if ((gameplay_mode != 2) || (M2C_FIELD(temp_s0_2, s16 *, 0x7C6) == 0)) {
        func_800E05F0(temp_s0_2);
    }
    if (M2C_FIELD(temp_s0_2, s8 *, 0x640) != 0) {
        M2C_FIELD(temp_s0_2, s32 *, 0x7D4) = (s32) (M2C_FIELD(temp_s0_2, s32 *, 0x7D4) | 0x70);
    } else {
        temp_v0_2 = M2C_FIELD(temp_s0_2, s32 *, 0x7D4);
        if (!(temp_v0_2 & 0x10)) {
            M2C_FIELD(temp_s0_2, s32 *, 0x7D4) = (s32) (temp_v0_2 & ~0x60);
        }
    }
    if (M2C_FIELD(temp_s0_2, s8 *, 0x7EB) == 0) {
        if (M2C_FIELD(temp_s0_2, s16 *, 0x6C4) >= 0) {
            var_f0 = 0.0f;
            M2C_FIELD(temp_s2, f32 *, 0x30C) = M2C_FIELD(temp_s0_2, f32 *, 0x714);
        } else {
            var_f0 = M2C_FIELD(temp_s0_2, f32 *, 0x714) - M2C_FIELD(temp_s2, f32 *, 0x30C);
        }
        if ((((s8) D_8013FECB != 0) || (((gameplay_mode != 6) || (gameplay_mode != 5)) && (var_f0 >= 5.0f)) || (((gameplay_mode == 6) || (gameplay_mode == 5)) && (0.600000024f < var_f0))) && ((temp_v0_3 = *(&D_80153E8F + (M2C_FIELD(temp_s0_2, s16 *, 0x7C6) * 8)), (temp_v0_3 == 0)) || (temp_v0_3 == 6))) {
            M2C_FIELD(temp_s0_2, s8 *, 0x7EA) = 1;
            M2C_FIELD(temp_s0_2, s8 *, 0x7EB) = 1;
            M2C_FIELD(temp_s2, f32 *, 0x30C) = 0.0f;
        }
    }
}
