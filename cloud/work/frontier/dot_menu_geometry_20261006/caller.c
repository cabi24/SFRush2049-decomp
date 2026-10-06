/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research only: base f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2.
 * Reconstructed from the protected D816C/D91A0 instruction boundaries and
 * existing accepted helper interfaces. No original N64 source/TU claim.
 * See README.md for private-ABI and decompiler-correction limits.
 */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
void *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
void drone_target_update(s32);
u32 entity_flags_apply(u32,u32,u32,u8);
s32 func_800A1910(void *,u8 *,s32);
void func_800B4FB0(s32);
void func_800C77C4(void *);
s32 func_800D8078(s8);
s32 func_800D8154(s32);
void func_800D9058(void);
void *memcpy(void *,const void *,u32);
void render_results_screen(void);
void resource_type_select(s32);
void *sound_control(s16,s16,void *,s16);
void sound_handles_clear(s32);
extern s32 gameplay_mode;
extern u8 input_rec0[];
extern s16 D_8014A0F8[],D_8014A100[];
extern s8 D_8014A10C[];
extern u8 D_80149DA8[],D_8013C068[];
extern s8 D_80149DD4[];
extern s8 D_8015418C[];
extern u8 D_80144031[],D_80144036[];
extern void *D_80146150[];
extern u8 D_80112AB8[],D_8011361C[],D_80139300[],D_80113EE0[];

void drone_throttle_calc(s8);
void func_800D816C(s8);
void render_replay_ui(s8);
extern u8 D_80113910[];

#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((u8 *)(p)+(o)))
extern f32 D_80112A98;
extern f32 D_80112A9C;
extern f32 D_80112AA0;
extern f32 D_80112AA4;
extern f32 D_80112AA8;
extern f32 D_80112AAC;
extern f32 D_80112AB0;
extern f32 D_80112AB4;
extern s8 D_80113ED4;
extern void *D_80113ED8;
extern s32 D_80113EDC;
extern s16 D_8014A10A;
extern s16 D_80151AD0;
extern s8 D_80157244;
extern s32 state_word_a;

