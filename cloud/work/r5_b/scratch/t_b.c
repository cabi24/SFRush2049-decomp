typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { s32 w0; s32 w4; s32 w8; f32 f12; f32 f16; u16 h20; s16 h22; s16 h24; s16 h26; s32 w28[9]; } Ent;
extern Ent D_8012E700[];
extern s32 D_80156990;
extern s32 D_801569A8;
void render_mode_select(s16 a, s16 b);
s16 func_8008E26C(u16 a, s32 b, s32 c, s32 d) {
    s16 i;
    for (i = 0; i < D_80156990; i++) {
        if (D_8012E700[i].h20 == 0xFFFF) break;
    }
    if (i == D_80156990) D_80156990++;
    if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;
    D_8012E700[i].w0 = d;
    D_8012E700[i].w4 = 0;
    D_8012E700[i].w8 = b;
    D_8012E700[i].f12 = 1.0f;
    D_8012E700[i].f16 = 1.0f;
    D_8012E700[i].h20 = a;
    D_8012E700[i].h22 = -1;
    D_8012E700[i].h24 = -1;
    D_8012E700[i].h26 = -1;
    D_8012E700[i].w28[0] = 0; D_8012E700[i].w28[1] = 0; D_8012E700[i].w28[2] = 0;
    D_8012E700[i].w28[3] = 0; D_8012E700[i].w28[4] = 0; D_8012E700[i].w28[5] = 0;
    D_8012E700[i].w28[6] = 0; D_8012E700[i].w28[7] = 0; D_8012E700[i].w28[8] = 0;
    render_mode_select(i, c);
    return i;
}
