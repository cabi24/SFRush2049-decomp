/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
float fabsf(float);
#pragma intrinsic (fabsf)

typedef struct Car {
    u8 pad0[20];
    f32 dir[3];          /* +0x14 */
    u8 pad32[12];
    f32 vel[3];          /* +0x2C */
    u8 pad56[60];
    f32 wheel[4][3];     /* +0x74 */
    u8 pad164[84];
    s16 h248;            /* +0xF8 */
    u8 pad250[0x3B8 - 250];
} Car;

typedef struct Obj {
    u8 pad0[4];
    f32 m[3][3];         /* +0x04 */
    f32 pos[3];          /* +0x28 */
    s32 w52;             /* +0x34 */
    u8 pad56[8];
    f32 f64;             /* +0x40 */
    f32 vel[3];          /* +0x44 */
    s32 w80;             /* +0x50 */
    s16 h84;             /* +0x54 */
    s16 h86;             /* +0x56 */
} Obj;

extern Car D_80152818[];
extern s32 D_80117498;
extern u32 D_801392D8[];
extern u8 D_8013F1E0[];
extern u16 D_8014295A[];
extern int D_8011735C;
#define DEBUG_PRINT(args)

Obj *func_8008E3C0(void *pool);
f32 func_8008E0B8(f32 *v);
void vector_normalize_length(f32 *dir, f32 m[3][3]);
void math_utility(void *src, void *dst);
s32 func_8008E26C(s32 a, void *b, s16 parent, s32 d);

int func_8008B2B4(void);

void func_8008E408(s16 idx, s32 type) {
    Car *car;
    s16 speed;
    s16 aspeed;
    s32 snap;
    Obj *o;
    f32 basis[3][3];
    f32 scale;
    s32 flags;
    f32 dir[3];
    s32 i, j;
    f32 r;

    car = &D_80152818[idx];
    snap = D_80117498;
    speed = car->h248 >> 2;
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
    o->h84 = idx;
    o->w80 = flags = (type << 17) | 0x10;
    dir[0] = car->dir[0];
    dir[1] = car->dir[1];
    dir[2] = car->dir[2];
    func_8008E0B8(dir);
    vector_normalize_length(dir, basis);
    math_utility(basis, o->m);
    if (aspeed < 90) {
        if (speed < 0) {
            scale = (f32)aspeed / 70.0f * 0.75f + 0.25f;
        } else {
            scale = (f32)aspeed / 90.0f * 0.85f + 0.15f;
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                o->m[i][j] *= scale;
            }
        }
    }
    o->pos[0] = car->wheel[type][0];
    o->pos[1] = car->wheel[type][1];
    o->pos[2] = car->wheel[type][2];
    o->vel[0] = car->vel[0];
    o->vel[1] = car->vel[1];
    o->vel[2] = car->vel[2];
    r = (f32)func_8008B2B4() * 1.0f / 32768.0f - 0.5f;
    o->h86 = 0;
    o->vel[0] *= r;
    o->vel[1] *= r;
    o->vel[2] *= r;
    o->pos[0] += o->vel[0];
    o->pos[1] += o->vel[1];
    o->pos[2] += o->vel[2];
    o->pos[1] += 1.25f;
    o->w52 = func_8008E26C(D_8014295A[o->h86], o->m, -1, 0x40000);

    o = func_8008E3C0(D_8013F1E0);
    if (o == 0) return;
    o->f64 = 0.0333333f;
    o->w80 = flags;
    o->h84 = idx;
    math_utility(basis, o->m);
    if (aspeed < 90) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                o->m[i][j] *= scale;
            }
        }
    }
    o->pos[0] = car->wheel[type][0];
    o->pos[1] = car->wheel[type][1];
    o->pos[2] = car->wheel[type][2];
    o->vel[0] = car->vel[0];
    o->vel[1] = car->vel[1];
    o->vel[2] = car->vel[2];
    r = (f32)func_8008B2B4() * 1.0f / 32768.0f - 0.5f;
    o->h86 = 4;
    o->vel[0] *= r;
    o->vel[1] *= r;
    o->vel[2] *= r;
    o->pos[0] += o->vel[0];
    o->pos[1] += o->vel[1];
    o->pos[2] += o->vel[2];
    o->pos[1] += 1.25f;
    o->w52 = func_8008E26C(D_8014295A[o->h86], o->m, -1, 0x40000);
    if (snap != D_80117498) DEBUG_PRINT(("x"));
}
