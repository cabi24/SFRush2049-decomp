/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete native DEF68 impact and collision sound reconstruction.
 * Related arcade ancestry: carsnd.c body-sound handling; N64 state and calls govern.
 * True logical input: one Model2056 pointer, private native carrier s1.
 * Both switch tables are derived from the current protected game image. */
#ifndef ROOT_IMPACT_SOUND_H
#define ROOT_IMPACT_SOUND_H
typedef signed char s8;typedef unsigned char u8;
typedef signed short s16;typedef unsigned short u16;
typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Impact24 {f32 force;f32 vector[3];f32 timestamp;s32 flag;} Impact24;
typedef struct Impact120 {Impact24 point[5];} Impact120;
typedef struct AudioModel2056 {
    u8 to_body_force[196];f32 body_force[4][3];
    u8 to_center_force[60];f32 center_force[3];
    u8 to_speed[692];f32 speed;
    u8 to_crash[588];s8 crash;u8 gap1601;
    s8 collision_kind,last_collision_kind;s16 collision_direction;
    u8 to_player[384];s16 player;u8 tail[64];
} AudioModel2056;
extern Impact120 D_80140808[];
extern volatile f32 D_8002EB90;
extern f32 D_80140BE0[],D_80140B10[],D_80142518[];
extern s32 D_8011735C,D_80140AE0[],state_word_a;
extern s16 D_8011F020[],D_8011F040[],D_80140A08[];
extern f32 D_80124324,D_80124328,D_8012432C,D_80124330,D_80124350,D_8012436C;
void func_800DED78(s16,s32,f32*,f32);
void best_times_display(s16);
u32 high_scores_display(u32,u32,u32,u8,f32,f32,f32);
void scheduler_recv(s32);
f32 fabsf(f32);
#pragma intrinsic(fabsf)
#endif


