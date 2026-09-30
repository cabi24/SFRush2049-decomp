typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 flag; u8 pad1[3]; s32 count; s32 size; void *mem; s32 f16; s32 free; } Pool;
extern Pool D_8013F1E0;
extern u8 D_8013C378[];
extern s32 D_801392D8[];
extern s32 D_801392F0[];
extern s8 D_80156994;
extern s8 D_8014978C;
extern u8 *D_80117480[];
extern s16 D_8013F380[];
extern s16 D_8013F38C[];
extern u8 D_80140BDC;
void pool_linked_list_init(Pool *);
u32 func_800B24EC(u8 *, s16 *, s32, s32, s32);
void physics_collision_test(void) {
    u8 **t;
    s16 *q;
    s32 *p;
    D_8013F1E0.mem = D_8013C378;
    D_8013F1E0.count = 100;
    D_8013F1E0.size = 88;
    D_8013F1E0.flag = 0;
    pool_linked_list_init(&D_8013F1E0);
    p = D_801392D8;
    do {
        *p++ = 0;
    } while (p < D_801392F0);
    if (D_80156994 != 0 || D_8014978C >= 6) {
        t = D_80117480;
        q = D_8013F380;
        do {
            func_800B24EC(*t, q, 0, (s8)(D_80140BDC - 1), 1);
            q++;
            t++;
        } while (q != D_8013F38C);
    }
}
