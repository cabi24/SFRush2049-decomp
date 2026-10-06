/* w10a: w6c/w7d group with one change in func_800E7FA0: the segment bound is a named local
 *   lo = curve->level; hi = lo[4]; if (hi < sample) sample = hi;
 * which gives lo[4] retail's t3 (E7FA0 38 -> 35 words in the unit; E847C unchanged at 232).
 * Forcing the six phase-2 colours (segment t2, hi t3, lo[0] t4, level IV t5, lo s0, lo[1] s1) on the
 * w6c source leaves 2 rows (order of `move t2,zero` / `move s0,t0`): see ../RESULTS.md. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH: real group (func_800E7FA0 internal, func_800E847C kept);
 * blob_unit: func_800E847C 232/525 words (21 normalised rows), func_800E7FA0 38/311. Arcade ancestor of
 * func_800E847C: update_game_data (game/mdrive.c). See ../RESULTS.md. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct TireData {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ f32 tirer[4][3];
} TireData;

typedef struct CarParams {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 scale;
} CarParams;

typedef struct Model {
    /* 0x000 */ TireData *tires;
    /* 0x004 */ CarParams *params;
    /* 0x008 */ u8 body;
    /* 0x009 */ u8 pad9[0x40 - 0x9];
    /* 0x040 */ f32 V[3];
    /* 0x04C */ u8 pad4C[0x64 - 0x4C];
    /* 0x064 */ f32 force[4][3];
    /* 0x094 */ u8 pad94[0xC0 - 0x94];
    /* 0x0C0 */ f32 bodyforce[4][3];
    /* 0x0F0 */ u8 padF0[0x2E0 - 0xF0];
    /* 0x2E0 */ f32 f2E0[3];
    /* 0x2EC */ u8 pad2EC[0x3D4 - 0x2EC];
    /* 0x3D4 */ f32 crashflag;
    /* 0x3D8 */ u8 pad3D8[0x3F0 - 0x3D8];
    /* 0x3F0 */ f32 magvel;
    /* 0x3F4 */ u8 pad3F4[0x480 - 0x3F4];
    /* 0x480 */ f32 f480;
    /* 0x484 */ f32 f484;
    /* 0x488 */ u8 pad488[0x4DC - 0x488];
    /* 0x4DC */ f32 f4DC;
    /* 0x4E0 */ f32 f4E0;
    /* 0x4E4 */ u8 pad4E4[0x538 - 0x4E4];
    /* 0x538 */ f32 f538;
    /* 0x53C */ f32 f53C;
    /* 0x540 */ u8 pad540[0x594 - 0x540];
    /* 0x594 */ f32 f594;
    /* 0x598 */ f32 f598;
    /* 0x59C */ u8 pad59C[0x61C - 0x59C];
    /* 0x61C */ u16 sviscode[4];
    /* 0x624 */ u8 pad624[8];
    /* 0x62C */ u16 sound_flags[4];
    /* 0x634 */ u8 pad634[0x642 - 0x634];
    /* 0x642 */ s8 thumpflag;
    /* 0x643 */ u8 pad643[0x710 - 0x643];
    /* 0x710 */ u32 u710;
    /* 0x714 */ f32 f714;
    /* 0x718 */ u8 pad718[0x758 - 0x718];
    /* 0x758 */ s16 mph;
    /* 0x75A */ u8 pad75A[0x77C - 0x75A];
    /* 0x77C */ f32 vel[3];
    /* 0x788 */ f32 acc[3];
    /* 0x794 */ f32 pos[3];
    /* 0x7A0 */ f32 uvs[3][3];
    /* 0x7C4 */ u8 pad7C4[4];
    /* 0x7C8 */ s16 in_game;
    /* 0x7CA */ u8 pad7CA[0x7D4 - 0x7CA];
    /* 0x7D4 */ u32 appearance;
    /* 0x7D8 */ u8 pad7D8[0x7DD - 0x7D8];
    /* 0x7DD */ s8 data_valid;
    /* 0x7DE */ u8 pad7DE[0x7EA - 0x7DE];
    /* 0x7EA */ s8 f7EA;
    /* 0x7EB */ s8 collidable;
    /* 0x7EC */ u8 pad7EC[0x7F8 - 0x7EC];
    /* 0x7F8 */ f32 level[4];
} Model; /* 0x808 */


