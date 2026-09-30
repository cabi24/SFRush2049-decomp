typedef unsigned char u8;
typedef signed int s32;
typedef struct { u8 *p[22]; } List;
extern List D_801149B4;
u8 func_800F0674(u8 *a, s32 b);
u8 func_800F084C(s32 arg) {
    List l;
    u8 r = 0;
    u8 **p = l.p;
    l = D_801149B4;
    while (**p != 0) {
        r |= func_800F0674(*p, arg);
        p++;
    }
    return r;
}
