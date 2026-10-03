/* Native complete captured-value source; all locals actually consumed.
 * Typed repairs to registered automatic seed, signed dot-list byte and true32-byte renderstride. */
#include "types.h"
s32 func_80109A60(Sprite *arg0) {
    s16 sp1A;
    Car952 *var_a0;
    SpriteRender32 *temp_v0;
    s16 temp_t3;
    s16 temp_t6;
    s16 temp_t6_2;
    s16 temp_t8;
    s16 temp_t9;
    s16 var_a1;
    s16 var_v0_2;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_a3;
    s32 var_t1;
    s32 var_v1_3;
    s8 temp_v0_2;
    s8 var_v0;
    s8 var_v1;
    s8 var_v1_2;
    Model2056 *temp_v1;

    var_v0 = ((s16) D_80151AD0 < 5) ^ 1;
    sp1A = (s16) D_80142DB4[arg0->selector & 0xF];
    if (var_v0 == 0) {
        var_v0 = (sp1A + 1) == 0;
        if (var_v0 == 0) {
            var_v0 = D_80156BDC == 0;
        }
    }
    var_v1_2 = arg0->hidden;
    if (var_v0 != var_v1_2) {
        arg0->hidden = var_v0;
        Input_ApplyPadConfig(arg0);
        var_v1_2 = arg0->hidden;
    }
    if (var_v1_2 != 0) {
        goto block_33;
    }
    temp_a1 = D_801407B4.x - D_801407D4.x;
    temp_a3 = D_801407B4.z - D_801407D4.z;
    temp_t3 = arg0->height;
    if (temp_a3 < temp_a1) {
        temp_a0 = D_801161C4 - 8;
        var_v1_3 = temp_a0;
        var_t1 = (s32) (temp_a3 * temp_a0) / temp_a1;
    } else {
        temp_a0_2 = D_801161C4 - 8;
        var_t1 = temp_a0_2;
        var_v1_3 = (s32) (temp_a1 * temp_a0_2) / temp_a3;
    }
    arg0->x = (s16) ((D_801160A8[D_80151AD0 - 1].x + (((s32) ((D_801161C4 - var_v1_3) - 8) / 2) + 4)) - ((s32) (temp_t3 + D_801161C4) / 2));
    arg0->y = (s16) ((D_801160A8[D_80151AD0 - 1].y + (((s32) ((D_801161C4 - var_t1) - 8) / 2) + 4)) - ((s32) (temp_t3 + D_801161C4) / 2));
    if (D_80140A04 != 0) {
        var_a0 = &player_array[sp1A];
        arg0->x = (s16) (s32) ((f32) arg0->x + (((var_a0->position[0] - (f32) D_801407D4.x) * (f32) var_v1_3) / (f32) temp_a1));
    } else {
        var_a0 = &player_array[sp1A];
        arg0->x = (s16) (s32) ((f32) arg0->x + ((f32) (var_v1_3 - 1) - (((var_a0->position[0] - (f32) D_801407D4.x) * (f32) var_v1_3) / (f32) temp_a1)));
    }
    temp_v0 = &D_80140BF0[arg0->render_slot];
    arg0->y = (s16) (s32) ((f32) arg0->y + (((var_a0->position[2] - (f32) D_801407D4.z) * (f32) var_t1) / (f32) temp_a3));
    temp_v0->flags |= 1;
    temp_t6 = var_a0->dead != 0;
    var_v0_2 = temp_t6;
    if (temp_t6 == 0) {
        temp_v1 = &D_8014A250[sp1A];
        temp_t9 = temp_v1->crash != 0;
        var_v0_2 = temp_t9;
        if (temp_t9 == 0) {
            temp_t6_2 = temp_v1->collidable == 0;
            var_v0_2 = temp_t6_2;
            if (temp_t6_2 == 0) {
                temp_t8 = temp_v1->hide != 0;
                var_v0_2 = temp_t8;
                if (temp_t8 == 0) {
                    var_v0_2 = (temp_v1->hit_target + 1) != 0;
                }
            }
        }
    }
    var_a1 = var_v0_2;
    if (var_a0->active > 0) {
        var_a1 = 0;
    }
    if (sp1A < D_801543CA) {
        if ((D_8002E8E8.tick & 8) && (var_a1 != 0)) {
            arg0->alpha = 0x40;
        } else {
            arg0->alpha = 0xFF;
        }
        temp_v0_2 = D_8014A250[sp1A].mode;
        if (temp_v0_2 == 2) {
            stat_race_update(arg0, sp1A, temp_t3, temp_t3);
        } else if (temp_v0_2 == 1) {
            stat_race_update(arg0, 7, temp_t3, temp_t3);
        }
        Input_ApplyPadConfig(arg0);
block_33:
        return 1;
    }
    var_v1 = arg0->hidden;
    if (var_v1 != 1) {
        arg0->hidden = 1;
        Input_ApplyPadConfig(arg0);
        var_v1 = arg0->hidden;
    }
    return var_v1;
}

