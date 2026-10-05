/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Sprite {
    u32 word0, word4, word8;
    u16 half12;
    s16 x, y;
    u16 half18;
    s16 width, height;
    u8 alpha, byte25;
    s8 hidden;
    u8 byte27;
    s16 left, right, top, bottom;
    u32 word36;
    s32 state;
    u32 selector;
    u32 word48;
    u16 half52;
} Sprite;

typedef struct MarkerRecord {
    u8 prefix[48];
    f32 position[3];
    u8 alpha;
    u8 remaining[3];
} MarkerRecord;

typedef struct PlayerInput {
    u8 prefix;
    u8 state;
    u8 remaining[74];
} PlayerInput;

extern s16 D_8014A100[];
extern s16 D_8014A10A;
extern s16 D_80151AD0;
extern s8 D_80149DA8[][10];
extern s8 D_8015418C[];
extern PlayerInput input_rec0[];
extern s32 D_80154618[];
extern MarkerRecord D_80111998[][17];
extern u8 D_80150B70[][152];
extern f32 D_80112A9C;
extern f32 D_80112AB0;
extern u8 D_80113EE0[];
extern u8 D_80120234[];
extern s32 D_801140E8;
extern s32 D_8011407C[];
extern s8 D_8013C068[][10];
extern void Input_ApplyPadConfig(Sprite *);
extern void func_800EF5B0(Sprite *, void *, s32);
extern void brake_light_update(s32, f32 *, void *, f32 *, s16 *);
extern void stat_race_update(Sprite *, s32, s32, s32);


