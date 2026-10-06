/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Original targets.c copyright notice:
 * Copyright 1996 Time Warner Interactive.
 * Unauthorized reproduction, adaptation, distribution, performance or display
 * of this computer program or the associated audiovisual work is strictly prohibited.
 */
/* N64 adaptation of targets.c:StartCone, rushtherock 845329d7.
 * The 48-byte MATRIX is the actual LIB/fmath.h interface, not frame padding. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { f32 uvs[3][3]; f32 pos[3]; } MAT3;
typedef struct { f32 uvs[4][3]; } MAT4;
typedef struct { f32 xuv[3], yuv[3], zuv[3], pos[3]; } MATV;
typedef union { MAT3 mat3; MAT4 mat4; MATV matv; f32 uvs[4][3]; } MATRIX;
typedef struct { f32 angle[3], velocity[3]; } Motion;
typedef struct Target {
    u8 other0[4], flags, other5[7];
    s32 object;
    s16 descriptor;
    u8 other18[2];
    f32 orientation[3][3], position[3];
    u8 state[12];
    s16 type;
    u8 other82[10];
    s8 owner;
    u8 other93[15];
    Motion *motion;
} Target;
typedef struct Visual {
    struct Visual *next;
    s16 index;
    u8 other6[6];
    Target *target;
    f32 lifetime;
    void (*callback)(void *, s16);
} Visual;
typedef struct {
    u8 other0[12]; void (*callback)(void *, s16);
    u8 other16[4]; s16 type; u8 other22[6];
    s32 sound; u8 other32[16];
} Descriptor;
typedef struct {
    u8 other0[20]; f32 velocity[3];
    u8 other32[831]; s8 hit; u8 other864[88];
} Car;
typedef struct { u8 other0[1600]; s8 hit; u8 other1601[455]; } Model;
extern Descriptor D_80117530[];
extern Car D_80152818[];
extern Model D_8014A250[];
extern s8 D_8013FECC, D_8013FECD;
extern Visual *D_801391F0;
extern Visual *func_80090284(void);
extern void model_data_load(s32, s32, u32);
extern void save_write_data(void *, s32, f32, s32);
extern void vector_normalize_length(f32 *, f32 [3][3]);
extern void math_utility(void *, void *);
extern s32 stat_lap_split(s32, s32, f32 *, u8);
void func_8010DCFC(Target *t)
{
    Visual *v;
    MATRIX tmat;
    Descriptor *descriptor = &D_80117530[t->descriptor];
    Car *car = &D_80152818[t->owner];
    Motion *motion;
    f32 *velocity;
    Model *model;
    if (D_8013FECD && descriptor->type == 236) {
        car->hit = 1;
        model_data_load(t->object, 0, 15);
        t->flags &= ~6;
        return;
    }
    if (D_8013FECC && descriptor->type == 236) {
        model_data_load(t->object, 0, 15);
        save_write_data(t->state, 0, 0.4f, 1);
        model = &D_8014A250[t->owner];
        if (!model->hit) model->hit = 1;
        t->flags &= ~6;
        return;
    }
    if (!(v = func_80090284())) return;
    v->index = 0;
    v->callback = descriptor->callback;
    v->target = t;
    v->lifetime = 5.0f;
    t->flags &= ~6;
    motion = t->motion;
    velocity = motion->velocity;
    velocity[0] = car->velocity[0] * 0.125f;
    velocity[1] = car->velocity[1] * 0.125f;
    velocity[2] = car->velocity[2] * 0.125f;
    vector_normalize_length(velocity, tmat.mat3.uvs);
    if (t->type == 339 || t->type == 237) {
        velocity[1] += 1.0f;
        motion->angle[1] = 12.0f;
        motion->angle[0] = 0.0f;
        motion->angle[2] = 15.0f;
    } else {
        velocity[1] += 2.0f;
        motion->angle[1] = 12.0f;
        motion->angle[0] = 0.0f;
        motion->angle[2] = 15.0f;
    }
    math_utility(tmat.mat3.uvs, t->orientation);
    v->next = D_801391F0;
    D_801391F0 = v;
    stat_lap_split(D_80117530[t->descriptor].sound, t->owner, t->position, 2);
}
