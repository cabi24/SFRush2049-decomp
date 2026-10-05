typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    u8 pad8[4];
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 pad3C[4];
} Item; /* 0x40 */
typedef struct {
    Item item[17];
} Menu; /* 0x440 */
typedef struct {
    u8 pad0[0x3C];
    s32 color;           /* 0x3C */
    u8 pad40[4];
} Node;

extern s32 D_801140EC;
extern Menu D_80111998[];
extern char *D_8011196C[];
extern f32 D_8011418C[];
extern volatile u8 D_80140BDC;
extern s16 D_80151AD0;
extern Node D_8012E700[];
extern s32 D_80154618[];
extern s32 D_80154630[];

void math_utility(void *dst, void *src);
s32 string_copy_format(char *name, s8 a, s8 b, s32 c);
s16 func_8008E26C(s32 name, f32 (*mat)[3], s16 parent, s32 flags);
void model_data_load();
void model_transform_setup();
s32 func_800B24EC();
void func_8008D870(s16 handle, s32 tex, s32 c);
void func_800B5898(f32 angle, f32 uv[][3]);
void func_8008B32C(f32 (*dst)[3], f32 (*src)[3], f32 s);
void particle_velocity_set(void);


void drone_throttle_calc(s8 player) {
    s32 color;
    s32 k;
    s16 tex;
    char pad[20];
    s32 i;
    s32 mask;
    s16 none;
    Item *e;
    s32 h;
    f32 scale;

    color = D_801140EC;
    scale = 1.0f;
    none = -1;
    if (player < 0) {}
    for (i = 0; i < 17; i++) {
        e = &D_80111998[player].item[i];
        if (e->handle == none) {
            mask = 1 << player;
            math_utility(D_8011418C, e->mat);
            h = func_8008E26C(string_copy_format(D_8011196C[e->type], 0, (s8)(D_80140BDC - 1), 1), e->mat, none,
                                      (e->type == 0 || e->type == 1 || e->type == 2 || e->type == 9 || e->type == 10) ? 0x42000 : 0);
            e->handle = h;
            D_8012E700[h].color = color;
            model_data_load(h, 1, 15);
            model_transform_setup(e->handle, 0, mask);
        }
    }
    func_8008D870(D_80111998[player].item[13].handle, func_800B24EC("BUTTON_TITLE", &tex, 0, (s8)(D_80140BDC - 1), 1), -1);
    math_utility(D_8011418C, D_80111998[player].item[13].mat);
    func_800B5898(-1.5707964f, D_80111998[player].item[13].mat);
    D_80111998[player].item[13].pos[0] = 0.0f;
    if (D_80151AD0 < 2)
        D_80111998[player].item[13].pos[1] = 115.0f;
    else
        D_80111998[player].item[13].pos[1] = 105.0f;
    D_80111998[player].item[13].pos[2] = 200.0f;
    D_80111998[player].item[13].pos[0] *= scale;
    D_80111998[player].item[13].pos[1] *= scale;
    D_80111998[player].item[13].pos[2] *= scale;
    func_8008D870(D_80111998[player].item[14].handle, func_800B24EC("CURSOR", &tex, 0, (s8)(D_80140BDC - 1), 1), -1);
    func_8008D870(D_80111998[player].item[15].handle, func_800B24EC("B1", &tex, 0, (s8)(D_80140BDC - 1), 1), -1);
    math_utility(D_8011418C, D_80111998[player].item[15].mat);
    func_800B5898(-1.5707964f, D_80111998[player].item[15].mat);
    D_80111998[player].item[15].pos[0] = -75.1f;
    if (D_80151AD0 < 2)
        D_80111998[player].item[15].pos[1] = 55.2f;
    else
        D_80111998[player].item[15].pos[1] = 50.2f;
    D_80111998[player].item[15].pos[2] = 100.0f;
    D_80111998[player].item[15].pos[0] *= scale;
    D_80111998[player].item[15].pos[1] *= scale;
    D_80111998[player].item[15].pos[2] *= scale;
    func_8008B32C(D_80111998[player].item[15].mat, D_80111998[player].item[15].mat, 0.5f);
    math_utility(D_8011418C, D_80111998[player].item[16].mat);
    func_800B5898(-1.5707964f, D_80111998[player].item[16].mat);
    D_80111998[player].item[16].pos[0] = -87.1f;
    if (D_80151AD0 < 2)
        D_80111998[player].item[16].pos[1] = 55.2f;
    else
        D_80111998[player].item[16].pos[1] = 50.2f;
    D_80111998[player].item[16].pos[2] = 100.0f;
    D_80111998[player].item[16].pos[0] *= scale;
    D_80111998[player].item[16].pos[1] *= scale;
    D_80111998[player].item[16].pos[2] *= scale;
    D_80154618[player] = 1;
    D_80154630[player] = 1;
    particle_velocity_set();
}

extern s8 D_zz;
void zz_caller(void) {
    drone_throttle_calc(D_zz);
}
void zz_caller2(void) {
    drone_throttle_calc(D_zz + 1);
}
