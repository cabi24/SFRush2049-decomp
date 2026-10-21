/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E847C = update_game_data (MAT3 mdrive.c, GC 0x8002B064); func_800E7FA0 = its static
 * callee _UpdateTireAppearance (MAT3 mdrive.c, GC 0x8002B6CC; called only from here, clobbers s0-s5).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct _tCar {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ f32 tireR[4][3];
} _tCar;

typedef struct CarParams {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 skidk;
} CarParams;


typedef struct tModelData {
    /* 0x000 */ _tCar *car;
    /* 0x004 */ CarParams *tune;
    /* 0x008 */ u8 body_type;
    /* 0x009 */ u8 pad009[0x40 - 0x9];
    /* 0x040 */ f32 v[3];
    /* 0x04C */ u8 pad04C[0x64 - 0x4C];
    /* 0x064 */ f32 tireForce[4][3];
    /* 0x094 */ u8 pad094[0xC4 - 0x94];
    /* 0x0C4 */ f32 bodyForce[4][3];
    /* 0x0F4 */ u8 pad0F4[0x2E0 - 0xF4];
    /* 0x2E0 */ f32 stuntW[3];
    /* 0x2EC */ u8 pad2EC[0x3D4 - 0x2EC];
    /* 0x3D4 */ f32 brake;
    /* 0x3D8 */ u8 pad3D8[0x3F0 - 0x3D8];
    /* 0x3F0 */ f32 magvel;
    /* 0x3F4 */ u8 pad3F4[0x61C - 0x3F4];
    /* 0x61C */ u16 sviscode[4];
    /* 0x624 */ u8 pad624[0x62C - 0x624];
    /* 0x62C */ u16 sound_flags[4];
    /* 0x634 */ u8 pad634[0x642 - 0x634];
    /* 0x642 */ s8 thumpflag;
    /* 0x643 */ u8 pad643[0x710 - 0x643];
    /* 0x710 */ s32 modelIteration;
    /* 0x714 */ f32 modelTime;
    /* 0x718 */ u8 pad718[0x758 - 0x718];
    /* 0x758 */ s16 rwmph;
    /* 0x75A */ u8 pad75A[0x77C - 0x75A];
    /* 0x77C */ f32 rwa[3];
    /* 0x788 */ f32 rwv[3];
    /* 0x794 */ f32 rwr[3];
    /* 0x7A0 */ f32 uvs[3][3];
    /* 0x7C4 */ u8 pad7C4[4];
    /* 0x7C8 */ s16 in_game;
    /* 0x7CA */ u8 pad7CA[0x7D4 - 0x7CA];
    /* 0x7D4 */ u32 appearance;
    /* 0x7D8 */ u8 pad7D8[0x7DD - 0x7D8];
    /* 0x7DD */ s8 data_valid;
    /* 0x7DE */ u8 pad7DE[0x7EA - 0x7DE];
    /* 0x7EA */ s8 collidable;
    /* 0x7EB */ s8 solid;
    /* 0x7EC */ u8 pad7EC[0x7F8 - 0x7EC];
    /* 0x7F8 */ f32 skidIntensity[4];
} tModelData; /* 0x808 */

typedef struct tCarData {
    /* 0x000 */ s32 modelIteration;
    /* 0x004 */ f32 modelTime;
    /* 0x008 */ f32 dr_pos[3];
    /* 0x014 */ f32 dr_vel[3];
    /* 0x020 */ f32 dr_acc[3];
    /* 0x02C */ f32 dr_uvs[3][3];
    /* 0x050 */ f32 dr_car_uvs[3][3];
    /* 0x074 */ f32 dr_tirepos[4][3];
    /* 0x0A4 */ f32 v[3];
    /* 0x0B0 */ f32 tireR[4][3];
    /* 0x0E0 */ f32 pitch;
    /* 0x0E4 */ f32 roll;
    /* 0x0E8 */ u32 appearance;
    /* 0x0EC */ s8 data_valid;
    /* 0x0ED */ u8 pad0ED[0xF8 - 0xED];
    /* 0x0F8 */ s16 rwmph;
    /* 0x0FA */ u8 pad0FA[0x308 - 0xFA];
    /* 0x308 */ s8 collidable;
    /* 0x309 */ s8 solid;
    /* 0x30C */ f32 collide_time;
    /* 0x310 */ s8 collide_count;
    /* 0x311 */ s8 collide_state;
    /* 0x312 */ u8 pad312[0x344 - 0x312];
    /* 0x344 */ u16 sound_flags[4];
    /* 0x34C */ u8 pad34C[0x359 - 0x34C];
    /* 0x359 */ s8 dead;
    /* 0x35A */ s8 in_tunnel;
    /* 0x35B */ u8 pad35B[0x368 - 0x35B];
    /* 0x368 */ f32 stuntW[3];
    /* 0x374 */ u8 pad374[0x3B8 - 0x374];
} tCarData; /* 0x3B8 */

