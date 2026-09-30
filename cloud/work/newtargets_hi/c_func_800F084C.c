typedef unsigned char u8;
typedef signed int s32;
typedef struct { u8 *p[22]; } List;
extern List D_801149B4;
u8 func_800F0674(u8 *a, s32 b);
u8 func_800F084C(s32 arg) {
    u8 r;
    u8 **p;
    List l;
    l = D_801149B4;
    r = 0;
    p = l.p;
    while (**p != 0) {
        r |= func_800F0674(*p, arg);
        p++;
    }
    return r;
}
