/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * mode_select_handler (0x800DEF68, 2976 B): EQUAL (744 words) in the whole-program unit, with its real
 * caller func_800E05F0 (draft) and its real callees func_800DED78 / best_times_display (both EQUAL here).
 * Provisional only because the caller func_800E05F0 does not match yet (s1 = model is an IPA register
 * parameter set by that caller).  w12c, 2026-10-06.
 *
 * Arcade ancestor (proven by structure and constants): do_bump_sounds() + check_forces_on_car() in
 * reference/repos/rushtherock/game/carsnd.c.  N64 differences: per-player arrays (bump_time D_80140BE0,
 * scrape_state D_80140A08, scrape_time D_80140B10, scrape handle D_80140AE0, thump timer D_80142518),
 * one high_scores_display (positional sound) call per event, volume as f32 (range()/256 [*0.65]),
 * random sample 48+Random(13) via the locked func_8008B2E4.
 *
 * Shaping that the words prove:
 *  - check_forces_on_car is a static defined AFTER the caller (as in carsnd.c).  Inlined, its DED78 calls
 *    carry its later line numbers, which decides an as1 tie (mtc1 $f20 vs the bump_time load).  Defined
 *    before the caller, umerge inlines DED78 into it instead.  Its deleted body is the stub func_800DEF60.
 *  - kill_scrape_sound() is written out at each of its three sites (a macro or an __inline function
 *    schedules the D_80140B10 store differently).
 *  - scrape pan arguments are int literals 1 / -1 (own constant webs; 0.0f stays shared with $f20).
 *  - `volume = volume / 256.0f; volume = volume * 0.65f;` as two statements.
 *  - own .rodata (natural literals, verify at splice): 5000.0f 0.01f 0.65f 0.001f, jump table,
 *    0.1f, jump table, 0.16666667f at 0x80124324..0x8012436C.
 */
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
void func_800DED78(s16,s32,f32*,f32);
void best_times_display(s16);
u32 high_scores_display(u32,u32,u32,u8,f32,f32,f32);
void scheduler_recv(s32);
f32 func_8008B2E4(f32 max);
f32 fabsf(f32);
#pragma intrinsic(fabsf)
#endif


void func_800DEF60(AudioModel2056 *m);
#define range(a,b,c) ((a<b)?b:((a>c)?c:a))