typedef struct _tSusp {
    f32 fwdforcemin;
    f32 fwdforcerange;
    f32 fwdangle;
    f32 bckforcemin;
    f32 bckforcerange;
    f32 bckangle;
    f32 sideforcemin;
    f32 sideforcerange;
    f32 sideangle;
} _tSusp;

typedef struct tLink { u8 pad[7]; u8 owner; } tLink;

extern tModelData D_8014A250[];   /* model */
extern tCarData D_80152818[];     /* gGameCar */
extern tLink D_80153E88[];        /* gLink */
extern f32 D_801543CC;            /* gModelTime */
extern const _tSusp *D_80111560[]; /* carSuspensions */
extern s8 D_8011156C[];           /* gCarSuspensions */
extern u8 D_80034840[];           /* model thread */

void osStopThread(void *t);
void osStartThread(void *t);
void battle_mode_setup(f32 time);                    /* game_reckon_all */
void math_utility(void *src, void *dst);             /* CopyUV */
void func_80090F44(f32 angle, f32 uv[][3]);          /* PitchUV */
void func_8009EA68(f32 angle, f32 uv[][3]);          /* RollUV */
void func_8009E820(f32 *v, f32 *out, f32 uv[][3]);   /* WorldVector */
void func_800E7FA0(s16 droneIndex);

typedef struct tSkidData {
    s32 forceTable[5];
    f32 skidThreshold;
    f32 smokeThreshold;
} tSkidData; /* 0x1C */

typedef struct tTireState { u8 pad0[0x50]; f32 sideforce; f32 traction; u8 pad58[4]; } tTireState; /* 0x5C */
#define TIRES(m) ((tTireState *)((u8 *)(m) + 0x430))

extern u32 D_80120EBC[4];          /* sSmoke */
extern u32 D_80120ECC[4];          /* sSmoke2 */
extern u32 D_80120EDC[4];          /* sSkid */
extern tSkidData D_80120EEC[4];    /* sSkidData */
extern s8 D_8013FECB;              /* gCoastFlag */
extern s32 D_8014A110;             /* gRace */

