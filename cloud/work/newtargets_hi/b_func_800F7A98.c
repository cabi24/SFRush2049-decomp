typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u16 h; u8 pad[94]; } Rec96;
typedef struct { u16 h; u8 pad[62]; } Rec64;
typedef struct { u8 pad[232]; Rec96 a[6]; u8 pad2[456 - 232 - 576 + 0]; } Dummy;
typedef struct { u8 pad0[232]; u16 a_h; } W;
typedef struct { u8 pad[232]; } W0;
typedef struct { u8 pad[232]; Rec96 a[6]; } BigA;
typedef struct { u8 pad[456]; Rec64 b[4]; } BigB;
typedef union { BigA *a; BigB *b; } Rows;
typedef struct { Rows *rows; } Tab;
typedef struct Obj { struct Obj *f0; u8 pad[40]; Tab *f2C; } Obj;
typedef struct { u8 b0; u8 b1; u8 pad[70]; Obj *f72; } Ply;
extern u32 D_801174B4;
extern s8 D_80149B8C;
extern Ply D_8014A118[];
extern Obj *D_80146150[];
s32 func_800F7A98(s32 i, s32 bit) {
    s32 v = D_80149B8C;
    Obj *o;
    Tab *t;
    if (D_801174B4 & 8) return 1;
    v = D_80149B8C;
    if (v >= 0 && v < 6) {
        if (D_8014A118[i].f72 == 0) {
            t = D_80146150[D_8014A118[i].b1]->f2C;
        } else {
            t = D_8014A118[i].f72->f0->f2C;
            if (t == 0) return 1;
        }
        return t->rows->a->a[v].h & (1 << bit);
    }
    return 1;
}
