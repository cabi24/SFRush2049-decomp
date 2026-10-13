/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010DCFC (0x8010DCFC, 660 B) DRAFT (w29b) = GC _StartCone@targets.c (same structure: gCarMines/gMineCones
 * flags D_8013FECD/D_8013FECC with target id 236 (GC 0x226), MBOX_HideObject = model_data_load(model, 0, 15),
 * StartBlast = save_write_data(pos, 0, 0.4f, 1), GrabEnvEntry = func_80090284, 5.0f, vel = car vel * 0.125f,
 * LookInDir = vector_normalize_length(vel, tmat), kinds 339/237 (GC 0x28D/0x227) +1 else +2, 12/0/15,
 * CopyUV = math_utility(tmat, pos), AddToEnvList (inlined: next = D_801391F0 list head; w28c had D_80138FF0, wrong: found by draft_check), PlayTargetSound =
 * stat_lap_split(targetInfo[type].f28, car, pos2, 2)). GC locals: v, tmat, vel, angV, car, tInfo.
 * Unit: 147/165 words, rows 89, ops 10, frame 136/136 (w28c: 145/165, rows 99, ops 11, frame 136/104).
 * Frame closed by inlined helpers AddToEnvList(e) (+8), GrabEnvEntry() (+8) and Flag1600(car) (+8): node at
 * 132 and tmat at 84..131 as retail. HYPOTHESIS: these three as source-level static helpers are a frame fit,
 * not proven (GC calls GrabEnvEntry/AddToEnvList as functions; the Lap f1600 flag set is inline on GC).
 * Residual: colouring (retail tInfo t0 / car a3 at entry, ours a2/a3; spill homes 48/60/64/68 vs ours) and the
 * second id-236 branch shape (retail bne + call in fallthrough, ours bnel). Declaration order of the pointer
 * locals has no effect here (24 permutations: identical output).
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

static void Flag1600(s32 car)
{
    if (!D_8014A250[car].f1600) {
        D_8014A250[car].f1600 = 1;
    }
}

static Node *GrabEnvEntry(void)
{
    return func_80090284();
}

static void AddToEnvList(Node *e)
{
    e->next = D_801391F0;
    D_801391F0 = e;
}

void func_8010DCFC(Obj *obj)
{
    Node *node;
    f32 tmat[4][3];
    Motion *v;
    f32 *vel;
    f32 *angV;
    ObjType *type;
    Car *car;

    type = &D_80117530[obj->type];
    car = &D_80152818[obj->car];
    if (D_8013FECD && type->id == 236) {
        car->f863 = 1;
        model_data_load(obj->model, 0, 15);
        obj->flags &= ~6;
        return;
    }
    if (D_8013FECC && type->id == 236) {
        model_data_load(obj->model, 0, 15);
        save_write_data(obj->f68, 0, 0.4f, 1);
        Flag1600(obj->car);
        obj->flags &= ~6;
        return;
    }
    node = GrabEnvEntry();
    if (node == 0) {
        return;
    }
    node->f4 = 0;
    node->obj = obj;
    node->f20 = type->f12;
    node->f16 = 5.0f;
    obj->flags &= ~6;
    v = obj->motion;
    vel = v->vel;
    vel[0] = car->vel[0] * 0.125f;
    vel[1] = car->vel[1] * 0.125f;
    vel[2] = car->vel[2] * 0.125f;
    vector_normalize_length(vel, tmat);
    if (obj->kind == 339 || obj->kind == 237) {
        vel[1] += 1.0f;
        v->f4 = 12.0f;
        v->f0 = 0.0f;
        v->f8 = 15.0f;
    } else {
        vel[1] += 2.0f;
        v->f4 = 12.0f;
        v->f0 = 0.0f;
        v->f8 = 15.0f;
    }
    math_utility(tmat, obj->pos);
    AddToEnvList(node);
    stat_lap_split(D_80117530[obj->type].f28, obj->car, obj->f56, 2);
}