typedef struct Curve {
    /* 0x00 */ s32 level[5];
    /* 0x14 */ f32 first;
    /* 0x18 */ f32 second;
} Curve; /* 0x1C */

extern Model D_8014A250[];
extern u32 D_80120EBC[4];
extern u32 D_80120ECC[4];
extern u32 D_80120EDC[4];
extern Curve D_80120EEC[4];
extern s8 D_8013FECB;
extern s32 D_8014A110;

f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_800E7FA0(s16 index)
{
    s32 blocked;
    s32 alternate;
    s32 i;
    s32 sample;
    Model *car;
    s32 segment;
    s32 *lo;
    s32 hi;
    f32 a;
    f32 f;
    f32 magnitude;
    f32 scale;
    Curve *curve;

    car = &D_8014A250[index];
    blocked = car->sviscode[1] == 8 || car->sviscode[2] == 8;
    alternate = car->sviscode[1] == 2 || car->sviscode[2] == 2;
    for (i = 0; i < 4; i++) {
        if (car->sviscode[i] == 1) {
            car->appearance |= D_80120ECC[i];
        } else {
            car->appearance &= ~(D_80120EDC[i] | D_80120ECC[i]);
        }
    }
    if (blocked || alternate || D_8013FECB != 0) {
        for (i = 0; i < 4; i++) {
            car->level[i] = 0.0f;
            car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
        }
    } else {
        for (i = 0; i < 4; i++) {
            curve = &D_80120EEC[i];
            switch (i) {
            case 0:
                if (fabsf(car->f4DC) > fabsf(car->f594)) {
                    sample = fabsf(car->f4DC);
                } else {
                    sample = fabsf(car->f594);
                }
                break;
            case 1:
                if (fabsf(car->f480) > fabsf(car->f538)) {
                    sample = fabsf(car->f480);
                } else {
                    sample = fabsf(car->f538);
                }
                break;
            case 2:
                if (fabsf(car->f484) > fabsf(car->f4E0)) {
                    sample = fabsf(car->f484);
                } else {
                    sample = fabsf(car->f4E0);
                }
                break;
            case 3:
                sample = fabsf(car->f53C) + fabsf(car->f598);
                break;
            }
            magnitude = sample;
            if (D_8014A110 == 6) {
                scale = 3.0f;
            } else {
                scale = car->params->scale;
            }
            if (curve->level[0] * scale <= magnitude) {
                hi = curve->level[4];
                segment = 0;
                if (hi < sample) {
                    sample = hi;
                }
                while (segment < 4 && curve->level[segment + 1] < sample) {
                    segment++;
                }
                f = ((f32)(sample - curve->level[segment]) / (f32)(curve->level[segment + 1] - curve->level[segment]) + segment) * 0.25f;
                car->level[i] = f;
                if (curve->first < f) {
                    car->appearance |= D_80120EDC[i];
                }
                if (curve->second < car->level[i]) {
                    car->appearance |= D_80120EBC[i];
                }
            } else if (car->level[i] != 0.0f) {
                if (curve->first < car->level[i]) {
                    car->appearance |= D_80120EDC[i];
                }
                if (curve->second < car->level[i]) {
                    car->appearance |= D_80120EBC[i];
                }
                car->level[i] -= 0.125f;
                if (car->level[i] <= 0.0f) {
                    car->level[i] = 0.0f;
                    car->appearance &= ~(D_80120EDC[i] | D_80120EBC[i]);
                }
            }
        }
    }
}



