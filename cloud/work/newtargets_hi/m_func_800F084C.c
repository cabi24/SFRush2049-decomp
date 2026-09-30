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
/*@1: while (**p != 0) {\n r |= func_800F0674(*p, arg);\n p++;\n } || while (**p != 0) {\n r = r | func_800F0674(*p, arg);\n p++;\n } || while (**p != 0) {\n r = (u8)(r | func_800F0674(*p, arg));\n p++;\n } || for (; **p != 0; p++) {\n r |= func_800F0674(*p, arg);\n } || for (; **p != 0; p++) {\n r = r | func_800F0674(*p, arg);\n } || while (**p) {\n r |= func_800F0674(*p++, arg);\n } || while (**p) {\n r = r | func_800F0674(*p++, arg);\n } */
    return r;
}
