typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

float sqrtf(float);
#pragma intrinsic (sqrtf)

struct CamCtl;
typedef struct CamKey {          /* 0x44 bytes */
    f32 pos[3];                  /* 0x00 */
    f32 dir[3];                  /* 0x0C */
    f32 scale[3];                /* 0x18 */
    f32 rot[4];                  /* 0x24 */
    f32 f34;
    f32 dur;                     /* 0x38 */
    f32 f3C;
    s32 flags;                   /* 0x40 */
} CamKey;
typedef struct CamScene {
    u8 pad00[0x10];
    s32 flags;                   /* 0x10 */
    s16 count;                   /* 0x14 */
    s16 id;                      /* 0x16 */
    struct CamScene *link;       /* 0x18 */
    CamKey *keys;                /* 0x1C */
    struct CamCtl *cur;          /* 0x20 */
} CamScene;
typedef struct CamCtl {
    CamScene *scene;             /* 0x00 */
    f32 t;                       /* 0x04 */
    u8 pad08[4];
    s16 idx;                     /* 0x0C */
    u16 mode;                    /* 0x0E */
    f32 f10;                     /* 0x10 */
    f32 f14;
    f32 look[3];                 /* 0x18 */
    f32 mat[9];                  /* 0x24 */
} CamCtl;
typedef struct Camera {
    u8 pad00[0x38];
    f32 pos[3];                  /* 0x38 */
    u8 pad44[0x28];
    CamCtl *ctl;                 /* 0x6C */
} Camera;

typedef struct GameCar {
    u8 pad0[8];
    f32 pos[3];                  /* 0x08 */
    u8 pad14[0x3A4];
} GameCar; /* 0x3B8 */

extern GameCar player_array[];   /* 0x80152818 */
extern s32 gameplay_mode;        /* 0x8015A110 */
extern volatile f32 D_8002EB94;
void func_800C0294(s32 arg0, f32 *arg1);
