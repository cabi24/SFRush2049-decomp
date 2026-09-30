/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; first pass, NOT a match: 376 vs 386 words, shape 74% */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
float fabsf(float);
#pragma intrinsic (fabsf)
typedef struct {
    u8 pad0[20]; f32 f20, f24, f28; u8 pad32[12];
    f32 f44, f48, f52; u8 pad56[60];
    f32 wheel[4][3];                 /* +116, stride 12 */
    u8 pad164[84];
    s16 h248; u8 pad250[0x3B8 - 250];
} Car;
typedef struct {
    u8 pad0[4]; f32 m[3][3]; f32 f40, f44, f48; s32 w52; u8 pad56[8];
    f32 f64; f32 f68, f72, f76; u32 w80; s16 h84; s16 h86; u8 pad88[4];
} Obj;
extern Car D_80152818[];
extern u32 D_80117498;
extern u32 D_801392D8[];
extern u8 D_8013F1E0[];
extern u16 D_8014295A[];
extern u32 D_8011735C;
extern f32 D_80123950, D_80123954, D_80123958, D_8012395C;
Obj *func_8008E3C0(void *);
void func_8008E0B8(f32 *);
void vector_normalize_length(f32 *, f32 *);
void math_utility(f32 *, f32 *);
s32 func_8008E26C(s32, f32 *, s32, s32);

void func_8008E408(s16 idx, s32 type) {
    Car *car = &D_80152818[idx];
    u32 loc168 = D_80117498;
    s16 sp186 = car->h248 >> 2;
    s16 sp184 = fabsf((f32)sp186);
    f32 dir[3];
    f32 basis[9];
    f32 f2;
    u32 w80;
    Obj *o;
    s32 i, j;
    f32 r;

    switch (type) {
    case 0: if (D_801392D8[idx] & 0x100) return; D_801392D8[idx] |= 0x100; break;
    case 1: if (D_801392D8[idx] & 0x200) return; D_801392D8[idx] |= 0x200; break;
    case 2: if (D_801392D8[idx] & 0x400) return; D_801392D8[idx] |= 0x400; break;
    case 3: if (D_801392D8[idx] & 0x800) return; D_801392D8[idx] |= 0x800; break;
    }
    o = func_8008E3C0(D_8013F1E0);
    if (o == 0) return;
    o->f64 = D_80123950;
    o->h84 = idx;
    o->w80 = w80 = (type << 17) | 0x10;
    dir[0] = car->f20; dir[1] = car->f24; dir[2] = car->f28;
    func_8008E0B8(dir);
    vector_normalize_length(dir, basis);
    math_utility(basis, &o->m[0][0]);
    if (sp184 < 90) {
        if (sp186 >= 0) f2 = (f32)sp184 / 90.0f * D_80123954 + D_80123958;
        else f2 = (f32)sp184 / 70.0f * 0.75f + 0.25f;
        for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) o->m[i][j] *= f2;
    }
    o->f40 = car->wheel[type][0]; o->f44 = car->wheel[type][1]; o->f48 = car->wheel[type][2];
    o->f68 = car->f44; o->f72 = car->f48; o->f76 = car->f52;
    D_8011735C = D_8011735C * 0x41C64E6D + 12345;
    o->h86 = 0;
    r = (f32)((D_8011735C >> 16) & 0x7FFF) * 1.0f / 32768.0f - 0.5f;
    o->f68 *= r; o->f72 *= r; o->f76 *= r;
    o->f40 += o->f68; o->f44 += o->f72; o->f48 += o->f76;
    o->f44 += 1.25f;
    o->w52 = func_8008E26C(D_8014295A[o->h86], &o->m[0][0], -1, 0x40000);

    o = func_8008E3C0(D_8013F1E0);
    if (o == 0) return;
    o->f64 = D_8012395C;
    o->w80 = w80;
    o->h84 = idx;
    math_utility(basis, &o->m[0][0]);
    if (sp184 < 90) {
        for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) o->m[i][j] *= f2;
    }
    o->f40 = car->wheel[type][0]; o->f44 = car->wheel[type][1]; o->f48 = car->wheel[type][2];
    o->f68 = car->f44; o->f72 = car->f48; o->f76 = car->f52;
    D_8011735C = D_8011735C * 0x41C64E6D + 12345;
    o->h86 = 4;
    r = (f32)((D_8011735C >> 16) & 0x7FFF) * 1.0f / 32768.0f - 0.5f;
    o->f68 *= r; o->f72 *= r; o->f76 *= r;
    o->f40 += o->f68; o->f44 += o->f72; o->f48 += o->f76;
    o->f44 += 1.25f;
    o->w52 = func_8008E26C(D_8014295A[o->h86], &o->m[0][0], -1, 0x40000);
}
