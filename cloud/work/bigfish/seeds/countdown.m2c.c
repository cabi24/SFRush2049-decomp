M2C_UNK finish_state_normal();                      /* extern */
M2C_UNK func_800014f0(f32);                         /* extern */
s32 func_800015bc();                                /* extern */
M2C_UNK func_80002790(u8 *, M2C_UNK, M2C_UNK, s8);  /* extern */
M2C_UNK func_800AB18C(s32, s32);                    /* extern */
M2C_UNK func_800B61A8(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_800C813C(M2C_UNK, M2C_UNK);            /* extern */
s32 func_800CF604(s16, s16);                        /* extern */
M2C_UNK func_800D5374();                            /* extern */
M2C_UNK func_800D5828(s16);                         /* extern */
M2C_UNK func_800D6160();                            /* extern */
M2C_UNK func_800E762C(s32);                         /* extern */
s32 func_800E7DD0();                                /* extern */
M2C_UNK func_800F7F3C();                            /* extern */
M2C_UNK func_800FBE30();                            /* extern */
M2C_UNK func_800FBE60();                            /* extern */
f32 func_800FBED8();                                /* extern */
M2C_UNK func_800FBF2C();                            /* extern */
M2C_UNK set_race_state(M2C_UNK);                    /* extern */
s32 setup_state_main(M2C_UNK, M2C_UNK);             /* extern */

void countdown(void) {
    D_8014A250_Record *var_v0_5;
    GameCar *temp_v0_3;
    GameCar *var_v1;
    GameCar *var_v1_2;
    InputRecord *var_v0_4;
    InputRecord *var_v0_6;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 var_s0;
    s16 var_s0_5;
    s32 *var_v0_2;
    s32 temp_v1;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_6;
    s32 var_s1;
    s32 var_s2;
    s32 var_v0;
    s32 var_v0_3;
    s8 temp_t6;
    s8 temp_t7;
    u8 *temp_v0;
    u8 *var_a0;
    u8 *var_s0_2;
    u8 temp_v1_2;
    void **temp_v0_2;

    var_v0 = state_word_a;
    if (var_v0 & 0x40000) {
        D_801174BC = 1;
        init_state_begin();
        init_state_continue();
        D_80150F14 = (u8) (s8) D_80150EFC;
        state_word_b = 0x80000;
        goto block_3;
    }
    if (var_v0 & 0x80000) {
block_3:
        if (setup_state_main(1, 0) != 0) {
            D_801146F0 = 1;
            state_word_b = 0x200000;
            set_race_state(0);
            goto block_7;
        }
        func_800C813C(0, 1);
        goto block_134;
    }
    if (var_v0 & 0x200000) {
block_7:
        if ((s8) D_80114650 != 0) {

        } else {
            camera_race_setup();
            race_init_helper();
            race_setup_1();
            race_setup_2((s16) D_8014A250.unk7C6);
            if ((s8) D_801146F0 != 0) {
                func_800014f0(3.5f);
                func_800E762C((s32) (3.5f * D_8002AFB4));
                D_801146F0 = 0;
            }
            func_800FBF2C();
            func_800FBE30();
            func_800C813C(0, 1);
        }
        goto block_134;
    }
    if (var_v0 & 0x100000) {
        if (gameplay_mode == 1) {
            func_800014f0(1800.0f);
        } else if (gameplay_mode == 4) {
            if (D_801407BC == 0) {
                func_800014f0((f32) (D_80140804 * 0x3C));
            } else {
                func_800014f0(600.0f);
            }
        } else if (gameplay_mode == 5) {
            func_800014f0(300.0f);
        } else if (gameplay_mode == 6) {
            if (D_80140B08 == 1) {
                func_800014f0((f32) (D_80140BD8 * 0x3C));
            } else {
                func_800014f0(1200.0f);
            }
        } else {
            func_800014f0(func_800FBED8());
        }
        func_800B61A8(0x4E, 0, 1, 0);
        func_800D6160();
        func_800FBE60();
        state_word_b = 0x400000;
        D_8014401C = D_801543CC;
        goto block_134;
    }
    if (var_v0 & 0x400000) {
        if ((D_8014401C + 3.0f) < D_801543CC) {
            func_800C813C(0, 1);
            D_8014401C = D_801543CC;
        }
        render_viewport_init();
        func_800FBE30();
        if ((s8) D_80142699 != 0) {
            func_800F7F3C();
            state_word_b = 0x01000000;
            ghost_race_setup();
            records_screen();
        } else if ((s8) D_8013FECB != 0) {
            temp_a1 = M2C_FIELD(&active_player_count, s16 *, 0);
            var_s2 = 1;
            var_s0 = 0;
            if (temp_a1 > 0) {
                do {
                    if ((s8) D_80142760 == 0) {
                        if (func_800CF604(var_s0, temp_a1) == 0) {
                            var_s2 = 0;
                        }
                    }
                    var_s0 += 1;
                } while (var_s0 < temp_a1);
            }
            if ((func_800E7DD0() != 0) && (var_s2 != 0)) {
                if (gameplay_mode == 4) {
                    func_800F7F3C();
                }
                state_word_b = 0x01000000;
                ghost_race_setup();
                records_screen();
            }
            if ((func_800015bc() == 0) && (gameplay_mode != 4) && (gameplay_mode != 5) && (gameplay_mode != 6)) {
                D_8013FECB = 0;
                D_80142690 = 0;
            }
        } else if (gameplay_mode == 4) {
            switch (D_801407BC) {                   /* irregular */
            case 0:
                if (func_800015bc() != 0) {
                    D_8013FECB = 1;
                    D_80142690 = 1;
                }
                break;
            case 1:
                temp_a1_2 = *(s16 *)0x80150000;
                if (temp_a1_2 > 0) {
                    var_v0_2 = &D_8015204C;
loop_59:
                    if (D_8015204C >= D_80140A00) {
                        D_8013FECB = 1;
                        D_80142690 = 1;
                    }
                    if ((u32) (var_v0_2 + 0x1E) < (u32) ((temp_a1_2 * 0x78) + 0x80152038)) {
                        var_v0_2 = &D_801520C4;
                        goto loop_59;
                    }
                }
                break;
            case 2:
                var_v0_3 = 0;
                if (M2C_FIELD(&active_player_count, s16 *, -0x5EF8) > 0) {
                    var_v1 = player_array;
loop_67:
                    if ((s32) M2C_FIELD(M2C_FIELD(player_array, void **, 0x380), u16 *, 0x40) >= D_80140AD8) {
                        var_v1 = (GameCar *) &D_80152F29;
                        ((GameCar *) &D_80152F29)->pad000[0] = 1;
                        var_v0_3 += 1;
                    }
                    if ((u32) &var_v1[1] < (u32) &player_array[M2C_FIELD(&active_player_count, s16 *, -0x5EF8)]) {
                        var_v1 = (GameCar *) &D_80153308;
                        goto loop_67;
                    }
                }
                if (var_v0_3 == M2C_FIELD(&active_player_count, s16 *, -0x5EF8)) {
                    D_8013FECB = 1;
                    D_80142690 = 1;
                }
                break;
            }
        } else if (gameplay_mode == 6) {
            if (D_80140B08 != 0) {
                if ((D_80140B08 == 1) && (func_800015bc() != 0)) {
                    func_800F7F3C();
                    D_8013FECB = 1;
                    D_80142690 = 1;
                }
            } else {
                func_80002790(&D_8015256C, 0, 4, (s8) D_8013FECB);
                var_s0_2 = &D_8015256C;
                if (M2C_FIELD(&active_player_count, s16 *, 0) > 0) {
                    var_a0 = &D_8012E67C;
                    var_v1_2 = player_array;
                    do {
                        temp_t7 = (s8) *var_a0;
                        temp_t6 = (s8) var_v1_2->unk3A3;
                        var_a0 += 1;
                        temp_v0 = &(&D_8015256C)[temp_t7];
                        var_v1_2 = var_v1_2 + 1;
                        *temp_v0 = (s8) *temp_v0 + temp_t6;
                    } while ((u32) var_a0 < (u32) &(&D_8012E67C)[M2C_FIELD(&active_player_count, s16 *, 0)]);
                }
                do {
                    temp_v1 = D_80142510;
                    if (((s8) D_80114654 == 0) && (temp_v1 == ((s8) *var_s0_2 + 1))) {
                        D_80114654 = 1;
                        func_800B61A8(2, 0, 2, 0);
                    }
                    if ((s8) *var_s0_2 >= temp_v1) {
                        func_800F7F3C();
                        D_8013FECB = 1;
                        D_80142690 = 1;
                    }
                    var_s0_2 += 1;
                } while (var_s0_2 != (u8 *)0x80152570);
            }
        } else if ((gameplay_mode == 0) || (gameplay_mode == 1) || (gameplay_mode == 3)) {
            var_s0_3 = 0;
            if (M2C_FIELD(&active_player_count, s16 *, 0) > 0) {
                var_v0_4 = &input_rec0;
loop_96:
                if ((s8) player_array[var_v0_4->pad00[0]].pad0EC[3] == 0) {

                } else {
                    var_s0_3 += 1;
                    var_v0_4 = (InputRecord *) ((u8 *) var_v0_4 + 0x4C);
                    if (var_s0_3 >= M2C_FIELD(&active_player_count, s16 *, 0)) {
                        goto block_100;
                    }
                    goto loop_96;
                }
            } else {
block_100:
                D_80143F10 = 0;
                if (M2C_FIELD(&active_player_count, s16 *, 0) > 0) {
                    var_v0_5 = &D_8014A250;
loop_102:
                    if ((s16) D_80152734 == (s8) D_8014A250.unk7E8) {
                        D_80143F10 = 1;
                    }
                    if ((u32) ((D_8014A250_Record *) ((u8 *) var_v0_5 + 0x808)) < (u32) ((D_8014A250_Record *) ((M2C_FIELD(&active_player_count, s16 *, 0) * 0x808) + (u8 *) &D_8014A250))) {
                        var_v0_5 = (D_8014A250_Record *) &D_8014B240;
                        goto loop_102;
                    }
                }
                if ((s8) D_8013FECB == 0) {
                    if ((s8) D_80142760 != 0) {
                        D_801525F4 = 0.0f;
                    }
                    D_8013FECB = 1;
                    func_800F7F3C();
                    speed_set();
                }
            }
        } else if (func_800015bc() != 0) {
            if ((s8) D_8013FECB == 0) {
                D_8013FECB = 1;
                func_800F7F3C();
                speed_set();
            }
            var_v0_6 = &input_rec0;
            var_s0_4 = 0;
            if (M2C_FIELD(&active_player_count, s16 *, 0) > 0) {
loop_117:
                temp_v1_2 = var_v0_6->pad00[0];
                if ((s16) D_80152734 != M2C_FIELD(((D_8014A250_Record *) ((u8 *) &D_8014A250 + (temp_v1_2 * 0x808))), s8 *, 0x7E8)) {
                    if ((gameplay_mode != 2) || (temp_v0_2 = M2C_FIELD(((temp_v1_2 * 4) + 0x80150000), void ***, 0x2698), (temp_v0_2 == NULL)) || (M2C_FIELD(*M2C_FIELD(*temp_v0_2, void ***, 0x28), s8 *, 5) < 0)) {
                        func_800B61A8(0x16, 0, 1, 0);
                    }
                    D_80142690 = 1;
                } else {
                    var_s0_4 += 1;
                    var_v0_6 = (InputRecord *) ((u8 *) var_v0_6 + 0x4C);
                    if (var_s0_4 < M2C_FIELD(&active_player_count, s16 *, 0)) {
                        goto loop_117;
                    }
                }
            }
        }
        goto block_134;
    }
    if (var_v0 & 0x01000000) {
        var_s0_5 = 0;
        if ((s8) D_80152744 > 0) {
            do {
                func_800D5828(M2C_FIELD(((D_8014A250_Record *) ((u8 *) &D_8014A250 + (var_s0_5 * 0x808))), s16 *, 0x7C6));
                temp_v0_3 = &player_array[var_s0_5];
                temp_v0_3->unkE8 &= ~8;
                cpak_read((s8) temp_v0_3->unk35B);
                var_s0_5 += 1;
            } while (var_s0_5 < (s8) D_80152744);
        }
        players_frame_update();
        func_800D5374();
        func_800014f0(4.0f);
        state_word_b = 0x02000000;
        goto block_134;
    }
    if (var_v0 & 0x02000000) {
        finish_state_normal();
        goto block_134;
    }
    if (var_v0 & 0x800000) {
        finish_state_alt();
block_134:
        var_v0 = state_word_a;
    }
    if (var_v0 & 0x600000) {
        var_s0_6 = 0;
        if ((s16) D_80151AD0 > 0) {
            var_s1 = 0x80150B94;
            do {
                func_800AB18C(var_s0_6, var_s1);
                var_s0_6 += 1;
                var_s1 += 0x98;
            } while (var_s0_6 < (s16) D_80151AD0);
        }
    }
}
