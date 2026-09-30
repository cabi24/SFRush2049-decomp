M2C_UNK attract_demo_handler(s32, s32);             /* extern */
M2C_UNK attract_mode_handler();                     /* extern */
M2C_UNK attract_video_handler(s32);                 /* extern */
M2C_UNK audio_distance_atten(s32);                  /* extern */
M2C_UNK camera_auto_follow(s16, s16, s16, s16, s32, s32, M2C_UNK *); /* extern */
M2C_UNK frame_sync(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_80004990(M2C_UNK *, M2C_UNK, s32);     /* extern */
M2C_UNK func_80007270(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */
M2C_UNK func_800075e0(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */
M2C_UNK func_800BE4F0(M2C_UNK *, s32);              /* extern */
M2C_UNK func_800BE6A4(M2C_UNK *, s32);              /* extern */
M2C_UNK func_800DCCE0();                            /* extern */
M2C_UNK func_800DCD58(s32, s32);                    /* extern */
M2C_UNK func_800DD0C0();                            /* extern */
M2C_UNK func_800DD45C(s32);                         /* extern */
u32 object_utility(M2C_UNK *, M2C_UNK);             /* extern */

void control_settings(s32 arg0, s8 *arg1, s8 *arg2, s8 *arg3, s32 (*arg4)()) {
    s32 sp764;
    M2C_UNK sp718;
    M2C_UNK sp5F0;
    M2C_UNK sp4A8;
    M2C_UNK sp398;
    M2C_UNK sp290;
    M2C_UNK sp188;
    M2C_UNK sp80;
    s32 sp5C;
    void *sp58;
    s16 temp_a2;
    s16 temp_s0;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_t8;
    s32 temp_t9;
    s32 var_a1;
    s32 var_s0;
    s32 var_s4;
    void *temp_v1_2;
    void *var_s1;
    void *var_s1_2;
    void *var_s1_3;

    if ((arg4 != NULL) && (arg4() != 0)) {
        func_800DD45C(arg0);
    }
    temp_t8 = (arg0 * 0x58) + 0x80153FD8;
    sp5C = temp_t8;
    var_s1 = temp_t8 + 0x2C;
    var_s4 = 1;
loop_4:
    if (M2C_FIELD(var_s1, s32 *, 0x14) == 0) {
        if (M2C_FIELD(var_s1, s8 *, 0) == 0) {
            M2C_FIELD(var_s1, s8 *, 0) = 1;
            *arg3 = 1;
            *arg2 = 0;
            *arg1 = 0;
            goto block_58;
        }
        var_s4 -= 1;
        var_s1 = (u8 *) var_s1 - 0x2C;
        if (var_s4 < 0) {
            goto block_8;
        }
        goto loop_4;
    }
block_8:
    if (var_s4 >= 0) {
        M2C_FIELD(var_s1, s32 *, 0x20) = (s32) *(s32 *)0x8015694C;
        M2C_FIELD(var_s1, s32 *, 0x24) = (s32) *(s32 *)0x80156944;
        func_80007270(0x801461D0, 0, 1);
        sp58 = var_s1;
        slot_state_setup();
        func_800075e0(0x801461D0, 0, 0);
        dispatch_handler(1);
        func_80007270(0x801461D0, 0, 1);
        sp58 = var_s1;
        slot_state_setup();
        var_s1_2 = var_s1;
        func_800075e0(0x801461D0, 0, 0);
        dispatch_handler(1);
        func_80004990(&sp718, 0x80121060, arg0 + 1);
        temp_s0 = ((s16) M2C_FIELD(var_s1_2, s16 *, 0x1C) / 2) + M2C_FIELD(var_s1_2, s16 *, 0x18);
        state_utility((s16) (temp_s0 - (object_utility(&sp718, -1) >> 1)), (s32) M2C_FIELD(var_s1_2, s16 *, 0x1A), &sp718);
        temp_t9 = M2C_FIELD(var_s1_2, s32 *, 0x14);
        switch (temp_t9) {
        case 1:
            sp58 = var_s1_2;
            func_800DCD58(arg0, var_s4);
            func_80007270(0x801461D0, 0, 1);
            sp58 = var_s1_2;
            slot_state_setup();
            func_800075e0(0x801461D0, 0, 0);
            dispatch_handler(1);
            func_800BE6A4(&sp5F0, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x194));
            func_800BE4F0(&sp5F0, 0x80121070);
            func_800BE4F0(&sp5F0, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x198));
            temp_a2 = M2C_FIELD(var_s1_2, s16 *, 0x1C);
            temp_v1 = M2C_FIELD(var_s1_2, s16 *, 0x1E) - 0x14;
            M2C_FIELD((void *)0x80118E20, s32 *, 0) = 1;
            M2C_FIELD((void *)0x80118E20, s32 *, 4) = 1;
            camera_auto_follow((s16) ((temp_a2 / 2) + M2C_FIELD(var_s1_2, s16 *, 0x18)), (s16) ((temp_v1 / 2) + M2C_FIELD(var_s1_2, s16 *, 0x1A) + 0x14), temp_a2, temp_v1, -1, 0, &sp5F0);
            M2C_FIELD((void *)0x80118E20, s32 *, 4) = 3;
            M2C_FIELD((void *)0x80118E20, s32 *, 0) = 0;
            func_80007270(0x801461D0, 0, 1);
            sp58 = var_s1_2;
            slot_state_setup();
            func_800075e0(0x801461D0, 0, 0);
            break;
        case 2:
            if (!(state_word_a & 0x7C03FFFE)) {
                sp58 = var_s1_2;
                func_800DCD58(arg0, var_s4);
            } else {
                func_800DCCE0();
            }
            sp58 = var_s1_2;
            sp764 = var_s4;
            func_800DD0C0();
            break;
        case 3:
            sp58 = var_s1_2;
            func_800DCD58(arg0, var_s4);
            if (M2C_FIELD(var_s1_2, s8 *, 0x10) == 0) {
                var_a1 = M2C_FIELD(M2C_ERROR(/* Read from unset register $t9 */), s32 *, 0x1AC);
            } else {
                var_a1 = M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1A8);
            }
            func_800BE6A4(&sp4A8, var_a1);
            func_800BE4F0(&sp4A8, 0x80121074);
            func_800BE4F0(&sp4A8, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1B0));
            sp764 = var_s4;
            sp58 = var_s1_2;
            attract_video_handler(M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x3B0));
            break;
        case 4:
            func_800DCCE0();
            func_800BE6A4(&sp398, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1B4));
            func_800BE4F0(&sp398, 0x80121078);
            func_800BE4F0(&sp398, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1B8));
            sp58 = var_s1_2;
            sp764 = var_s4;
            attract_mode_handler();
            break;
        case 15:
            func_800DCCE0();
            func_800BE6A4(&sp290, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1BC));
            func_800BE4F0(&sp290, 0x80121088);
            func_800BE4F0(&sp290, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x198));
            sp58 = var_s1_2;
            sp764 = var_s4;
            attract_mode_handler();
            break;
        case 5:
        case 7:
            sp58 = var_s1_2;
            func_800DCD58(arg0, var_s4);
            sp764 = var_s4;
            attract_video_handler(M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x3B0));
            break;
        case 8:
            sp58 = var_s1_2;
            func_800DCD58(arg0, var_s4);
            sp764 = var_s4;
            attract_video_handler(M2C_FIELD((M2C_FIELD(&countdown_object, s32 *, 0x10) + (M2C_FIELD(countdown_state.unkC, u16 *, 0x30) * 4)), s32 *, 4));
            break;
        case 6:
            sp58 = var_s1_2;
            func_800DCD58(arg0, var_s4);
            func_800BE6A4(&sp188, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1C4));
            func_800BE4F0(&sp188, 0x8012107C);
            func_800BE4F0(&sp188, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1C8));
            sp58 = var_s1_2;
            sp764 = var_s4;
            attract_mode_handler();
            break;
        case 9:
            func_800DCCE0();
            func_800BE6A4(&sp80, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1CC));
            func_800BE4F0(&sp80, 0x80121080);
            func_800BE4F0(&sp80, M2C_FIELD(M2C_FIELD(&countdown_object, CountdownObject **, 0), s32 *, 0x1B8));
            sp58 = var_s1_2;
            sp764 = var_s4;
            attract_mode_handler();
            break;
        case 10:
            temp_a0 = M2C_FIELD(var_s1_2, s32 *, 0x20);
            if (temp_a0 & 0x800) {
                audio_distance_atten(temp_a0);
                if (M2C_FIELD(var_s1_2, s8 *, 1) != 0) {
                    M2C_FIELD(var_s1_2, s8 *, 1) = 0;
                    M2C_FIELD(var_s1_2, s8 *, 2) = 1;
                } else if (M2C_FIELD(var_s1_2, s8 *, 2) != 0) {
                    M2C_FIELD(var_s1_2, s8 *, 2) = 0;
                } else {
                    M2C_FIELD(var_s1_2, s8 *, 1) = 1;
                }
            }
            if (M2C_FIELD(var_s1_2, s32 *, 0x20) & 0x400) {
                audio_distance_atten(M2C_FIELD(var_s1_2, s32 *, 0x20));
                if (M2C_FIELD(var_s1_2, s8 *, 1) != 0) {
                    M2C_FIELD(var_s1_2, s8 *, 1) = 0;
                } else if (M2C_FIELD(var_s1_2, s8 *, 2) != 0) {
                    M2C_FIELD(var_s1_2, s8 *, 2) = 0;
                    M2C_FIELD(var_s1_2, s8 *, 1) = 1;
                } else {
                    M2C_FIELD(var_s1_2, s8 *, 2) = 1;
                }
            }
            if (M2C_FIELD(var_s1_2, s32 *, 0x20) & 2) {
                frame_sync(0x25, 0, 1, 0);
                M2C_FIELD(var_s1_2, s8 *, 0) = 1;
            }
            temp_v1_2 = M2C_FIELD(&countdown_object, s32 *, 0x10) + (M2C_FIELD(M2C_FIELD(&countdown_object, void **, 0xC), u16 *, 0x34) * 4);
            sp764 = var_s4;
            sp58 = var_s1_2;
            attract_demo_handler(M2C_FIELD(temp_v1_2, s32 *, 0), M2C_FIELD(temp_v1_2, s32 *, 4));
            break;
        case 11:
            if ((u32) M2C_FIELD(var_s1_2, u32 *, 0x28) < (u32) game_loop_tick) {
                M2C_FIELD(var_s1_2, s8 *, 0) = 1;
            }
            sp764 = var_s4;
            sp58 = var_s1_2;
            attract_mode_handler();
            break;
        case 12:
            if ((u32) M2C_FIELD(var_s1_2, u32 *, 0x28) < (u32) game_loop_tick) {
                M2C_FIELD(var_s1_2, s8 *, 0) = 1;
            }
            sp764 = var_s4;
            sp58 = var_s1_2;
            attract_mode_handler();
            break;
        case 13:
            if ((u32) M2C_FIELD(var_s1_2, u32 *, 0x28) < (u32) game_loop_tick) {
                M2C_FIELD(var_s1_2, s8 *, 0) = 1;
            }
            sp764 = var_s4;
            sp58 = var_s1_2;
            attract_mode_handler();
            break;
        case 14:
            M2C_FIELD(var_s1_2, s8 *, 0) = 1;
            break;
        }
        *arg3 = M2C_FIELD(var_s1_2, s8 *, 0);
        if (M2C_FIELD(var_s1_2, s8 *, 0) != 0) {
            M2C_FIELD(var_s1_2, s32 *, 0x14) = 0;
            *arg1 = M2C_FIELD(var_s1_2, s8 *, 1);
            *arg2 = M2C_FIELD(var_s1_2, s8 *, 2);
block_58:
            var_s0 = 0;
            var_s1_3 = sp5C + (var_s4 * 0x2C);
            do {
                player_state_set(var_s0, (s32) M2C_FIELD(var_s1_3, s8 *, 4));
                player_mode_set(var_s0, (s32) M2C_FIELD(var_s1_3, s8 *, 8));
                var_s0 += 1;
                var_s1_3 = (u8 *) var_s1_3 + 1;
            } while (var_s0 != 4);
            process_inputs();
        }
    }
}