void mode_select_handler(AudioModel2056 *m) {
    Impact120 *temp_a1;
    Impact120 *temp_a2;
    Impact24 *var_v0;
    Impact24 *var_v0_2;
    f32 *var_v1_2;
    f32 *temp_a0;
    f32 *temp_s2;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f16;
    f32 temp_f22;
    f32 temp_f22_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 var_f12;
    f32 var_f12_2;
    f32 var_f18;
    f32 var_f2;
    f32 var_f2_2;
    s16 *temp_s2_2;
    s16 temp_s6;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 temp_v1;
    s16 var_a3;
    s16 var_s0;
    s16 var_s4;
    s16 var_v0_4;
    s32 *temp_s0;
    s32 *temp_s0_2;
    s32 *temp_s0_3;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 var_a0;
    s32 var_v0_3;
    s32 var_v1;
    s8 temp_v0;
    s8 temp_v0_2;

    temp_s6 = m->player;
    var_a3 = 0;
    if (m->crash != 0) {
        best_times_display(temp_s6);
        return;
    }
    temp_f22 = D_80124324;
    do {
        func_800DED78(var_a3, (s32) m->player, m->body_force[var_a3], temp_f22);
        var_a3 += 1;
    } while (var_a3 < 4);
    func_800DED78(4, (s32) m->player, m->center_force, temp_f22);
    temp_s2 = &D_80140BE0[temp_s6];
    temp_f0 = *temp_s2;
    var_s0 = 0;
    var_f12 = 0.0f;
    if ((temp_f0 != 0.0f) && (((D_8002EB90 - temp_f0) > 1.0f) || (D_8002EB90 < temp_f0))) {
        *temp_s2 = 0.0f;
    }
    temp_a2 = &D_80140808[temp_s6];
    var_v0 = &temp_a2->point[0];
    var_v1 = 0;
    do {
        var_v1 += 0x18;
        if (var_v0->flag != 0) {
            temp_f0_2 = var_v0->force;
            if (var_f12 < temp_f0_2) {
                var_f12 = temp_f0_2;
            }
        }
        var_v0++;
    } while (var_v1 < 0x60);
    var_f2 = 150.0f;
    temp_f0_3 = var_f12 * D_80124328;
    if (temp_f0_3 < 150.0f) {

    } else if (temp_f0_3 > 235.0f) {
        var_f2 = 235.0f;
    } else {
        var_f2 = temp_f0_3;
    }
    temp_f22_2 = D_8012432C;
    var_a0 = 0;
    var_v0_2 = &temp_a2->point[0];
    var_f18 = var_f2 * 0.00390625f * temp_f22_2;
    do {
        if (var_v0_2->flag != 0) {
            if ((var_f12 * 0.5f) < var_v0_2->force) {
                var_s0 |= 1 << var_a0;
            }
            var_v0_2->force = 0.0f;
            var_v0_2->flag = 0;
        }
        var_a0 += 1;
        var_v0_2++;
    } while (var_a0 < 4);
    if (var_s0 != 0) {
        if (*temp_s2 == 0.0f) {
            temp_t7 = (s32) (((u32) D_8011735C * 0x41C64E6DU) + 0x3039U);
            D_8011735C = temp_t7;
            high_scores_display((u32) (s32) ((((f32) ((temp_t7 >> 0x10) & 0x7FFF) * 13.0f) / 32768.0f) + 48.0f), (u32) m->player, 1U, 1U, var_f18, (f32) D_8011F020[var_s0], (f32) D_8011F040[var_s0]);
            *temp_s2 = D_8002EB90;
        } else {
            temp_t8 = (s32) (((u32) D_8011735C * 0x41C64E6DU) + 0x3039U);
            D_8011735C = temp_t8;
            high_scores_display((u32) (s32) ((((f32) ((temp_t8 >> 0x10) & 0x7FFF) * 13.0f) / 32768.0f) + 48.0f), (u32) m->player, 1U, 1U, var_f18, (f32) D_8011F020[var_s0], (f32) D_8011F040[var_s0]);
            *temp_s2 = 0.0f;
        }
    } else {
        temp_a1 = &D_80140808[temp_s6];
        var_s0 = 0;
        if (temp_a1->point[4].flag != 0) {
            var_f12_2 = 0.0f;
            var_v0_3 = 0;
            var_v1_2 = temp_a2->point[4].vector;
            do {
                var_v0_3 += 4;
                temp_f2 = fabsf(*var_v1_2);
                if (var_f12_2 < temp_f2) {
                    var_f12_2 = temp_f2;
                }
                var_v1_2++;
            } while (var_v0_3 != 0xC);
            var_f2_2 = 180.0f;
            temp_f0_4 = var_f12_2 * D_80124330;
            if (temp_f0_4 < 180.0f) {

            } else if (temp_f0_4 > 235.0f) {
                var_f2_2 = 235.0f;
            } else {
                var_f2_2 = temp_f0_4;
            }
            var_f18 = var_f2_2 * 0.00390625f;
            temp_f2_2 = temp_a1->point[4].vector[0];
            temp_f16 = var_f12_2 * 0.5f;
            if (temp_f16 < fabsf(temp_f2_2)) {
                var_s0 = 2;
                if (temp_f2_2 < 0.0f) {
                    var_s0 = 1;
                }
            }
            temp_f2_3 = temp_a1->point[4].vector[1];
            if (temp_f16 < fabsf(temp_f2_3)) {
                if (temp_f2_3 < 0.0f) {
                    var_s0 |= 4;
                } else {
                    var_s0 |= 8;
                }
            }
            if (temp_f16 < fabsf(temp_a1->point[4].vector[2])) {
                var_s0 |= 1;
            }
            temp_a1->point[4].force = 0.0f;
            temp_a1->point[4].flag = 0;
        }
        if (var_s0 != 0) {
            temp_t9 = (s32) (((u32) D_8011735C * 0x41C64E6DU) + 0x3039U);
            D_8011735C = temp_t9;
            high_scores_display((u32) (s32) ((((f32) ((temp_t9 >> 0x10) & 0x7FFF) * 13.0f) / 32768.0f) + 48.0f), (u32) m->player, 1U, 1U, var_f18, (f32) D_8011F020[var_s0], (f32) D_8011F040[var_s0]);
        }
    }
    if ((var_s0 == 0) && (m->speed > 20.0f)) {
        var_v0_4 = 0;
        if ((m->body_force[0][0] != 0.0f) || (m->body_force[2][0] != 0.0f)) {
            var_v0_4 = 1;
        }
        if ((m->body_force[1][0] != 0.0f) || (m->body_force[3][0] != 0.0f)) {
            var_v0_4 |= 2;
        }
        temp_s2_2 = &D_80140A08[temp_s6];
        temp_v1 = *temp_s2_2;
        var_s4 = 0;
        switch (temp_v1) {                          /* switch 1 */
        case 0:                                     /* switch 1 */
            D_80140B10[temp_s6] = D_8002EB90;
            *temp_s2_2 = var_v0_4;
            var_s4 = var_v0_4;
            break;
        case 1:                                     /* switch 1 */
        case 2:                                     /* switch 1 */
        case 3:                                     /* switch 1 */
            if (var_v0_4 == 0) {
                temp_s0 = &D_80140AE0[temp_s6];
                scheduler_recv(*temp_s0);
                *temp_s0 = -1;
                *temp_s2_2 = 0;
block_68:
                D_80140B10[temp_s6] = 0.0f;
            } else {
                temp_f0_5 = D_80140B10[temp_s6];
                if ((D_80124350 < (D_8002EB90 - temp_f0_5)) || (D_8002EB90 < temp_f0_5)) {
                    *temp_s2_2 = temp_v1 + 3;
                    var_s4 = *temp_s2_2;
                }
            }
            break;
        case 4:                                     /* switch 1 */
        case 5:                                     /* switch 1 */
        case 6:                                     /* switch 1 */
            if (var_v0_4 == 0) {
                temp_s0_2 = &D_80140AE0[temp_s6];
                scheduler_recv(*temp_s0_2);
                *temp_s0_2 = -1;
                *temp_s2_2 = 0;
                goto block_68;
            }
            break;
        }
        switch (var_s4) {                           /* switch 2 */
        case 1:                                     /* switch 2 */
            temp_t7_2 = (s32) (((u32) D_8011735C * 0x41C64E6DU) + 0x3039U);
            D_8011735C = temp_t7_2;
            high_scores_display((u32) (s32) ((((f32) ((temp_t7_2 >> 0x10) & 0x7FFF) * 13.0f) / 32768.0f) + 48.0f), (u32) m->player, 1U, 1U, temp_f22_2, 1.0f, 0.0f);
            break;
        case 2:                                     /* switch 2 */
            temp_t6 = (s32) (((u32) D_8011735C * 0x41C64E6DU) + 0x3039U);
            D_8011735C = temp_t6;
            high_scores_display((u32) (s32) ((((f32) ((temp_t6 >> 0x10) & 0x7FFF) * 13.0f) / 32768.0f) + 48.0f), (u32) m->player, 1U, 1U, temp_f22_2, -1.0f, 0.0f);
            break;
        case 3:                                     /* switch 2 */
            temp_t9_2 = (s32) (((u32) D_8011735C * 0x41C64E6DU) + 0x3039U);
            D_8011735C = temp_t9_2;
            high_scores_display((u32) (s32) ((((f32) ((temp_t9_2 >> 0x10) & 0x7FFF) * 13.0f) / 32768.0f) + 48.0f), (u32) m->player, 1U, 1U, temp_f22_2, 0.0f, 0.0f);
            break;
        case 4:                                     /* switch 2 */
            D_80140AE0[temp_s6] = high_scores_display(0x13U, (u32) m->player, 1U, 2U, temp_f22_2, 1.0f, 0.0f);
            break;
        case 5:                                     /* switch 2 */
            D_80140AE0[temp_s6] = high_scores_display(0x13U, (u32) m->player, 1U, 2U, temp_f22_2, -1.0f, 0.0f);
            break;
        case 6:                                     /* switch 2 */
            D_80140AE0[temp_s6] = high_scores_display(0x13U, (u32) m->player, 1U, 2U, temp_f22_2, 0.0f, 0.0f);
            break;
        }
    } else {
        temp_s0_3 = &D_80140AE0[temp_s6];
        scheduler_recv(*temp_s0_3);
        *temp_s0_3 = -1;
        D_80140A08[temp_s6] = 0;
        D_80140B10[temp_s6] = 0.0f;
    }
    temp_a0 = &D_80142518[temp_s6];
    temp_f0_6 = *temp_a0;
    if ((temp_f0_6 != 0.0f) && ((D_8012436C < (D_8002EB90 - temp_f0_6)) || (D_8002EB90 < temp_f0_6))) {
        *temp_a0 = 0.0f;
    }
    temp_v0 = m->collision_kind;
    if (temp_v0 < m->last_collision_kind) {
        m->last_collision_kind = temp_v0;
    }
    if ((temp_v0 != m->last_collision_kind) && (m->last_collision_kind = temp_v0, (*temp_a0 == 0.0f)) && (*temp_a0 = D_8002EB90, !(state_word_a & 0x200000))) {
        temp_v0_2 = m->last_collision_kind;
        switch (temp_v0_2) {                        /* switch 3; irregular */
        case 1:                                     /* switch 3 */
            temp_v0_3 = m->collision_direction;
            high_scores_display(9U, (u32) m->player, 1U, 2U, 1.0f, (f32) D_8011F020[temp_v0_3], (f32) D_8011F040[temp_v0_3]);
            return;
        case 2:                                     /* switch 3 */
            temp_v0_4 = m->collision_direction;
            high_scores_display(5U, (u32) m->player, 1U, 2U, 1.0f, (f32) D_8011F020[temp_v0_4], (f32) D_8011F040[temp_v0_4]);
            return;
        case 3:                                     /* switch 3 */
            temp_v0_5 = m->collision_direction;
            high_scores_display(3U, (u32) m->player, 1U, 2U, 1.0f, (f32) D_8011F020[temp_v0_5], (f32) D_8011F040[temp_v0_5]);
            break;
        }
    }
}
