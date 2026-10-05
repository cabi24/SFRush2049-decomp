/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { s32 w0; s32 w4; void *w8; f32 f12; f32 f16; u16 h20; s16 h22; s16 h24; s16 h26; s32 w28[9]; s32 w64; } Ent;
extern Ent D_8012E700[];
extern s32 D_80156990;
extern s32 D_801569A8;
void render_mode_select(s16 a, s16 b);

static s32 find(s32 *p) { s32 i;
    for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }
    *p = i;
    return i;
}
s16 func_8008E26C(s32 a, void *b, s16 c, s32 d) {
    Ent *e; s32 i; s32 n;
    i = find(&n);
    if (i == D_80156990) D_80156990++;
    if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;
    e = &D_8012E700[i];
    e->w0 = d; e->w4 = 0; e->w8 = b; e->f12 = 1.0f; e->f16 = 1.0f; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0;
    render_mode_select(i, c);
    return i;
}