s32 game_results_input(Sprite *arg0) {
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s16 screen[2];
    f32 sp54;
    PlayerInput *sp2C;
    s32 sp28;
    u8 *sp24;
    PlayerInput *temp_t8_2;
    s16 temp_a2;
    s16 temp_a2_2;
    s16 temp_t0;
    s16 var_a0;
    s16 var_a1;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_v0_2;
    s32 temp_v1_3;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1;
    s8 temp_t8_3;
    s8 temp_t8_4;
    s8 temp_t9;
    s8 temp_v0_4;
    s8 var_v0;
    s8 var_v1_2;
    s8 var_v1_3;
    u32 temp_t6_2;
    u32 temp_v0;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 temp_v0_3;

    temp_v0 = arg0->selector;
    temp_t6 = (temp_v0 >> 4) & 0xF;
    temp_t8 = (temp_v0 >> 0xC) & 0xF;
    var_t4 = temp_t8;
    temp_t0 = D_8014A100[temp_t8];
    temp_t7 = (temp_v0 >> 8) & 0xF;
    var_t2 = temp_v0 & 0xF;
    sp64 = ((temp_v0 >> 0x10) & 0xFF) - 1;
    var_t3 = (temp_t0 + var_t2) - 1;
    sp54 = D_80112A9C;
    if (D_80151AD0 >= 2) {
        sp54 = D_80112A9C * 0.5f;
    }
    var_a0 = 0;
    var_a1 = 0;
    if (var_t2 > 0) {
        do {
            var_v1 = 1;
            if ((temp_t0 + var_a1) == 8) {
                var_v1 = 0;
            }
            if (var_v1 == 0) {
                var_t3 += 1;
            } else {
                var_a0 += 1;
            }
            var_a1 += 1;
        } while (var_a0 < var_t2);
    }
    temp_t8_2 = &input_rec0[var_t4];
    sp2C = temp_t8_2;
    var_v0 = temp_t8_2->state == 5;
    if ((var_v0 == 0) && ((temp_t6 == 0) || (temp_t8_3 = D_80149DA8[var_t4][var_t3] == 0, var_v0 = temp_t8_3, (temp_t8_3 == 0)))) {
        var_v0 = (var_t3 < 0xA) ^ 1;
        if (var_v0 == 0) {
            temp_t9 = D_8015418C[var_t4] == 0;
            var_v0 = temp_t9;
            if (temp_t9 == 0) {
                var_v0 = D_8014A10A < var_t2;
            }
        }
    }
    var_v1_2 = arg0->hidden;
    if (var_v0 != var_v1_2) {
        arg0->hidden = var_v0;
        sp68 = var_t4;
        sp60 = var_t3;
        Input_ApplyPadConfig(arg0);
        var_v1_2 = arg0->hidden;
    }
    if (var_v1_2 != 0) {

    } else {
        sp28 = temp_t6 != 0;
        if (temp_t7 != 0) {
            if (D_80151AD0 >= 2) {
                sp60 = var_t3;
                sp68 = var_t4;
                func_800EF5B0(arg0, D_80120234, 0);
            }
            if (D_80154618[var_t4] != 0) {
                temp_v1 = (u8 *)&D_80111998[var_t4][var_t3];
                sp24 = temp_v1;
                sp60 = var_t3;
                sp68 = var_t4;
                brake_light_update(var_t4, (f32 *) (temp_v1 + 0x30), D_80150B70[var_t4], 0, screen);
                var_t3 = sp60;
                var_t4 = sp68;
                arg0->x = (s16) (s32) ((f32) screen[0] + ((55.0f * sp54 * 300.0f) / D_80112AB0));
                arg0->y = screen[1] - ((s16) arg0->height / 2);
                arg0->alpha = sp24[60];
            } else {
                temp_v0_2 = var_t4 * 0x18;
                arg0->x = *(s32 *)(D_80113EE0 + D_80151AD0 * 0x60 + temp_v0_2 - 0x50) + 0x14;
                arg0->y = *(s32 *)(D_80113EE0 + D_80151AD0 * 0x60 + temp_v0_2 - 0x5C) + (var_t2 * 0x10) + 8;
            }
            if (sp28 != 0) {
                var_v0_2 = 0xA;
                if (D_80151AD0 == 1) {
                    var_v0_2 = 0x14;
                }
                arg0->x += var_v0_2;
            }
            sp60 = var_t3;
            sp68 = var_t4;
            Input_ApplyPadConfig(arg0);
            temp_t6_2 = arg0->selector & 0xFFF0FF;
            arg0->selector = temp_t6_2;
            if (sp28 != 0) {
                temp_a2 = arg0->height;
                arg0->selector = (temp_t6_2 & 0xFFFF) | 0x1C0000;
                sp68 = var_t4;
                sp60 = var_t3;
                stat_race_update(arg0, D_801140E8, (s32) temp_a2, (s32) temp_a2);
            }
        }
        if (D_80154618[var_t4] != 0) {
            temp_v1_2 = (u8 *)&D_80111998[var_t4][var_t3];
            sp24 = temp_v1_2;
            sp60 = var_t3;
            brake_light_update(var_t4, (f32 *) (temp_v1_2 + 0x30), D_80150B70[var_t4], 0, screen);
            arg0->x = (s16) (s32) ((f32) screen[0] + ((75.0f * sp54 * 300.0f) / D_80112AB0));
            arg0->y = screen[1] - ((s16) arg0->height / 2);
            arg0->half18 = 0x7F00;
            if (sp28 != 0) {
                var_v0_3 = 0xA;
                if (D_80151AD0 == 1) {
                    var_v0_3 = 0x14;
                }
                arg0->x += var_v0_3;
            }
            temp_v0_3 = sp24[60];
            if ((s32) temp_v0_3 >= 0xFF) {
                arg0->alpha = 0xFE;
            } else {
                arg0->alpha = temp_v0_3;
            }
            var_v1_3 = arg0->hidden;
            temp_t8_4 = arg0->alpha == 0;
            if (temp_t8_4 != var_v1_3) {
                arg0->hidden = temp_t8_4;
                Input_ApplyPadConfig(arg0);
                var_v1_3 = arg0->hidden;
            }
            if (var_v1_3 != 0) {

            } else {
                Input_ApplyPadConfig(arg0);
                goto block_46;
            }
        } else {
            arg0->alpha = 0xFE;
            sp60 = var_t3;
            Input_ApplyPadConfig(arg0);
block_46:
            if (sp28 != 0) {

            } else {
                temp_v0_4 = D_8013C068[sp2C->state][sp60];
                temp_v1_3 = temp_v0_4 + 1;
                if (sp64 != temp_v0_4) {
                    sp64 = (s32) temp_v0_4;
                    arg0->selector = (arg0->selector & 0xFFFF) | ((temp_v1_3 << 0x10) & 0xFF0000);
                    if ((sp64 == 0x19) && ((sp60 == 0) || (sp60 == 1))) {
                        sp64 = temp_v1_3;
                    }
                    temp_a2_2 = arg0->height;
                    stat_race_update(arg0, D_8011407C[sp64], (s32) temp_a2_2, (s32) temp_a2_2);
                }
            }
        }
    }
    return 1;
}
