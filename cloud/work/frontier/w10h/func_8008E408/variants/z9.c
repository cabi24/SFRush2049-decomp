/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8008E408(idx, type): spawn the two "wheel effect" objects for car idx, wheel `type` (0..3).
 *   - once-per-wheel latch: bit 0x100<<type in D_801392D8[idx]; returns if already set.
 *   - two objects from the pool D_8013F1E0 (func_8008E3C0): f64 = 0.0333333f, h84 = idx,
 *     w80 = (type << 17) | 0x10; orientation = basis(normalize(car->dir)) (func_8008E0B8,
 *     vector_normalize_length, math_utility copy), scaled when |speed| < 90
 *     (speed = car->h248 >> 2; reverse: |s|/70*0.75+0.25, forward: |s|/90*0.85+0.15);
 *     pos = car->wheel[type], vel = car->vel * (Random(1.0f) - 0.5f), pos += vel, pos.y += 1.25;
 *     w52 = func_8008E26C(D_8014295A[h86], &o->m, -1, 0x40000) with h86 = 0 / 4.
 * No arcade ancestor identified.
 * STATE: NOT A MATCH. blob_unit: 22 of 386 words differ, all of them sp offsets of the six
 * ugen spill temps (retail 28/40/44/48, here 24/36/40/44). Registers, schedule, locals
 * and frame (256) are identical. Shaping found (see notes.md):
 *   - Random() = func_8008B2E4(1.0f) (inlined; keeps the *1.0f multiply);
 *   - `snap = D_80117498` + compiled-out check before the second alloc = retail's dead sw 168(sp);
 *   - `if (type > 3) DEBUG_PRINT` before the switch fixes car t4 / seed t3 colouring;
 *   - u32 type stops the switch's `case 3` sharing the loop-bound register (li at,3);
 *   - pos = pos + vel in order 0,1,2; first lookup is D_8014295A[0], second D_8014295A[o->h86];
 *   - pA..pF padding locals reproduce every retail local offset (dir 228, scale 216,
 *     speed 186, aspeed 184, o 172, snap 168, basis 120): retail has ~22 more named slots.
 */
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

f32 func_8008B2E4(f32 max);

void func_8008E408(s16 idx, u32 type) {
    Car *car;
    s32 flags;
    s32 i, j;
    f32 dir[3];
    f32 r;
    s32 pB1;
    f32 scale;
    s32 pC1, pC2, pC3, pC4, pC5, pC6, pC7;
    s16 speed;
    s16 aspeed;
    s32 pD1, pD2;
    Obj *o;
    s32 snap;
    s32 pE1, pE2, pE3;
    f32 basis[3][3];
    s32 pF0;
    s32 pF1;
    s32 pF2;
    s32 pF3;
    s32 pF4;
    s32 pF5;
    s32 pF6;
    s32 pF7;
    s32 pF8;

    car = &D_80152818[idx];
    if (car->h248 == 0) DEBUG_PRINT(("c"));
    snap = D_80117498;
    speed = car->h248 >> 2;
    aspeed = fabsf((f32)speed);

    if (type > 3) DEBUG_PRINT(("t"));
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
    r = func_8008B2E4(1.0f) - 0.5f;
    o->h86 = 0;
    o->vel[0] *= r;
    o->vel[1] *= r;
    o->vel[2] *= r;
    o->pos[0] = o->pos[0] + o->vel[0];
    o->pos[1] = o->pos[1] + o->vel[1];
    o->pos[2] = o->pos[2] + o->vel[2];
    o->pos[1] += 1.25f;
    o->w52 = func_8008E26C(D_8014295A[0], o->m, -1, 0x40000);

    if (snap != D_80117498) DEBUG_PRINT(("x"));
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
    r = func_8008B2E4(1.0f) - 0.5f;
    o->h86 = 4;
    o->vel[0] *= r;
    o->vel[1] *= r;
    o->vel[2] *= r;
    o->pos[0] = o->pos[0] + o->vel[0];
    o->pos[1] = o->pos[1] + o->vel[1];
    o->pos[2] = o->pos[2] + o->vel[2];
    o->pos[1] += 1.25f;
    o->w52 = func_8008E26C(D_8014295A[o->h86], o->m, -1, 0x40000);
}
