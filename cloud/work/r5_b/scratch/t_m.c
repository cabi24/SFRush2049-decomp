typedef signed short s16; typedef unsigned int u32; typedef signed int s32; typedef unsigned char u8;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
void f1(a, v) s16 a; s32 v; { D_8012E700[a].flags = D_8012E700[a].flags | (v << 8); }
