typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct Visual {
    struct Visual *next;    /* 0x00 */
    s16 index;              /* 0x04 */
    s16 objnum;             /* 0x06 */
    s16 slot;               /* 0x08 */
    u8 padA[2];
    void *data;             /* 0x0C */
    f32 timeStamp;          /* 0x10 */
    void *func;             /* 0x14 */
} Visual;

typedef struct TargInfo {
    u8 pad0[12];
    void *visFunc;          /* 0x0C */
    u8 pad10[12];
    s32 sound;              /* 0x1C */
    u8 pad20[16];
} TargInfo;                 /* 0x30 */

typedef struct Target {
    u8 pad0[4];
    u8 flags;               /* 0x04 */
    u8 pad5[11];
    s16 type;               /* 0x10 */
    u8 pad12[2];
    f32 uv[3][3];           /* 0x14 */

    f32 soundPos[3];        /* 0x38 */
    u8 pad44[22];
    s16 state;              /* 0x5A */
    s8 slot;                /* 0x5C */
} Target;

typedef struct GameCar {
    u8 pad0[20];
    f32 RWV[3];             /* 0x14 */
    u8 pad20[920];
} GameCar;                  /* 0x3B8 */

typedef struct MATRIX {
    f32 uvs[3][3];
    f32 pos[3];
} MATRIX;

extern TargInfo D_80117530[];
extern GameCar player_array[];
extern Visual *D_801391F0;

Visual *func_80090284(void);
void vector_normalize_length(f32 *v, f32 m[3][3]);
void math_utility(f32 src[3][3], f32 dst[3][3]);
s32 stat_lap_split(s32 sound, s32 slot, f32 *position, u8 mode);

void func_8010E72C(Target *t)
{
    Visual *v;
    MATRIX tmat;

    if (!(v = func_80090284())) {
        return;
    }
    v->index = 0;
    v->func = D_80117530[t->type].visFunc;
    v->data = t;
    v->timeStamp = 0.0333333f;
    t->state = 4;
    t->flags &= ~6;
    vector_normalize_length(player_array[t->slot].RWV, tmat.uvs);
    math_utility(tmat.uvs, t->uv);
    v->next = D_801391F0;
    D_801391F0 = v;
    stat_lap_split(D_80117530[t->type].sound, t->slot, t->soundPos, 2);
}
