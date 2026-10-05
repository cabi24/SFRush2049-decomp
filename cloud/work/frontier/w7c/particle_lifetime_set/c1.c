typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct { char c[4]; } Name4;
typedef struct {
    s16 s;          /* 0 */
    s16 t;          /* 2 */
    u8 pad4[16];
} SprVtx;           /* 20 */
typedef struct {
    s16 count;      /* 0 */
    u16 flags;      /* 2 */
    u8 pad4[16];
    SprVtx v[4];    /* 20 */
} Sprite;
typedef struct {
    f32 rect[4];    /* 0 */
    u8 pad10[8];
    f32 u[4];       /* 24 */
    f32 v[4];       /* 40 */
} QuadDef;          /* 56 */

extern QuadDef D_80114264[][4];
extern u32 D_801145D4[][4];
extern Sprite *D_80114628[4];
extern s32 D_80114638[];
extern Name4 D_8011464C;
extern char D_801202E8[];
extern volatile u8 D_80140BDC;
extern s16 D_80151AD0;
extern s32 D_80138DE4;
extern u32 state_word_a;
extern void *D_80114624;
extern char D_8011421C[], D_801141D4[];

s32 func_800B24EC(char *name, u16 *index, s8 lo, s8 hi, s32 err);
Sprite *func_800A78BC(s32 n, f32 *verts, u16 tex, Name4 *name, u32 flags, s32 mode);
void *sound_control(s16 a, s16 b, char *name, s16 mode);

void particle_lifetime_set(void)
{
    s32 i, k;
    f32 verts[12];
    u16 tex;
    Name4 name;
    f32 scale;
    QuadDef *q;

    D_80138DE4 = 1025;
    for (i = 0; i < 4; i++) {
        name = D_8011464C;
        D_80114638[0] = func_800B24EC(D_801202E8, &tex, 0, D_80140BDC - 1, 1);
        scale = D_80138DE4;
        q = &D_80114264[D_80151AD0 - 1][i];
        verts[0] = q->rect[1] * scale;
        verts[1] = q->rect[3] * scale;
        verts[2] = scale;
        verts[3] = q->rect[0] * scale;
        verts[4] = q->rect[3] * scale;
        verts[5] = scale;
        verts[6] = q->rect[0] * scale;
        verts[7] = q->rect[2] * scale;
        verts[8] = scale;
        verts[9] = q->rect[1] * scale;
        verts[10] = q->rect[2] * scale;
        verts[11] = scale;
        D_80114628[i] = func_800A78BC(4, verts, tex, &name, D_801145D4[D_80151AD0][i] | 0x1210, 1);
        D_80114628[i]->flags &= 0x7FFF;
        if (D_80114638[D_80151AD0] == 1) {
            for (k = 0; k < D_80114628[i]->count; k++) {
                D_80114628[i]->v[k].s = D_80114264[D_80151AD0 - 1][i].u[k];
                D_80114628[i]->v[k].t = D_80114264[D_80151AD0 - 1][i].v[k];
            }
        }
    }
    if (state_word_a & 0x80) {
        D_80114624 = sound_control(0, 0, D_8011421C, 2);
    } else {
        D_80114624 = sound_control(0, 0, D_801141D4, 2);
    }
}
