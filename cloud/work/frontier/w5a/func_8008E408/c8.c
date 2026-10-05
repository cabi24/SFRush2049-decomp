typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
float fabsf(float);
#pragma intrinsic (fabsf)

typedef struct {
    u8 pad0[20];
    f32 dir[3];                      /* +20 */
    u8 pad32[12];
    f32 vel[3];                      /* +44 */
    u8 pad56[60];
    f32 wheel[4][3];                 /* +116, stride 12 */
    u8 pad164[84];
    s16 speed;                       /* +248 */
    u8 pad250[0x3B8 - 250];
} Car;

typedef struct {
    u8 pad0[4];
    f32 m[3][3];                     /* +4 */
    f32 pos[3];                      /* +40 */
    s32 handle;                      /* +52 */
    u8 pad56[8];
    f32 f64;                         /* +64 */
    f32 vel[3];                      /* +68 */
    u32 flags;                       /* +80 */
    s16 owner;                       /* +84 */
    s16 kind;                        /* +86 */
} Obj;

typedef struct { u8 r, g, b, a; } Color;

extern Car D_80152818[];
extern Color D_80117498;
extern u32 D_801392D8[];
extern u8 D_8013F1E0[];
extern u16 D_8014295A[];
extern s32 D_8011735C;

Obj *func_8008E3C0(void *);
f32 func_8008E0B8(f32 *);
void vector_normalize_length(f32 *, f32 m[3][3]);
void math_utility(void *, void *);
s32 func_8008E26C(s32, void *, s16, s32);
s32 func_8008B2B4(void);
void func_8008B32C(f32 src[3][3], f32 dst[3][3], f32 scale);

static f32 frand(f32 range) {
    return (f32)func_8008B2B4() * range / 32768.0f;
}


void func_8008E408(s16 idx, s32 type) {
    s32 pad240[4];
    f32 dir[3];
    s32 pad220[2];
    f32 scale;
    s32 pad188[7];
    s16 speed;
    s16 aspeed;
    s32 pad176[2];
    Obj *o;
    Color color;
    s32 pad156[3];
    f32 basis[3][3];
    s32 pad56[6];
    s32 padp;
    Car *car;
    s32 i;
    s32 j;
    f32 r;

    color = D_80117498;
    car = &D_80152818[idx];
    speed = car->speed >> 2;
    aspeed = fabsf((f32)speed);

    switch (type) {
    case 0: if (D_801392D8[idx] & 0x100) return; D_801392D8[idx] |= 0x100; break;
    case 1: if (D_801392D8[idx] & 0x200) return; D_801392D8[idx] |= 0x200; break;
    case 2: if (D_801392D8[idx] & 0x400) return; D_801392D8[idx] |= 0x400; break;
    case 3: if (D_801392D8[idx] & 0x800) return; D_801392D8[idx] |= 0x800; break;
    }

    o = func_8008E3C0(D_8013F1E0);
    if (o == 0) return;
    o->f64 = 0.0333333f;
    o->owner = idx;
    o->flags = (type << 17) | 0x10;
    dir[0] = car->dir[0];
    dir[1] = car->dir[1];
    dir[2] = car->dir[2];
    func_8008E0B8(dir);
    vector_normalize_length(dir, basis);
    math_utility(basis, o->m);
    if (aspeed < 90) {
        if (speed < 0) scale = (f32)aspeed / 70.0f * 0.75f + 0.25f;
        else scale = (f32)aspeed / 90.0f * 0.85f + 0.15f;
        for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) o->m[i][j] *= scale;
    }
    o->pos[0] = car->wheel[type][0];
    o->pos[1] = car->wheel[type][1];
    o->pos[2] = car->wheel[type][2];
    o->vel[0] = car->vel[0];
    o->vel[1] = car->vel[1];
    o->vel[2] = car->vel[2];
    r = frand(1.0f) - 0.5f;
    o->kind = 0;
    o->vel[0] *= r; o->vel[1] *= r; o->vel[2] *= r;
    o->pos[0] = o->vel[0] + o->pos[0];
    o->pos[1] = o->vel[1] + o->pos[1];
    o->pos[2] = o->vel[2] + o->pos[2];
    o->pos[1] += 1.25f;
    o->handle = func_8008E26C(D_8014295A[0], o->m, -1, 0x40000);
    if (o == 0) {
    }

    o = func_8008E3C0(D_8013F1E0);
    if (o == 0) return;
    o->f64 = 0.0333333f;
    o->flags = (type << 17) | 0x10;
    o->owner = idx;
    math_utility(basis, o->m);
    if (aspeed < 90) {
        for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) o->m[i][j] *= scale;
    }
    o->pos[0] = car->wheel[type][0];
    o->pos[1] = car->wheel[type][1];
    o->pos[2] = car->wheel[type][2];
    o->vel[0] = car->vel[0];
    o->vel[1] = car->vel[1];
    o->vel[2] = car->vel[2];
    r = frand(1.0f) - 0.5f;
    o->kind = 4;
    o->vel[0] *= r; o->vel[1] *= r; o->vel[2] *= r;
    o->pos[0] = o->vel[0] + o->pos[0];
    o->pos[1] = o->vel[1] + o->pos[1];
    o->pos[2] = o->vel[2] + o->pos[2];
    o->pos[1] += 1.25f;
    o->handle = func_8008E26C(D_8014295A[o->kind], o->m, -1, 0x40000);
}
