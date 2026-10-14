/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010DCFC (660 B) DRAFT: object hit/launch handler (attract/race_track4; types 236/237/339). Unit 145/165,
 * ops 11, frame 104 vs 136. Residual: retail computes &D_80117530[type] and &D_80152818[car] at entry and spills
 * them to the low spill area (48/60), i.e. not named locals; named locals are node (132) and a 48-byte mat
 * (84..132) plus 12 unexplained bytes (72..84). Probably inlined-helper parameters.
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
extern Node *D_80138FF0;

void model_data_load(s32, s32, s32);
void save_write_data(void *, s32, f32, s32);
Node *func_80090284(void);
void vector_normalize_length(void *, void *);
void math_utility(void *, void *);
s32 stat_lap_split(s32, s32, void *, u8);

void StartCone(Obj *obj)
{
    ObjType *type;
    Car *car;
    Node *node;
    Motion *m;
    f32 mat[4][3];

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
        if (!D_8014A250[obj->car].f1600) {
            D_8014A250[obj->car].f1600 = 1;
        }
        obj->flags &= ~6;
        return;
    }
    node = func_80090284();
    if (node == 0) {
        return;
    }
    node->f4 = 0;
    node->obj = obj;
    node->f20 = type->f12;
    node->f16 = 5.0f;
    obj->flags &= ~6;
    m = obj->motion;
    m->vel[0] = car->vel[0] * 0.125f;
    m->vel[1] = car->vel[1] * 0.125f;
    m->vel[2] = car->vel[2] * 0.125f;
    vector_normalize_length(m->vel, mat);
    if (obj->kind == 339 || obj->kind == 237) {
        m->vel[1] += 1.0f;
        m->f4 = 12.0f;
        m->f0 = 0.0f;
        m->f8 = 15.0f;
    } else {
        m->vel[1] += 2.0f;
        m->f4 = 12.0f;
        m->f0 = 0.0f;
        m->f8 = 15.0f;
    }
    math_utility(mat, obj->pos);
    node->next = D_80138FF0;
    D_80138FF0 = node;
    stat_lap_split(D_80117530[obj->type].f28, obj->car, obj->f56, 2);
}