f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_800E7FA0(s16 droneIndex)
{
    s32 index;
    s32 side;
    s32 forceLow;
    s32 force;
    tModelData *m = &D_8014A250[droneIndex];
    tSkidData *skidData;
    s32 airborne = m->sviscode[1] == 8 || m->sviscode[2] == 8;
    s32 wet = m->sviscode[1] == 2 || m->sviscode[2] == 2;

    for (side = 0; side < 4; side++) {
        if (m->sviscode[side] == 1) {
            m->appearance |= D_80120ECC[side];
        } else {
            m->appearance &= ~(D_80120EDC[side] | D_80120ECC[side]);
        }
    }
    if (airborne || wet || D_8013FECB) {
        for (side = 0; side < 4; side++) {
            m->skidIntensity[side] = 0;
            m->appearance &= ~(D_80120EDC[side] | D_80120EBC[side]);
        }
    } else {
        for (side = 0; side < 4; side++) {
            skidData = &D_80120EEC[side];
            switch (side) {
            case 0:
                force = fabsf(TIRES(m)[1].sideforce) > fabsf(TIRES(m)[3].sideforce) ?
                    fabsf(TIRES(m)[1].sideforce) : fabsf(TIRES(m)[3].sideforce);
                break;
            case 1:
                force = fabsf(TIRES(m)[0].sideforce) > fabsf(TIRES(m)[2].sideforce) ?
                    fabsf(TIRES(m)[0].sideforce) : fabsf(TIRES(m)[2].sideforce);
                break;
            case 2:
                force = fabsf(TIRES(m)[0].traction) > fabsf(TIRES(m)[1].traction) ?
                    fabsf(TIRES(m)[0].traction) : fabsf(TIRES(m)[1].traction);
                break;
            case 3:
                force = fabsf(TIRES(m)[2].traction) + fabsf(TIRES(m)[3].traction);
                break;
            }
            {
                f32 scale = D_8014A110 == 6 ? 3.0f : m->tune->skidk;
                f32 intensity;
                s32 maxForce;

                if (force >= scale * skidData->forceTable[0]) {
                    maxForce = skidData->forceTable[4];
                    if (force > maxForce) {
                        force = maxForce;
                    }
                    for (index = 0; index < 4; index++) {
                        if (force <= skidData->forceTable[index + 1]) {
                            break;
                        }
                    }
                    forceLow = skidData->forceTable[index];
                    if (maxForce) {}
                    intensity = 0.25f * (index +
                        (f32)(force - forceLow) / (skidData->forceTable[index + 1] - forceLow));
                    m->skidIntensity[side] = intensity;
                    if (intensity > skidData->skidThreshold) {
                        m->appearance |= D_80120EDC[side];
                    }
                    if (m->skidIntensity[side] > skidData->smokeThreshold) {
                        m->appearance |= D_80120EBC[side];
                    }
                } else if (m->skidIntensity[side]) {
                    if (m->skidIntensity[side] > skidData->skidThreshold) {
                        m->appearance |= D_80120EDC[side];
                    }
                    if (m->skidIntensity[side] > skidData->smokeThreshold) {
                        m->appearance |= D_80120EBC[side];
                    }
                    m->skidIntensity[side] -= 0.125f;
                    if (m->skidIntensity[side] <= 0) {
                        m->skidIntensity[side] = 0;
                        m->appearance &= ~(D_80120EDC[side] | D_80120EBC[side]);
                    }
                }
            }
        }
    }
}

