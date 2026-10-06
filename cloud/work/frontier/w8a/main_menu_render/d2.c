typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { u8 model; u8 pad1[67]; void *node; u8 pad48[4]; } CarRec;   /* 76 */
typedef struct { u8 pad0[556]; f32 pos[3]; u8 pad238[2056 - 568]; } ModelDat; /* 0x808 */
typedef struct { u8 pad0[12]; f32 a[3]; f32 b[3]; u8 pad24[152 - 36]; } View;  /* 152 */

extern s16 D_8014A108;
extern CarRec D_8014A118[];
extern ModelDat D_8014A250[];
extern f32 D_801141B0[];
extern View D_80150B70[];

void main_menu_input(void *node, f32 *a, f32 *b, f32 *c, f32 *d);

void main_menu_render(void)
{
    s32 i;
    void *node;

    for (i = 0; i < D_8014A108; i++) {
        if ((node = D_8014A118[i].node) != (void *) -1) {
            main_menu_input(node, D_8014A250[D_8014A118[i].model].pos, D_801141B0,
                            D_80150B70[i].b, D_80150B70[i].a);
        }
    }
}