void func_800D91A0(void) {
    s8 *selection_table;
    s32 spEC;
    s32 spE8;
    s32 spD8;
    s8 *spA4;
    s8 *spA0;
    u8 *sp9C;
    s16 *sp8C;
    s16 *temp_s0_2;
    s16 *var_s7;
    s16 temp_a0_2;
    s16 temp_s0;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s16 var_v0_3;
    s32 temp_a0;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a1;
    s32 temp_a3_2;
    s32 temp_t9;
    s32 var_a0;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_s1;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t0_3;
    s32 var_t0_4;
    s32 var_t0_5;
    s32 var_t3;
    s32 var_v1_3;
    s8 *temp_s1;
    s8 *temp_s3;
    s8 *temp_v0_2;
    s8 *temp_v0_4;
    s8 *temp_v0_5;
    s8 *temp_v0_6;
    s8 *temp_v0_7;
    s8 *temp_v1;
    s8 *var_a0_2;
    s8 *var_a1;
    s8 *var_v0_6;
    s8 *var_v0_7;
    s8 *var_v1;
    s8 *var_v1_4;
    s8 temp_t6;
    s8 temp_t6_2;
    s8 temp_v0_8;
    s8 temp_v1_6;
    s8 var_v0_4;
    s8 var_v0_5;
    u8 *var_v1_2;
    u8 temp_a3;
    u8 temp_s0_3;
    void **temp_v0_10;
    void **temp_v0_3;
    void **temp_v0_9;
    void *temp_v0;
    void *temp_v1_4;
    void *temp_v1_5;
    void *var_v0;
    void *var_v0_2;

    if (D_80113ED4 == 0) {
        if (state_word_a & 0x7C03FFFE) {
            var_s7 = &D_80151AD0;
            var_a0 = 1;
            var_v1 = D_8015418C;
            if (D_80151AD0 > 0) {
                do {
                    temp_t6 = *var_v1;
                    var_v1 += 1;
                    if (temp_t6 == 1) {
                        var_a0 = 0;
                    }
                } while ((u32) var_v1 < (u32) &D_8015418C[D_80151AD0]);
            }
            if (var_a0 == 1) {
                return;
            }
            goto block_15;
        }
        var_s7 = &D_80151AD0;
        var_s1 = 0;
        if (*&D_80151AD0 > 0) {
            spE8 = 0;
            do {
                temp_v0 = D_80113EE0 + (*&D_80151AD0 * 0x60) + var_s1;
                temp_a0 = M2C_FIELD(temp_v0, s32 *, -0x60);
                temp_a1 = M2C_FIELD(temp_v0, s32 *, -0x5C);
                ambient_sound_set(temp_a0, temp_a1, M2C_FIELD(temp_v0, s32 *, -0x58) + temp_a0, M2C_FIELD(temp_v0, s32 *, -0x54) + temp_a1, 0xB0, 0, 0, 0);
                var_s1 += 0x18;
                temp_t9 = spE8 + 1;
                spE8 = temp_t9;
            } while (temp_t9 < *&D_80151AD0);
        }
        if (*&D_80151AD0 < 4) {
            var_a0_2 = &D_8014A10C[*&D_80151AD0];
            do {
                var_a0_2 += 1;
                M2C_FIELD(var_a0_2, s8 *, -1) = 0;
            } while ((u32) var_a0_2 < (u32) &gameplay_mode);
        }
block_15:
        func_800D9058();
        if (state_word_a & 0x7C03FFFE) {
            var_v0 = sound_control(0, 0, D_80112AB8, 0x51);
        } else if (*var_s7 == 1) {
            var_v0 = sound_control(0, 0, D_8011361C, 0x15);
        } else {
            var_v0 = sound_control(0, 0, D_80113910, 0x29);
        }
        D_80113ED8 = var_v0;
        temp_s0 = *var_s7;
        if (temp_s0 == 1) {
            D_80112A98 = 1.0f;
            D_80112A9C = 0.9f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = 0.0f;
            D_80112AA8 = 120.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
        } else if (temp_s0 == 2) {
            D_80112A98 = 1.0f;
            D_80112A9C = 1.3f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = 0.0f;
            D_80112AA8 = 95.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
        } else {
            D_80112A98 = 1.0f;
            D_80112A9C = 1.3f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = -30.0f;
            D_80112AA8 = 95.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
        }
        D_80112AB4 = 225.0f;
        D_80113EDC = 0;
        D_80113ED4 = 1;
        goto block_26;
    }
block_26:
    var_v1_2 = input_rec0;
    var_a0_3 = 0;
    do {
        temp_a3 = M2C_FIELD(var_v1_2, u8 *, 1);
        if (temp_a3 != 5) {
            temp_v0_2 = &D_8015418C[var_a0_3];
            if (*temp_v0_2 == 0) {
                if ((state_word_a & 0x7C0000) && (M2C_FIELD(var_v1_2, s32 *, 4) & 2)) {
                    *temp_v0_2 = 1;
                }
            } else {
                spA0 = temp_v0_2;
                sp9C = var_v1_2;
                temp_v1 = &D_8014A10C[var_a0_3];
                sp8C = &D_8014A100[var_a0_3];
                temp_s0_2 = &D_8014A0F8[var_a0_3];
                spE8 = var_a0_3;
                temp_s1 = (var_a0_3 * 0xA) + D_80149DA8;
                temp_s3 = &D_80149DD4[var_a0_3];
                spA4 = temp_v1;
                spD8 = 0;
                var_t3 = 0;
                spEC = 0;
                if (*temp_v1 == 0) {
                    *temp_s0_2 = 0;
                    var_t0 = 2;
                    *sp8C = 0;
                    M2C_FIELD(temp_s1, s8 *, 1) = 0;
                    M2C_FIELD(temp_s1, s8 *, 0) = 0;
                    *temp_s3 = 0;
                    var_v0_2 = temp_s1 + 2;
                    do {
                        var_t0 += 4;
                        M2C_FIELD(var_v0_2, s8 *, 1) = 0;
                        M2C_FIELD(var_v0_2, s8 *, 2) = 0;
                        M2C_FIELD(var_v0_2, s8 *, 3) = 0;
                        var_v0_2 = (u8 *) var_v0_2 + 4;
                        M2C_FIELD(var_v0_2, s8 *, -4) = 0;
                    } while (var_t0 != 10);
                    temp_v0_3 = M2C_FIELD(sp9C, void ***, 0x48);
                    if (temp_v0_3 != NULL) {

                        memcpy((temp_a3 * 0xA) + D_8013C068, (u8 *) *temp_v0_3 + 0x21, 0xAU);
                    } else {

                        memcpy((temp_a3 * 0xA) + D_8013C068, (u8 *) D_80146150[temp_a3] + 0x21, 0xAU);
                    }
                    if (state_word_a & 0x7C03FFFE) {

                        drone_throttle_calc((s8)spE8);
                    }
                }


                func_800D816C((s8)spE8);
                selection_table=(s8 *)D_8013C068+M2C_FIELD(sp9C,u8 *,1)*10;
                if (((*temp_s3 == 1) && (temp_a0_2 = *temp_s0_2, (temp_s1[temp_a0_2] == 0)) && (temp_a0_2 != 0xB)) || (func_800D8154((s32) *temp_s0_2) == 0) || ((*(D_80144036 + (M2C_FIELD(sp9C, u8 *, 1) * 0x304)) == 0) && (10 == *temp_s0_2))) {
loop_46:
                    *temp_s0_2 += 1;
                    if (*temp_s0_2 >= 0xC) {
                        *temp_s0_2 = 0;
                    }
                    if ((*temp_s3 == 1) && (temp_s1[(s32) *temp_s0_2] == 0) && ((s32) *temp_s0_2 != 0xB)) {
                        goto loop_46;
                    }
                    if (func_800D8154((s32) *temp_s0_2) == 0) {
                        goto loop_46;
                    }
                    if ((*(D_80144036 + (M2C_FIELD(sp9C, u8 *, 1) * 0x304)) == 0) && (10 == *temp_s0_2)) {
                        goto loop_46;
                    }
                }
                if (M2C_FIELD(sp9C, s32 *, 4) & 0x400) {

                    entity_flags_apply(0x28U, 0U, 1U, 0U);
loop_56:
                    *temp_s0_2 -= 1;
                    if (*temp_s0_2 < 0) {
                        *temp_s0_2 = 0xB;
                    }
                    if ((*temp_s3 == 1) && (temp_s1[(s32) *temp_s0_2] == 0) && ((s32) *temp_s0_2 != 0xB)) {
                        goto loop_56;
                    }
                    if (func_800D8154((s32) *temp_s0_2) == 0) {
                        goto loop_56;
                    }
                    if ((*(D_80144036 + (M2C_FIELD(sp9C, u8 *, 1) * 0x304)) == 0) && (10 == *temp_s0_2)) {
                        goto loop_56;
                    }
                }
                if (M2C_FIELD(sp9C, s32 *, 4) & 0x800) {

                    entity_flags_apply(0x27U, 0U, 1U, 0U);
loop_66:
                    *temp_s0_2 += 1;
                    if (*temp_s0_2 >= 0xC) {
                        *temp_s0_2 = 0;
                    }
                    if ((*temp_s3 == 1) && (temp_s1[(s32) *temp_s0_2] == 0) && ((s32) *temp_s0_2 != 0xB)) {
                        goto loop_66;
                    }
                    if (func_800D8154((s32) *temp_s0_2) == 0) {
                        goto loop_66;
                    }
                    if ((*(D_80144036 + (M2C_FIELD(sp9C, u8 *, 1) * 0x304)) == 0) && (10 == *temp_s0_2)) {
                        goto loop_66;
                    }
                }
                for (var_t0_2=11;var_t0_2>=0;var_t0_2--) {
                    if(func_800D8154(var_t0_2)) {
                        var_t3++;
                        if(var_t3==D_8014A10A) {
                            spD8=var_t0_2;
                            break;
                        }
                    }
                }
                var_t3=0;
                for(var_t0_3=0;var_t0_3<*temp_s0_2;var_t0_3++) {
                    if(func_800D8154(var_t0_3)) var_t3++;
                }
                var_v0_3 = *sp8C;
                temp_v1_2 = (var_t3 - D_8014A10A) + 2;
                if (var_v0_3 < temp_v1_2) {
                    *sp8C = temp_v1_2;
                    var_v0_3 = *sp8C;
                }
                temp_v1_3 = var_t3 - 1;
                if (temp_v1_3 < var_v0_3) {
                    *sp8C = temp_v1_3;
                    var_v0_3 = *sp8C;
                }
                if (spD8 < var_v0_3) {
                    var_v0_3 = (s16) spD8;
                    *sp8C = (s16) spD8;
                }
                if (var_v0_3 < 0) {
                    *sp8C = 0;
                }
                if (*temp_s0_2 == 0xB) {
                    if (M2C_FIELD(sp9C, s32 *, 4) & 2) {

                        entity_flags_apply(0x2BU, 0U, 1U, 0U);
                        func_800C77C4(selection_table);
                    }
                } else {
                    temp_a0_3 = M2C_FIELD(sp9C, s32 *, 4);
                    if (temp_a0_3 & 0x1000) {

                        entity_flags_apply(0x29U, 0U, 1U, 0U);
                        if (*temp_s0_2 == 0xA) {
                            temp_v1_4 = *M2C_FIELD(sp9C, void ***, 0x48);
                            var_v0_4 = M2C_FIELD(temp_v1_4, s8 *, 0x2B) - 1;
                            if (var_v0_4 < 0) {
                                var_v0_4 = 2;
                            }
                            M2C_FIELD(temp_v1_4, s8 *, 0x2B) = var_v0_4;
                        } else {
                            do {
                                temp_v0_4 = selection_table + *temp_s0_2;
                                *temp_v0_4 -= 1;
                                temp_v0_5 = *temp_s0_2 + selection_table;
                                if (*temp_v0_5 < 0) {
                                    *temp_v0_5 = 25;
                                }
                            } while (func_800D8078((s8)spE8) == 0);
                        }
                    } else if (temp_a0_3 & 0x2000) {

                        entity_flags_apply(0x2CU, 0U, 1U, 0U);
                        if (*temp_s0_2 == 0xA) {
                            temp_v1_5 = *M2C_FIELD(sp9C, void ***, 0x48);
                            var_v0_5 = M2C_FIELD(temp_v1_5, s8 *, 0x2B) + 1;
                            if (var_v0_5 >= 3) {
                                var_v0_5 = 0;
                            }
                            M2C_FIELD(temp_v1_5, s8 *, 0x2B) = var_v0_5;
                        } else {
                            do {
                                temp_v0_6 = selection_table + *temp_s0_2;
                                *temp_v0_6 += 1;
                                temp_v0_7 = *temp_s0_2 + selection_table;
                                if (*temp_v0_7 >= 0x1A) {
                                    *temp_v0_7 = 0;
                                }
                            } while (func_800D8078((s8)spE8) == 0);
                        }
                    }
                }
                *temp_s3=0;
                for(var_t0=0;var_t0<10;var_t0++) temp_s1[var_t0]=0;
                var_t0_4 = 0;
                do {
                    temp_a3_2 = var_t0_4 + 1;
                    var_a0_4 = temp_a3_2;
                    if (temp_a3_2 < 0xA) {
                        var_a1 = selection_table + var_a0_4;
                        do {
                            temp_v0_8 = *(selection_table + var_t0_4);
                            temp_v1_6 = *var_a1;
                            if (((temp_v0_8 == temp_v1_6) || ((temp_v0_8 == 0x15) && (temp_v1_6 == 8)) || ((temp_v0_8 == 8) && (temp_v1_6 == 0x15)) || ((temp_v0_8 == 0x16) && (temp_v1_6 == 9)) || ((temp_v0_8 == 9) && (temp_v1_6 == 0x16))) && (19 != temp_v0_8) && (25 != temp_v0_8)) {
                                temp_s1[var_t0_4] = 1;
                                temp_s1[var_a0_4] = 1;
                                *temp_s3 = 1;
                            }
                            var_a0_4 += 1;
                            var_a1 += 1;
                        } while (var_a0_4 != 10);
                    }
                    var_t0_4 = temp_a3_2;
                } while (temp_a3_2 < 9);
                if (state_word_a & 0x7C03FFFE) {
                    temp_v0_9 = M2C_FIELD(sp9C, void ***, 0x48);
                    if (temp_v0_9 != NULL) {
                        var_a0_4 = M2C_FIELD(*temp_v0_9, s32 *, 8);
                        if ((var_a0_4 != 0) && (*(D_80144031 + (M2C_FIELD(M2C_FIELD(var_a0_4, void **, 0), u8 *, 0x10) * 0x304)) == 0)) {
                            spEC = 1;
                        }
                    }
                }
                if (spEC != 0) {
                    *spA0 = 0;
                    render_replay_ui((s8)spE8);
                    D_80113EDC = 2;
                } else {
                    var_v1_3 = 0;
                    var_t0_5 = 0;
                    temp_a0_4 = M2C_FIELD(sp9C, s32 *, 4);
                    var_v0_7 = temp_s1;
                    if (temp_a0_4 & 5) {
                        do {
                            var_t0_5 += 1;
                            if (*var_v0_7 == 1) {
                                var_v1_3 = 1;
                            }
                            var_v0_7 += 1;
                        } while (var_t0_5 != 10);
                        if (var_v1_3 == 1) {
                            entity_flags_apply(0x2AU, 0U, 1U, 0U);
                        } else {
                            temp_s0_3 = M2C_FIELD(sp9C, u8 *, 1);
                            resource_type_select(temp_a0_4);
                            if (temp_s0_3 != 5) {
                                temp_v0_10 = M2C_FIELD(sp9C, void ***, 0x48);
                                if (temp_v0_10 != NULL) {
                                    if (func_800A1910((temp_s0_3 * 0xA) + D_8013C068, (u8 *) *temp_v0_10 + 0x21, 0xA) != 0) {
                                        goto block_150;
                                    }
                                } else if (func_800A1910((temp_s0_3 * 0xA) + D_8013C068, (u8 *) D_80146150[temp_s0_3] + 0x21, 0xA) != 0) {
block_150:
                                    D_80139300[temp_s0_3] = 1;
                                }
                            }
                            drone_target_update(0);
                            *spA0 = 0;
                            render_replay_ui((s8)spE8);
                            D_80113EDC = 2;
                        }
                    }
                }
                *spA4 = *spA0;
                var_v1_2 = sp9C;
                var_a0_3 = spE8;
            }
        }
        var_a0_3 += 1;
        var_v1_2 += 0x4C;
    } while (var_a0_3 < 4);
    if (D_80151AD0 > 0) {
        var_v1_4 = D_8015418C;
        do {
            temp_t6_2 = *var_v1_4;
            var_v1_4 += 1;
            if (temp_t6_2 == 1) {
                D_80113EDC = 0;
            }
        } while ((u32) var_v1_4 < (u32) &D_8015418C[D_80151AD0]);
    }
    if ((D_80157244 != 0) || (D_80113EDC != 0)) {
        render_results_screen();
        if (!(state_word_a & 0x7C03FFFE)) {
            sound_handles_clear(0);
            func_800B4FB0(1);
        }
    }
}
