typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u8 p[232]; u16 h232; } Row96;
typedef struct { u8 p[232]; u16 h232; u8 q[222]; u16 h456; } Row64;
typedef struct { Row96 *rows; } Tab;
typedef struct { u8 pad[44]; Tab *t; } Inner;
typedef struct Obj { struct Obj *f0; u8 pad[40]; Tab *f2C; } Obj;
typedef struct { u8 b0; u8 b1; u8 pad[70]; Obj *f72; } Ply;
extern u32 D_801174B4;
extern s8 D_80149B8C;
extern Ply D_8014A118[];
extern Obj *D_80146150[];
s32 func_800F7A98(s32 i, s32 bit) {
    s32 v;
Ply *p; 
    Obj *o;
    Tab *t;
    if (D_801174B4 & 8) return 1;
    v = D_80149B8C;
    if (v >= 0 && v < 6) {
p = &D_8014A118[i]; 
        if (p->f72 == 0) {
            o = D_80146150[p->b1];
            return *(u16 *)((u8 *)o->f2C->rows + v * 96 + 232) & (1 << bit);
        }
        t = p->f72->f0->f2C;
        if (t == 0) return 1;
        return *(u16 *)((u8 *)t->rows + v * 96 + 232) & (1 << bit);
    }
    if (v >= 14 && v < 18) {
        p = &D_8014A118[i];
        if (p->f72 == 0) {
            o = D_80146150[p->b1];
            return *(u16 *)((u8 *)o->f2C->rows + v * 64 + 456) & (1 << bit);
        }
        t = p->f72->f0->f2C;
        return *(u16 *)((u8 *)t->rows + v * 64 + 456) & (1 << bit);
    }
    return 1;
}
