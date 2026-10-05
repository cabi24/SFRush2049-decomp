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
    f32 pos[3];             /* 0x14 */
    u8 pad20[12];
    f32 dir[3];             /* 0x2C */
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

extern TargInfo D_80117530[];
extern GameCar player_array[];
extern Visual *D_801391F0;

Visual *func_80090284(void);
void func_80090E9C(f32 arg0, void *arg1);
s32 stat_lap_split(s32 sound, s32 slot, f32 *position, u8 mode);

void func_8010DBB8(Target *t)
{
    TargInfo *info = &D_80117530[t->type];
    Visual *v;
    GameCar *car;
    f32 dot;

    t->state = 7;
    if (!(v = func_80090284())) {
        return;
    }
    v->index = 0;
    v->func = info->visFunc;
    v->data = t;
    v->timeStamp = 0.0333333f;
    car = &player_array[t->slot];
    dot = car->RWV[0] * t->dir[0] + car->RWV[1] * t->dir[1] + car->RWV[2] * t->dir[2];
    if (dot > 0.0f) {
        func_80090E9C(3.1415927f, t->pos);
    }
    t->flags &= ~6;
    v->next = D_801391F0;
    D_801391F0 = v;
    stat_lap_split(D_80117530[t->type].sound, t->slot, t->soundPos, 2);
}
