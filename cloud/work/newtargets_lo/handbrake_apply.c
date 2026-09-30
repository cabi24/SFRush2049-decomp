typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { s16 child; s16 pad2; s16 x0; s16 x1; s16 y0; s16 y1; s16 pad[4]; } Node;
extern Node *D_80124EEC;
s32 func_800AC9BC(Node *, s32, s32, s32);
s32 handbrake_apply(Node *p, s16 x, s16 y, s32 a3) {
    s16 outside;
    if (p == 0) {
        p = D_80124EEC;
    }
    while (1) {
        outside = x >= p->x1 || x < p->x0 || y >= p->y1 || y < p->y0;
        if (outside) {
            if (p->child == -1) {
                return 0;
            }
            { s32 c = p->child; p = &D_80124EEC[c]; }
        } else {
            return func_800AC9BC(p, x, y, a3);
        }
    }
}