void func_800E847C(void)
{
    s16 i;
    s16 j;
    s16 k;
    s16 l;
    s16 high_index;
    s16 same_count[4];
    tModelData *m;
    tCarData *gc;
    f32 force[3];
    f32 angle;
    const _tSusp *susp;
    u16 tsf;

    osStopThread(D_80034840);
    battle_mode_setup(D_801543CC);
    for (i = 0; i < 6; i++) {
        m = &D_8014A250[i];
        gc = &D_80152818[i];
        if (m->in_game && gc->dead < 2) {
            gc->modelIteration = m->modelIteration;
            gc->modelTime = m->modelTime;
            gc->dr_pos[0] = m->rwr[0];
            gc->dr_pos[1] = m->rwr[1];
            gc->dr_pos[2] = m->rwr[2];
            gc->stuntW[0] = m->stuntW[0];
            gc->stuntW[1] = m->stuntW[1];
            gc->stuntW[2] = m->stuntW[2];
            m->stuntW[0] = 0.0f;
            m->stuntW[1] = 0.0f;
            m->stuntW[2] = 0.0f;
            math_utility(m->uvs, gc->dr_uvs);
            math_utility(m->uvs, gc->dr_car_uvs);
            susp = D_80111560[D_8011156C[m->body_type]];
            force[0] = m->tireForce[0][0] + m->tireForce[1][0];
            force[1] = m->tireForce[0][1] + m->tireForce[1][1];
            force[2] = m->tireForce[0][2] + m->tireForce[1][2];
            force[0] = m->tireForce[2][0] + force[0];
            force[1] = m->tireForce[2][1] + force[1];
            force[2] = m->tireForce[2][2] + force[2];
            force[0] = m->tireForce[3][0] + force[0];
            force[1] = m->tireForce[3][1] + force[1];
            force[2] = m->tireForce[3][2] + force[2];
            if ((angle = force[2] - susp->fwdforcemin) > 0) {
                angle *= susp->fwdangle / susp->fwdforcerange;
                if (angle > susp->fwdangle) angle = susp->fwdangle;
            } else {
                if ((angle = force[2] - susp->bckforcemin) < 0) {
                    angle *= -susp->bckangle / susp->bckforcerange;
                    if (angle < susp->bckangle) angle = susp->bckangle;
                } else angle = 0;
            }
            gc->pitch = 0.6f * gc->pitch + 0.4f * angle;
            func_80090F44(gc->pitch, gc->dr_car_uvs);
            if ((angle = force[0] - susp->sideforcemin) > 0) {
                angle *= -susp->sideangle / susp->sideforcerange;
                if (angle > susp->sideangle) angle = susp->sideangle;
            } else {
                if ((angle = force[0] + susp->sideforcemin) < 0) {
                    angle *= -susp->sideangle / susp->sideforcerange;
                    if (angle < -susp->sideangle) angle = -susp->sideangle;
                } else angle = 0;
            }
            gc->roll = 0.7f * gc->roll + 0.3f * angle;
            func_8009EA68(gc->roll, gc->dr_car_uvs);
            for (j = 0; j < 4; j++) {
                gc->tireR[j][0] = m->car->tireR[j][0];
                gc->tireR[j][1] = m->car->tireR[j][1];
                gc->tireR[j][2] = m->car->tireR[j][2];
                func_8009E820(m->car->tireR[j], gc->dr_tirepos[j], gc->dr_uvs);
                gc->dr_tirepos[j][0] = gc->dr_tirepos[j][0] + gc->dr_pos[0];
                gc->dr_tirepos[j][1] = gc->dr_tirepos[j][1] + gc->dr_pos[1];
                gc->dr_tirepos[j][2] = gc->dr_tirepos[j][2] + gc->dr_pos[2];
            }
            gc->dr_vel[0] = m->rwv[0];
            gc->dr_vel[1] = m->rwv[1];
            gc->dr_vel[2] = m->rwv[2];
            gc->dr_acc[0] = m->rwa[0];
            gc->dr_acc[1] = m->rwa[1];
            gc->dr_acc[2] = m->rwa[2];
            if (D_80153E88[i].owner == 0 || D_80153E88[i].owner == 6) {
                high_index = 0;
                for (k = 0; k < 4; k++) {
                    gc->sound_flags[k] = m->sound_flags[k];
                    same_count[k] = 0;
                    if (m->sviscode[k] != 8 || m->sound_flags[k]) {
                        for (l = k + 1; l < 4; l++) {
                            if (m->sound_flags[k] == m->sound_flags[l]) same_count[k]++;
                        }
                    }
                    if (same_count[k] >= same_count[high_index]) {
                        high_index = k;
                        tsf = m->sound_flags[k];
                    }
                }
                gc->in_tunnel = (tsf & 0x100) >> 8;
                if (m->brake) m->appearance |= 0x1000;
                else m->appearance &= ~0x1000;
                if (m->magvel > 20.0f && m->bodyForce[0][0]) m->appearance |= 0x80;
                else m->appearance &= ~0x80;
                if (m->magvel > 20.0f && m->bodyForce[2][0]) m->appearance |= 0x4000;
                else m->appearance &= ~0x4000;
                if (m->magvel > 20.0f && m->bodyForce[1][0]) m->appearance |= 0x100;
                else m->appearance &= ~0x100;
                if (m->magvel > 20.0f && m->bodyForce[3][0]) m->appearance |= 0x2000;
                else m->appearance &= ~0x2000;
                if (m->thumpflag > 2) m->appearance |= 0x800;
                else if (!m->thumpflag) m->appearance &= ~0x800;
                if (gc->solid && !m->solid) m->appearance &= ~8;
                else if (!gc->solid && m->solid) m->appearance |= 8;
                func_800E7FA0(i);
                gc->v[0] = m->v[0];
                gc->v[1] = m->v[1];
                gc->v[2] = m->v[2];
                gc->rwmph = m->rwmph;
            }
            gc->appearance = m->appearance;
            m->appearance &= ~0x100000;
            gc->collidable = m->collidable;
            gc->solid = m->solid;
            if (!gc->solid && gc->collide_time == 0) {
                gc->collide_time = m->modelTime;
                gc->collide_state = -1;
                gc->collide_count = -1;
            }
            if (gc->solid == 1) gc->collide_time = 0;
            gc->data_valid = m->data_valid;
        }
    }
    osStartThread(D_80034840);
}
