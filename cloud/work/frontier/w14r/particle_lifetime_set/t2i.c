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
    s32 idx;
    /*@{DO*//*@| s32 k; @}*/
    f32 verts[12];
    u16 tex;
    Name4 name;
    /*@{DO*/s32 k;/*@| @}*/
    f32 scale;
    f32 left, top, right, bottom;
    PointList *list;
    /*@{LP*//*@| PointList *p; @}*/

    D_801391E4 = 1025;
    /*@{LN*//*@| name = D_8011464C; @}*/
    for (i = 0; i < 4; i++) {
        /*@{LN*/name = D_8011464C;/*@| @}*/
        idx = D_80151AD0 - 1;
        D_80114638 = func_800B24EC(D_801202E8, &tex, 0, D_80140BDC - 1, 1);
        /*@{LSC*/scale = D_801391E4;/*@| @}*/
        /*@{LR*/right = D_80114264[idx].slot[i].rect[1] * scale;/*@| @}*/
        /*@{LB*/bottom = D_80114264[idx].slot[i].rect[3] * scale;/*@| @}*/
        /*@{LLF*/left = D_80114264[idx].slot[i].rect[0] * scale;/*@| @}*/
        /*@{LT*/top = D_80114264[idx].slot[i].rect[2] * scale;/*@| @}*/
        verts[0] = /*@{LR*/right/*@| D_80114264[idx].slot[i].rect[1] * scale @}*/;
        verts[1] = /*@{LB*/bottom/*@| D_80114264[idx].slot[i].rect[3] * scale @}*/;
        verts[2] = /*@{LS*/scale/*@| D_801391E4 @}*/;
        verts[3] = /*@{LLF*/left/*@| D_80114264[idx].slot[i].rect[0] * scale @}*/;
        verts[4] = /*@{LB*/bottom/*@| D_80114264[idx].slot[i].rect[3] * scale @}*/;
        verts[5] = /*@{LS*/scale/*@| D_801391E4 @}*/;
        verts[6] = /*@{LLF*/left/*@| D_80114264[idx].slot[i].rect[0] * scale @}*/;
        verts[7] = /*@{LT*/top/*@| D_80114264[idx].slot[i].rect[2] * scale @}*/;
        verts[8] = /*@{LS*/scale/*@| D_801391E4 @}*/;
        verts[9] = /*@{LR*/right/*@| D_80114264[idx].slot[i].rect[1] * scale @}*/;
        verts[10] = /*@{LT*/top/*@| D_80114264[idx].slot[i].rect[2] * scale @}*/;
        verts[11] = /*@{LS*/scale/*@| D_801391E4 @}*/;
        D_80114628[i] = func_800A78BC(4, verts, tex, &name, D_801145D4[D_80151AD0][i] | 0x1210, 1);
        /*@{LP*/D_80114628[i]->flags &= 0x7FFF;/*@| p = D_80114628[i]; p->flags &= 0x7FFF; @}*/
        if (D_8011463C[idx] == 1) {
            /*@{LLS*/list = D_80114628[i];/*@| @}*/
            for (k = 0; k < /*@{LLS*/list/*@| D_80114628[i] @}*/->count; k++) {
                /*@{LLS*/list/*@| D_80114628[i] @}*/->pts[k].x = D_80114264[idx].slot[i].xs[k];
                /*@{LLS*/list/*@| D_80114628[i] @}*/->pts[k].y = D_80114264[idx].slot[i].ys[k];
            }
        }
    }
    if (state_word_a & 0x80) {
        D_80114624 = sound_control(0, 0, D_8011421C, 2);
    } else {
        D_80114624 = sound_control(0, 0, D_801141D4, 2);
    }
}