void mode_select_handler(AudioModel2056 *m) {
    s32 i;
    s16 bump_index, scrape_side, scrape_snd;
    f32 volume, high_force;
    s32 p;

    p = m->player;
    if (m->crash != 0) {
        best_times_display(p);
        return;
    }
    func_800DEF60(m);

    bump_index = 0;
    high_force = 0.0f;
    if (D_80140BE0[p] != 0.0f) {
        if (D_8002EB90 - D_80140BE0[p] > 1.0f || D_8002EB90 < D_80140BE0[p]) {
            D_80140BE0[p] = 0.0f;
        }
    }
    for (i = 0; i < 4; i++) {
        if (D_80140808[p].point[i].flag && D_80140808[p].point[i].force > high_force) {
            high_force = D_80140808[p].point[i].force;
        }
    }
    volume = range(high_force * 0.01f, 150.0f, 235.0f);
    volume = volume / 256.0f;
    volume = volume * 0.65f;
    for (i = 0; i < 4; i++) {
        if (D_80140808[p].point[i].flag) {
            if (D_80140808[p].point[i].force > high_force * 0.5f) {
                bump_index |= 1 << i;
            }
            D_80140808[p].point[i].force = 0.0f;
            D_80140808[p].point[i].flag = 0;
        }
    }
    if (bump_index) {
        if (D_80140BE0[p] == 0.0f) {
            high_scores_display((s32)(func_8008B2E4(13.0f) + 48.0f), m->player, 1, 1, volume,
                                D_8011F020[bump_index], D_8011F040[bump_index]);
            D_80140BE0[p] = D_8002EB90;
        } else {
            high_scores_display((s32)(func_8008B2E4(13.0f) + 48.0f), m->player, 1, 1, volume,
                                D_8011F020[bump_index], D_8011F040[bump_index]);
            D_80140BE0[p] = 0.0f;
        }
    } else {
        bump_index = 0;
        if (D_80140808[p].point[4].flag) {
            high_force = 0.0f;
            for (i = 0; i < 3; i++) {
                if (fabsf(D_80140808[p].point[4].vector[i]) > high_force) {
                    high_force = fabsf(D_80140808[p].point[4].vector[i]);
                }
            }
            volume = range(high_force * 0.001f, 180.0f, 235.0f);
            volume = volume / 256.0f;
            if (fabsf(D_80140808[p].point[4].vector[0]) > high_force * 0.5f) {
                if (D_80140808[p].point[4].vector[0] < 0.0f) {
                    bump_index |= 1;
                } else {
                    bump_index |= 2;
                }
            }
            if (fabsf(D_80140808[p].point[4].vector[1]) > high_force * 0.5f) {
                if (D_80140808[p].point[4].vector[1] < 0.0f) {
                    bump_index |= 4;
                } else {
                    bump_index |= 8;
                }
            }
            if (fabsf(D_80140808[p].point[4].vector[2]) > high_force * 0.5f) {
                bump_index |= 1;
            }
            D_80140808[p].point[4].force = 0.0f;
            D_80140808[p].point[4].flag = 0;
        }
        if (bump_index) {
            high_scores_display((s32)(func_8008B2E4(13.0f) + 48.0f), m->player, 1, 1, volume,
                                D_8011F020[bump_index], D_8011F040[bump_index]);
        }
    }

    if (bump_index == 0 && m->speed > 20.0f) {
        scrape_side = 0;
        if (m->body_force[0][0] != 0.0f || m->body_force[2][0] != 0.0f) {
            scrape_side |= 1;
        }
        if (m->body_force[1][0] != 0.0f || m->body_force[3][0] != 0.0f) {
            scrape_side |= 2;
        }
        scrape_snd = 0;
        switch (D_80140A08[p]) {
        case 0:
            D_80140B10[p] = D_8002EB90;
            D_80140A08[p] = scrape_side;
            scrape_snd = scrape_side;
            break;
        case 1:
        case 2:
        case 3:
            if (scrape_side == 0) {
                scheduler_recv(D_80140AE0[p]);
                D_80140AE0[p] = -1;
                D_80140A08[p] = 0;
                D_80140B10[p] = 0.0f;
            }
            else if (D_8002EB90 - D_80140B10[p] > 0.1f || D_8002EB90 < D_80140B10[p]) {
                D_80140A08[p] += 3;
                scrape_snd = D_80140A08[p];
            }
            break;
        case 4:
        case 5:
        case 6:
            if (scrape_side == 0) {
                scheduler_recv(D_80140AE0[p]);
                D_80140AE0[p] = -1;
                D_80140A08[p] = 0;
                D_80140B10[p] = 0.0f;
            }
            break;
        }
        switch (scrape_snd) {
        case 1:
            high_scores_display((s32)(func_8008B2E4(13.0f) + 48.0f), m->player, 1, 1, 0.65f, 1, 0.0f);
            break;
        case 2:
            high_scores_display((s32)(func_8008B2E4(13.0f) + 48.0f), m->player, 1, 1, 0.65f, -1, 0.0f);
            break;
        case 3:
            high_scores_display((s32)(func_8008B2E4(13.0f) + 48.0f), m->player, 1, 1, 0.65f, 0.0f, 0.0f);
            break;
        case 4:
            D_80140AE0[p] = high_scores_display(19, m->player, 1, 2, 0.65f, 1, 0.0f);
            break;
        case 5:
            D_80140AE0[p] = high_scores_display(19, m->player, 1, 2, 0.65f, -1, 0.0f);
            break;
        case 6:
            D_80140AE0[p] = high_scores_display(19, m->player, 1, 2, 0.65f, 0.0f, 0.0f);
            break;
        }
    } else {
        scheduler_recv(D_80140AE0[p]);
        D_80140AE0[p] = -1;
        D_80140A08[p] = 0;
        D_80140B10[p] = 0.0f;
    }

    if (D_80142518[p] != 0.0f) {
        if (D_8002EB90 - D_80142518[p] > 0.16666667f || D_8002EB90 < D_80142518[p]) {
            D_80142518[p] = 0.0f;
        }
    }
    if (m->last_collision_kind > m->collision_kind) {
        m->last_collision_kind = m->collision_kind;
    }
    if (m->last_collision_kind != m->collision_kind) {
        m->last_collision_kind = m->collision_kind;
        if (D_80142518[p] == 0.0f) {
            D_80142518[p] = D_8002EB90;
            if (!(state_word_a & 0x200000)) {
                switch (m->last_collision_kind) {
                case 1:
                    high_scores_display(9, m->player, 1, 2, 1.0f, D_8011F020[m->collision_direction], D_8011F040[m->collision_direction]);
                    break;
                case 2:
                    high_scores_display(5, m->player, 1, 2, 1.0f, D_8011F020[m->collision_direction], D_8011F040[m->collision_direction]);
                    break;
                case 3:
                    high_scores_display(3, m->player, 1, 2, 1.0f, D_8011F020[m->collision_direction], D_8011F040[m->collision_direction]);
                    break;
                }
            }
        }
    }
}

void func_800DEF60(AudioModel2056 *m) {
    s16 k;

    for (k = 0; k < 4; k++) {
        func_800DED78(k, m->player, m->body_force[k], 5000.0f);
    }
    func_800DED78(4, m->player, m->center_force, 5000.0f);
}
