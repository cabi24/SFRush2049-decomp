typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef union Color4 {
    u32 word;
    u8 b[4];
} Color4;

typedef struct Marker {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 view;
    /* 0x06 */ s16 mesh;
    /* 0x08 */ s16 player;
} Marker;

typedef struct Car {                 /* D_80152818[], 0x3B8 */
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ f32 pos[3];
    /* 0x014 */ u8 pad14[0x344];
    /* 0x358 */ s8 hidden;
    /* 0x359 */ u8 pad359[0x33];
    /* 0x38C */ u32 flags;
    /* 0x390 */ u8 pad390[0x10];
    /* 0x3A0 */ s8 colorIndex;
    /* 0x3A1 */ u8 alpha;
    /* 0x3A2 */ s8 alphaMode;
    /* 0x3A3 */ u8 pad3A3[0x15];
} Car;

typedef struct Model {               /* D_8014A250[], 0x808 */
    /* 0x000 */ u8 pad0[0x6C4];
    /* 0x6C4 */ s16 state;
    /* 0x6C6 */ u8 pad6C6[0x100];
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ u8 pad7C8[4];
    /* 0x7CC */ s8 mode;
    /* 0x7CD */ u8 pad7CD[0x3B];
} Model;

typedef struct ViewCam {             /* D_80150B70[], 0x98 */
    /* 0x00 */ f32 proj[9];
    /* 0x24 */ f32 pos[3];
    /* 0x30 */ f32 basis[9];
    /* 0x54 */ u8 pad54[0x44];
} ViewCam;

typedef struct ViewInfo {            /* D_8017A510[], 0x48 */
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 scale;
    /* 0x14 */ u8 pad14[0x34];
} ViewInfo;

typedef struct Mesh {                /* D_8015B268[], 0x58 */
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x54];
} Mesh;

extern Car D_80152818[];
extern Model D_8014A250[];
extern ViewCam D_80150B70[];
extern ViewInfo D_8017A510[];
extern Mesh D_8015B268[];
extern s8 D_801613A8;
extern s8 D_8017A63C;
extern s8 D_801613C0[][4];
extern s8 D_8012E67C[];
extern s32 D_801174B4;
extern s32 D_8014A110;
extern Color4 D_8011B558[];
extern u8 D_8011B56C[];
extern f32 D_8011AD90[4][3];

void func_8008C074(Mesh *mesh, s32 count, f32 *positions, s32 parameter, u8 *color, s32 flags, s32 indexed);
void func_8008C544(f32 *v, f32 *out, f32 *m);
f32 func_8008C768(f32 y, f32 x);
f32 func_8008B3C8(f32 *v);
