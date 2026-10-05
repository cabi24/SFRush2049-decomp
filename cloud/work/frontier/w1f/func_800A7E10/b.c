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