typedef struct CarData {
    /* 0x000 */ u32 u0;
    /* 0x004 */ f32 f4;
    /* 0x008 */ f32 dr_pos[3];
    /* 0x014 */ f32 dr_acc[3];
    /* 0x020 */ f32 dr_vel[3];
    /* 0x02C */ f32 dr_uvs[3][3];
    /* 0x050 */ f32 uvs2[3][3];
    /* 0x074 */ f32 dr_tirepos[4][3];
    /* 0x0A4 */ f32 V[3];
    /* 0x0B0 */ f32 TIRER[4][3];
    /* 0x0E0 */ f32 roll;
    /* 0x0E4 */ f32 pitch;
    /* 0x0E8 */ u32 appearance;
    /* 0x0EC */ s8 data_valid;
    /* 0x0ED */ u8 padED[0xF8 - 0xED];
    /* 0x0F8 */ s16 mph;
    /* 0x0FA */ u8 padFA[0x308 - 0xFA];
    /* 0x308 */ s8 f308;
    /* 0x309 */ s8 collidable;
    /* 0x30A */ u8 pad30A[2];
    /* 0x30C */ f32 collide_time;
    /* 0x310 */ s8 collide_state;
    /* 0x311 */ s8 collide_count;
    /* 0x312 */ u8 pad312[0x344 - 0x312];
    /* 0x344 */ u16 sound_flags[4];
    /* 0x34C */ u8 pad34C[0x359 - 0x34C];
    /* 0x359 */ s8 f359;
    /* 0x35A */ u8 in_tunnel;
    /* 0x35B */ u8 pad35B[0x368 - 0x35B];
    /* 0x368 */ f32 f368[3];
    /* 0x374 */ u8 pad374[0x3B8 - 0x374];
} CarData; /* 0x3B8 */

typedef struct Slot {
    u8 pad0[7];
    u8 b7;
} Slot;


extern CarData D_80152818[];
extern Slot D_80153E88[];
extern s8 D_8011156C[];
extern f32 *D_80111560[];
extern f32 D_801543CC;
extern s32 D_80034840;

void osPfsChecker_full(void *);
void osStartThread(void *);
void battle_mode_setup(f32);
void math_utility(void *, void *);
void func_80090F44(f32, void *);
void func_8009EA68(f32, void *);
void func_8009E820(void *, void *, void *);

