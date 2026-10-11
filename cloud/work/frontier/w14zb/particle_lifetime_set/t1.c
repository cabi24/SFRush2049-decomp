typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct { char c[4]; } Name4;
typedef struct {
    s16 x;
    s16 y;
    char pad4[0x10];
} Point; /* 0x14 */
typedef struct {
    s16 count;
    u16 flags;
    char pad4[0x10];
    Point pts[1];
} PointList;
typedef struct {
    char pad0[0x10];
    u16 width;
    u16 height;
} Dims;
typedef struct {
    f32 rect[4];    /* 0x00 */
    f32 dx;         /* 0x10 */
    f32 dy;         /* 0x14 */
    f32 xs[4];      /* 0x18 */
    f32 ys[4];      /* 0x28 */
} Motion;           /* 0x38 */
typedef struct {
    Motion slot[4];
} MotionFrame;      /* 0xE0 */

extern MotionFrame D_80114264[];
extern u32 D_801145D4[][4];
extern PointList *D_80114628[4];
extern Dims *D_80114638;
extern s32 D_8011463C[];
extern Name4 D_8011464C;
extern char D_801202E8[];
extern volatile u8 D_80140BDC;
extern s16 D_80151AD0;
extern s32 D_801391E4;
extern u32 state_word_a;
extern void *D_80114624;
extern char D_8011421C[], D_801141D4[];

Dims *func_800B24EC(char *name, u16 *index, s8 lo, s8 hi, s32 err);
PointList *func_800A78BC(s32 n, f32 *verts, u16 tex, Name4 *name, u32 flags, s32 mode);
void *sound_control(s16 a, s16 b, char *name, s16 mode);

void particle_lifetime_set(void)
{
    s32 i;
    f32 verts[12];
    /*@{*/u16 tex;
    Name4 name;/*@| Name4 name;
    u16 tex; @}*/
    s32 k;
    f32 scale;
    f32 left, top, right, bottom;
    PointList *list;

    D_801391E4 = 1025;

    for (i = 0; i < 4; i++) {
        /*@{*/name = D_8011464C;/*@| @}*/
        /*@{*//*@| if (i > 9) {} @}*/
        D_80114638 = func_800B24EC(D_801202E8, &tex, 0, D_80140BDC - 1, 1);
        /*@{*/scale = D_801391E4;/*@| scale = (f32)D_801391E4; @}*/
        right = D_80114264[D_80151AD0 - 1].slot[i].rect[/*@{*/1/*@| 1 @}*/] * scale;
        bottom = D_80114264[D_80151AD0 - 1].slot[i].rect[/*@{*/3/*@| 3 @}*/] * scale;
        left = D_80114264[D_80151AD0 - 1].slot[i].rect[/*@{*/0/*@| 0 @}*/] * scale;
        top = D_80114264[D_80151AD0 - 1].slot[i].rect[/*@{*/2/*@| 2 @}*/] * scale;
        /*@{*/verts[0] = right;
        verts[1] = bottom;
        verts[2] = scale;
        verts[3] = left;
        verts[4] = bottom;
        verts[5] = scale;
        verts[6] = left;
        verts[7] = top;
        verts[8] = scale;
        verts[9] = right;
        verts[10] = top;
        verts[11] = scale;/*@| verts[0] = right; verts[1] = bottom; verts[2] = scale;
        verts[3] = left; verts[4] = bottom; verts[5] = scale;
        verts[6] = left; verts[7] = top; verts[8] = scale;
        verts[9] = right; verts[10] = top; verts[11] = scale; @}*/
        D_80114628[i] = func_800A78BC(4, verts, tex, &name, /*@{*/D_801145D4[D_80151AD0][i] | 0x1210/*@| 0x1210 | D_801145D4[D_80151AD0][i] @}*/, 1);
        D_80114628[i]->flags &= 0x7FFF;
        if (D_8011463C[D_80151AD0 - 1] == 1) {
            list = D_80114628[i];
            for (k = 0; k < list->count; k++) {
                list->pts[k].x = D_80114264[D_80151AD0 - 1].slot[i].xs[k];
                list->pts[k].y = D_80114264[D_80151AD0 - 1].slot[i].ys[k];
            }
        }
    }
    if (state_word_a & 0x80) {
        D_80114624 = sound_control(0, 0, D_8011421C, 2);
    } else {
        D_80114624 = sound_control(0, 0, D_801141D4, 2);
    }
}
