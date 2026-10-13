/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010DCFC (0x8010DCFC, 660 B) = GC _StartCone@targets.c (rush_d.elf 0x8004E6BC; static void
 * _StartCone(struct tTarget *t)). Starts a hit traffic cone flying: with gCarMines (D_8013FECD) and target id
 * 236 (GC 0x226) the car's f863 flag is set and the model hidden (MBOX_HideObject = model_data_load(model, 0,
 * 15)); with gMineCones (D_8013FECC) the cone explodes instead (StartBlast = save_write_data(pos, 0, 0.4f, 1))
 * and the car's lap-record byte +1600 is set once (GC: model[car] stride 0x808, byte 0x640); otherwise a tVisual
 * entry is grabbed (GrabEnvEntry = func_80090284), filled (index 0, func = targetInfo[type].func, data = target,
 * timeStamp 5.0), the target's velocity set to the car's velocity * 0.125, LookInDir = vector_normalize_length,
 * vel[1] += 1.0 for kinds 339/237 (GC 0x28D/0x227) else 2.0, angular velocity (0, 12, 15), CopyUV =
 * math_utility(tmat, pos), the entry linked into the environment list (AddToEnvList inlined: head D_801391F0)
 * and PlayTargetSound = stat_lap_split(targetInfo[type].sound, car, pos2, 2).
 * GC locals v (here `vis`, a tVisual), tmat, vel, angV, car, tInfo; N64 names follow them.
 *
 * w28c draft 145/165 -> w29b 147/165 -> w30b MATCH. Levers (w30b):
 *  - `vel = t->motion->vel;` computed from the target directly, BEFORE `angV = t->motion;` (147 -> 51 words;
 *    with vel derived from angV uopt folds the three vel stores into angV+12 offsets and tInfo loses t0);
 *  - the lap-flag helper takes the record pointer (`Flag1600(&D_8014A250[car])`), not the car index (-> 28);
 *  - tVisual stores in retail order index, func, data, timeStamp (-> 19);
 *  - angular-velocity stores in GC order 0, 12, 15 (FP ring $f16/$f18 as retail; -> 11);
 *  - GrabEnvEntry/AddToEnvList NOT helpers (GC calls them, N64 retail inlines/calls directly): only one
 *    inlined helper area (8 B, Flag1600) under the named locals.
 * Shaping devices (disclosed):
 *  - Flag1600 is an inlined static helper of unknown identity (GC has the same test-and-set inline in
 *    _StartCone); it supplies the 8-byte inline area at 32..39 and the retail colouring of the record pointer.
 *  - FIVE UNUSED locals u1..u5 = exact frame residual. Retail frame 136, 112 without them (17 words differ:
 *    every spill home 8-20 B off). Measured homes (spcensus): vis 132, tmat 84..131, u1..u3 72..80, vel 68,
 *    angV 64, car 60, u4/u5 52..56, tInfo 48 = retail's `sw v1,132`, `sw a0,68`, `sw v0,64`, `sw a3,60`,
 *    `sw t0,48`. Their identity is unknown (N64-only locals absent from the GC build).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Motion {
    f32 f0, f4, f8;
    f32 vel[3];
} Motion;

typedef struct Obj {
    u8 pad0[4];
    u8 flags;
    u8 pad5[7];
    s32 model;
    s16 type;
    u8 pad12[2];
    f32 pos[3];
    u8 pad20[56 - 32];
    u8 f56[12];
    u8 f68[12];
    s16 kind;
    u8 pad52[92 - 82];
    s8 car;
    u8 pad5D[108 - 93];
    Motion *motion;
} Obj;

typedef struct ObjType {
    u8 pad0[12];
    s32 f12;
    u8 pad10[4];
    s16 id;
    u8 pad16[6];
    s32 f28;
    u8 pad20[48 - 32];
} ObjType;

typedef struct Car {
    u8 pad0[20];
    f32 vel[3];
    u8 pad20[863 - 32];
    u8 f863;
    u8 pad360[952 - 864];
} Car;

typedef struct Lap {
    u8 pad0[1600];
    s8 f1600;
    u8 pad641[2056 - 1601];
} Lap;

typedef struct Node {
    struct Node *next;
    s16 f4;
    u8 pad6[6];
    Obj *obj;
    f32 f16;
    s32 f20;
} Node;

extern s8 D_8013FECD;
extern s8 D_8013FECC;
extern ObjType D_80117530[];
extern Car D_80152818[];
extern Lap D_8014A250[];
extern Node *D_801391F0;

void model_data_load(s32, s32, s32);
void save_write_data(void *, s32, f32, s32);
Node *func_80090284(void);
void vector_normalize_length(void *, void *);
void math_utility(void *, void *);
s32 stat_lap_split(s32, s32, void *, u8);

static void Flag1600(Lap *lap)
{
    if (!lap->f1600) {
        lap->f1600 = 1;
    }
}

void func_8010DCFC(Obj *obj)
{
    Node *vis;
    f32 tmat[4][3];
    s32 u1;
    s32 u2;
    s32 u3;
    f32 *vel;
    Motion *angV;
    Car *car;
    s32 u4;
    s32 u5;
    ObjType *tInfo;

    tInfo = &D_80117530[obj->type];
    car = &D_80152818[obj->car];
    if (D_8013FECD && tInfo->id == 236) {
        car->f863 = 1;
        model_data_load(obj->model, 0, 15);
        obj->flags &= ~6;
        return;
    }
    if (D_8013FECC && tInfo->id == 236) {
        model_data_load(obj->model, 0, 15);
        save_write_data(obj->f68, 0, 0.4f, 1);
        Flag1600(&D_8014A250[obj->car]);
        obj->flags &= ~6;
        return;
    }
    vis = func_80090284();
    if (vis == 0) {
        return;
    }
    vis->f4 = 0;
    vis->f20 = tInfo->f12;
    vis->obj = obj;
    vis->f16 = 5.0f;
    obj->flags &= ~6;
    vel = obj->motion->vel;
    angV = obj->motion;
    vel[0] = car->vel[0] * 0.125f;
    vel[1] = car->vel[1] * 0.125f;
    vel[2] = car->vel[2] * 0.125f;
    vector_normalize_length(vel, tmat);
    if (obj->kind == 339 || obj->kind == 237) {
        vel[1] += 1.0f;
        angV->f0 = 0.0f;
        angV->f4 = 12.0f;
        angV->f8 = 15.0f;
    } else {
        vel[1] += 2.0f;
        angV->f0 = 0.0f;
        angV->f4 = 12.0f;
        angV->f8 = 15.0f;
    }
    math_utility(tmat, obj->pos);
    vis->next = D_801391F0;
    D_801391F0 = vis;
    stat_lap_split(D_80117530[obj->type].f28, obj->car, obj->f56, 2);
}
