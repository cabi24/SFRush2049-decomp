/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8008E26C @ 0x8008E26C, 300 bytes -- NOT a match: 34 of 75 words differ (20 aligned).
 * Allocates a 0x44-byte scene record in D_8012E700[]: first slot whose id (+0x14) is 0xFFFF,
 * else a new one (count D_80156990, high-water D_801569A8); fills it (flags, owner pointer,
 * scale 1.0/1.0, id, child/sibling/third link = -1, nine zero words) and links it under `c`
 * with render_mode_select(index, parent). Returns the index.
 *
 * What this source reproduces that the earlier single-function sources (68/75) did not:
 * the four parameter copies into t5/t4/t3/t2, the dead `sw v1,32(sp)` before the call (the index
 * is an address-taken local written through an inlined static's out parameter), the narrowed
 * index kept in a word slot at sp+24 and returned from it, and the 40-byte frame.
 * Residual: retail computes i*68 into a3 (a coloured register) directly at the loop exit, before
 * the two `if`s, and adds the array base after them; here the whole address is formed after the
 * `if`s in temps, which also shifts the t6-t9 ring for the two sign extensions (see NOTES.md).
 * `find` must be internal: score with --keep func_8008E26C (group) or in the whole-program unit.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { s32 w0; s32 w4; void *w8; f32 f12; f32 f16; u16 h20; s16 h22; s16 h24; s16 h26; s32 w28[9]; s32 w64; } Ent;
extern Ent D_8012E700[];
extern s32 D_80156990;
extern s32 D_801569A8;
void render_mode_select(s16 a, s16 b);

static void find(s32 *p) { s32 i;
    for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }
    if (i == D_80156990) D_80156990++;
    if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;
    *p = i;
}
s32 func_8008E26C(s32 a, void *b, s16 c, s32 d) {
    s32 pad; s32 i; Ent *e; s32 idx;
    find(&i);
    e = &D_8012E700[i];
    e->w0 = d; e->w4 = 0; e->w8 = b; e->f12 = 1.0f; e->f16 = 1.0f; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0;
    idx = (s16)i;
    render_mode_select(i, c);
    return idx;
}
