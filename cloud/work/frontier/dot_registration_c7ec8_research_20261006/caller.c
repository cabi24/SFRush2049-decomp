typedef unsigned char u8; typedef signed char s8; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define NULL ((void*)0)
#define M2C_FIELD(expr,type,offset) (*(type)((u8 *)(expr)+(offset)))
extern s8 D_8011742C,D_80117430,D_80117434;
extern u8 D_801211BC,D_801211C8,D_80144030,D_80144D68;
extern void **D_80152028;
extern void track_render_process(s32,s32);
extern s32 func_800950AC(void*,void*,u32);
extern void **func_800C7EC8(void**),**func_800C7200(void);
extern void drone_set_catchup(void*,s32,s32),func_80002790(void*,s32,u32),func_80007c68(void*,void*,u32),AdjustSteer(void*),func_80091FBC(void*,void*,void*);
extern s32 draw_ui_element(void*,s32,s32);
void func_800C813C(void) {
    s32 sp9C;
    void **sp94;
    s8 *sp54;
    void *sp50;
    s32 temp_t9;
    s8 var_v1;
    void **temp_s6;
    void **temp_t9_2;
    void **temp_v0_3;
    void **var_a2;
    void **var_s0;
    void **var_s2;
    void *temp_s1;
    void *temp_s3;
    void *temp_s8;
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_4;
    void *var_s8;

    track_render_process(0, 0);
    if (D_8011742C >= 2) {
        D_8011742C -= 1;
        return;
    }
    var_v1 = D_80117430;
    sp9C = 0;
    sp54 = &D_80117434;
    do {
        if ((D_8011742C != 0) || (*sp54 != 0) || ((temp_s8 = (sp9C * 0x304) + (u8 *) &D_80144030, (M2C_FIELD(temp_s8, s8 *, 0) != 0)) && (M2C_FIELD(temp_s8, s8 *, 1) != 0) && (M2C_FIELD(temp_s8, s8 *, 6) == 0))) {
            *sp54 = 1;
            if (var_v1 != sp9C) {
                var_s2 = *(void ***)((u8 *) &D_80144D68 + (sp9C * 0x10));
                if (var_s2 != NULL) {
                    var_s8 = (sp9C * 0x304) + (u8 *) &D_80144030;
                    D_80117430 = (s8)sp9C;
loop_11:
                    if (M2C_FIELD(var_s8, s8 *, 1) != 0) {
                        temp_v0 = *var_s2;
                        temp_s6 = M2C_FIELD(temp_v0, void ***, 0);
                        if (func_800950AC(&D_801211BC, (u8 *) temp_v0 + 0x12, 8) == 0) {
                            if (func_800950AC(&D_801211C8, (u8 *) *var_s2 + 0x35, 1) == 0) {
                                sp94 = temp_s6;
                                sp50 = var_s8;
                                func_800C7EC8(var_s2);
                            } else {
                                var_s0 = D_80152028;
                                if (var_s0 != NULL) {
loop_16:
                                    temp_v0_2 = *var_s0;
                                    if (var_s2 == (void *) (M2C_FIELD(temp_v0_2, s32 *, 4))) {

                                    } else {
                                        var_s0 = M2C_FIELD(temp_v0_2, void ***, 0);
                                        if (var_s0 == NULL) {
                                            goto block_19;
                                        }
                                        goto loop_16;
                                    }
                                } else {
block_19:
                                    drone_set_catchup(var_s2, 0, 0x58);
                                    if (M2C_FIELD(*var_s2, void ***, 0x48) == NULL) {

                                    } else {
                                        temp_v0_3 = func_800C7200();
                                        temp_s1 = *temp_v0_3;
                                        func_80002790(temp_s1, 0, 0x2C);
                                        M2C_FIELD(temp_s1, void ***, 4) = var_s2;
                                        M2C_FIELD(*var_s2, s32 *, 8) = 0x80094C28;
                                        M2C_FIELD(*M2C_FIELD(temp_s1, void ***, 4), s32 *, 0xC) = 0x800949D4;
                                        M2C_FIELD(temp_s1, s32 *, 0x24) = 0;
                                        temp_t9_2 = M2C_FIELD(*var_s2, void ***, 0x48);
                                        M2C_FIELD(temp_s1, void ***, 0x28) = temp_t9_2;
                                        temp_s3 = *temp_t9_2;
                                        M2C_FIELD(temp_s3, s32 *, 0x4C) = 0;
                                        if ((draw_ui_element(temp_v0_3, 0, 0) != 0) && (draw_ui_element(temp_v0_3, 1, 0) == 0)) {
                                            *temp_v0_3 = NULL;
                                        } else {
                                            M2C_FIELD(temp_s1, s8 *, 8) = (s8) M2C_FIELD(temp_s3, s8 *, 6);
                                            M2C_FIELD(temp_s1, u8 *, 9) = (u8) M2C_FIELD(temp_s3, u8 *, 7);
                                            func_80007c68((u8 *) temp_s1 + 0xA, (u8 *) temp_s3 + 0x16, 0xD);
                                            M2C_FIELD(temp_s1, s32 *, 0x18) = (s32) M2C_FIELD(temp_s3, s32 *, 0x28);
                                            M2C_FIELD(temp_s1, s32 *, 0x1C) = (s32) M2C_FIELD(temp_s3, s32 *, 0x2C);
                                            M2C_FIELD(temp_s1, f32 *, 0x20) = (f32) M2C_FIELD(temp_s3, f32 *, 0x38);
                                            AdjustSteer(var_s2);
                                            M2C_FIELD(temp_s1, void ***, 0x28) = NULL;
                                            var_a2 = D_80152028;
                                            if (var_a2 != NULL) {
loop_26:
                                                temp_v0_4 = *var_a2;
                                                if (!(M2C_FIELD(temp_s1, f32 *, 0x20) < M2C_FIELD(temp_v0_4, f32 *, 0x20))) {
                                                    var_a2 = M2C_FIELD(temp_v0_4, void ***, 0);
                                                    if (var_a2 != NULL) {
                                                        goto loop_26;
                                                    }
                                                }
                                            }
                                            func_80091FBC(&D_80152028, temp_v0_3, var_a2);
                                        }
                                    }
                                }
                            }
                        }
                        var_s2 = temp_s6;
                        if (temp_s6 != NULL) {
                            goto loop_11;
                        }
                    }
                }
                var_v1 = -1;
                *sp54 = 0;
            }
        }
        temp_t9 = sp9C + 1;
        sp54 += 1;
        sp9C = temp_t9;
    } while (temp_t9 != 4);
    D_80117430 = var_v1;
    D_8011742C = 0;
}