#define model D_8014A250
void func_800E847C(void) {
    s16 i;
    s16 j;
    s16 k;
    s16 l;
    s16 high_index;
    s16 same_count[4];
    Model *m;
    CarData *gc;
    f32 temp[3];
    f32 *curve;
    f32 value;
    u16 snd_flags;
    f32 bound;

    osPfsChecker_full(&D_80034840);
    battle_mode_setup(D_801543CC);
    for (i = 0; i < 6; i++) {
        gc = &D_80152818[i];
        if (!model[i].in_game || gc->f359 >= 2) {
            continue;
        }
        m = &model[i];
        gc->u0 = m->u710;
        gc->f4 = m->f714;
        gc->dr_pos[0] = m->pos[0];
        gc->dr_pos[1] = m->pos[1];
        gc->dr_pos[2] = m->pos[2];
        gc->f368[0] = m->f2E0[0];
        gc->f368[1] = m->f2E0[1];
        gc->f368[2] = m->f2E0[2];
        m->f2E0[0] = 0;
        m->f2E0[1] = 0;
        m->f2E0[2] = 0;
        math_utility(m->uvs, gc->dr_uvs);
        math_utility(m->uvs, gc->uvs2);
        temp[0] = m->force[0][0] + m->force[1][0];
        temp[1] = m->force[0][1] + m->force[1][1];
        temp[2] = m->force[0][2] + m->force[1][2];
        temp[0] += m->force[2][0];
        temp[1] += m->force[2][1];
        temp[2] += m->force[2][2];
        temp[0] += m->force[3][0];
        temp[1] += m->force[3][1];
        temp[2] += m->force[3][2];
        curve = D_80111560[D_8011156C[m->body]];
        value = temp[2] - curve[0];
        if (value > 0.0f) {
            bound = curve[2];
            value *= bound / curve[1];
            if (bound < value) {
                value = bound;
            }
        } else {
            value = temp[2] - curve[3];
            if (value < 0.0f) {
                bound = curve[5];
                value *= -bound / curve[4];
                if (value < bound) {
                    value = bound;
                }
            } else {
                value = 0.0f;
            }
        }
        gc->roll = gc->roll * 0.6f + 0.4f * value;
        func_80090F44(gc->roll, gc->uvs2);
        bound = curve[6];
        value = temp[0] - bound;
        if (value > 0.0f) {
            bound = curve[8];
            value *= -bound / curve[7];
            if (bound < value) {
                value = bound;
            }
        } else {
            value = temp[0] + bound;
            if (value < 0.0f) {
                bound = -curve[8];
                value *= bound / curve[7];
                if (value < bound) {
                    value = bound;
                }
            } else {
                value = 0.0f;
            }
        }
        gc->pitch = gc->pitch * 0.7f + 0.3f * value;
        func_8009EA68(gc->pitch, gc->uvs2);
        for (j = 0; j < 4; j++) {
            gc->TIRER[j][0] = m->tires->tirer[j][0];
            gc->TIRER[j][1] = m->tires->tirer[j][1];
            gc->TIRER[j][2] = m->tires->tirer[j][2];
            func_8009E820(m->tires->tirer[j], gc->dr_tirepos[j], gc->dr_uvs);
            gc->dr_tirepos[j][0] = gc->dr_tirepos[j][0] + gc->dr_pos[0];
            gc->dr_tirepos[j][1] = gc->dr_tirepos[j][1] + gc->dr_pos[1];
            gc->dr_tirepos[j][2] = gc->dr_tirepos[j][2] + gc->dr_pos[2];
        }
        gc->dr_acc[0] = m->acc[0];
        gc->dr_acc[1] = m->acc[1];
        gc->dr_acc[2] = m->acc[2];
        gc->dr_vel[0] = m->vel[0];
        gc->dr_vel[1] = m->vel[1];
        gc->dr_vel[2] = m->vel[2];
        if (D_80153E88[i].b7 == 0 || D_80153E88[i].b7 == 6) {
            high_index = 0;
            for (k = 0; k < 4; k++) {
                gc->sound_flags[k] = m->sound_flags[k];
                same_count[k] = 0;
                if (m->sviscode[k] != 8 || m->sound_flags[k] != 0) {
                    for (l = k + 1; l < 4; l++) {
                        if (m->sound_flags[k] == m->sound_flags[l]) {
                            same_count[k]++;
                        }
                    }
                }
                if (same_count[k] >= same_count[high_index]) {
                    high_index = k;
                    snd_flags = m->sound_flags[k];
                }
            }
            gc->in_tunnel = (snd_flags & 0x100) >> 8;
            if (m->crashflag != 0.0f) {
                m->appearance |= 0x1000;
            } else {
                m->appearance &= ~0x1000;
            }
            if (m->magvel > 20.0f && m->bodyforce[0][1] != 0.0f) {
                m->appearance |= 0x80;
            } else {
                m->appearance &= ~0x80;
            }
            if (m->magvel > 20.0f && m->bodyforce[2][1] != 0.0f) {
                m->appearance |= 0x4000;
            } else {
                m->appearance &= ~0x4000;
            }
            if (m->magvel > 20.0f && m->bodyforce[1][1] != 0.0f) {
                m->appearance |= 0x100;
            } else {
                m->appearance &= ~0x100;
            }
            if (m->magvel > 20.0f && m->bodyforce[3][1] != 0.0f) {
                m->appearance |= 0x2000;
            } else {
                m->appearance &= ~0x2000;
            }
            if (m->thumpflag > 2) {
                m->appearance |= 0x800;
            } else if (m->thumpflag == 0) {
                m->appearance &= ~0x800;
            }
            if (gc->collidable && !m->collidable) {
                m->appearance &= ~8;
            } else if (!gc->collidable && m->collidable) {
                m->appearance |= 8;
            }
            func_800E7FA0(i);
            gc->V[0] = m->V[0];
            gc->V[1] = m->V[1];
            gc->V[2] = m->V[2];
            gc->mph = m->mph;
        }
        gc->appearance = m->appearance;
        m->appearance &= ~0x100000;
        gc->f308 = m->f7EA;
        gc->collidable = m->collidable;
        if (gc->collidable == 0 && gc->collide_time == 0.0f) {
            gc->collide_count = -1;
            gc->collide_state = -1;
            gc->collide_time = m->f714;
        }
        if (gc->collidable == 1) {
            gc->collide_time = 0.0f;
        }
        gc->data_valid = m->data_valid;
    }
    osStartThread(&D_80034840);
}
