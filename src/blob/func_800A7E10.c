/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also MATCH at -O2) */
/*
 * Append the eight corner vertices of an axis-aligned box (min, max; fixed
 * point x16) to the Vtx pool D_80157248 at the running index D_80156CE0, tag
 * the 0x44-byte record D_8012E700[idx] with (first index << 3) | 7 in its
 * flags word, advance the index by 8 and keep the high-water mark
 * D_80156D30.  No arcade ancestor (N64 display-list side).
 *
 * Shaping fact: the vertex pointer is post-incremented after each of the
 * first seven vertices (IDO folds the increments into one `addiu v0,v0,112`
 * with negative offsets); indexing vtx[0..7] leaves 25 aligned rows.
 */
typedef float f32;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} Vtx_t;

typedef union {
    Vtx_t v;
    long long force_structure_alignment;
} Vtx;

typedef struct {
    u32 unk0;
    u32 flags;
    char pad8[0x3C];
} Entry;

extern s32 D_80156CE0;
extern s32 D_80156D30;
extern Vtx D_80157248[];
extern Entry D_8012E700[];

void func_800A7E10(f32 *min, f32 *max, s16 idx) {
    Vtx *vtx;

    vtx = &D_80157248[D_80156CE0];
    D_8012E700[idx].flags |= (D_80156CE0 << 3) | 7;
    D_80156CE0 += 8;
    if (D_80156D30 < D_80156CE0) {
        D_80156D30 = D_80156CE0;
    }
    vtx->v.ob[0] = min[0] * 16.0f;
    vtx->v.ob[1] = min[1] * 16.0f;
    vtx->v.ob[2] = min[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = min[0] * 16.0f;
    vtx->v.ob[1] = min[1] * 16.0f;
    vtx->v.ob[2] = max[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = min[0] * 16.0f;
    vtx->v.ob[1] = max[1] * 16.0f;
    vtx->v.ob[2] = min[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = min[0] * 16.0f;
    vtx->v.ob[1] = max[1] * 16.0f;
    vtx->v.ob[2] = max[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = max[0] * 16.0f;
    vtx->v.ob[1] = min[1] * 16.0f;
    vtx->v.ob[2] = min[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = max[0] * 16.0f;
    vtx->v.ob[1] = min[1] * 16.0f;
    vtx->v.ob[2] = max[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = max[0] * 16.0f;
    vtx->v.ob[1] = max[1] * 16.0f;
    vtx->v.ob[2] = min[2] * 16.0f;
    vtx++;
    vtx->v.ob[0] = max[0] * 16.0f;
    vtx->v.ob[1] = max[1] * 16.0f;
    vtx->v.ob[2] = max[2] * 16.0f;
}
