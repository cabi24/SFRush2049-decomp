typedef unsigned char u8;
typedef signed int s32;
typedef struct { u8 *p[22]; } List;
extern List D_801149B4;
u8 func_800F0674(u8 *a, s32 b);
u8 func_800F084C(s32 arg) {
    List l;
    u8 r;
    s32 i;
    l = D_801149B4;
    r = 0;
    i = 0;
    while (*l.p[i] != 0) {
        r |= func_800F0674(l.p[i], arg);
        i++;
    }
    return r;
}
