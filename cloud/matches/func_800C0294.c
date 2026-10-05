/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Translate packed vertices: for every 0x20-byte record in the table at
 * *0x801525EC (count u16 0x8015267C) whose u16 id (+0) equals `id`, unpack
 * the 8-byte vertex D_8015201C[rec->vtx] (three s16 integer parts plus a
 * u16 holding three 5-bit fractions, 1/32 units), add off[0..2], round to
 * nearest 1/32 and pack it again.  Only caller: camera_track_spline.
 * No arcade ancestor (N64 vertex format).
 *
 * Needs -O3 (-O2 saves s0 and uses a 40-byte frame).
 * Quirks the match depends on:
 *  - `n` counts the matches and is never returned; the trailing
 *    `if (n == 0) return;` (most likely a compiled-out diagnostic) is what
 *    keeps the counter alive and puts it in v0.  `return n;` instead puts
 *    the record pointer in v0 and adds a `move v0,v1`.
 *  - ipos[4]/pos[4] (not [3]) give the retail slots sp+16 / sp+0 in the
 *    32-byte frame; element 3 of each is unused.
 *  - `pos[k] = pos[k] + off[k]` (not `+=`, not `off[k] + pos[k]`) gives the
 *    retail add operand order and the reload of pos[0] before rounding.
 *  - the fraction word is OR-ed in the order x, y, z.
 */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ char pad02[0x14];
    /* 0x16 */ u16 vtx;
    /* 0x18 */ char pad18[8];
} Rec20;

typedef struct {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u16 frac;
} PVtx;

extern u16 D_8015267C;
extern Rec20 *D_801525EC;
extern PVtx *D_8015201C;

void func_800C0294(s32 id, f32 *off) {
    s32 ipos[4];
    f32 pos[4];
    Rec20 *r;
    PVtx *v;
    s32 n;
    u32 i;

    n = 0;
    r = D_801525EC;
    for (i = 0; i < D_8015267C; i++, r++) {
        if (id == r->id) {
            n++;
            v = &D_8015201C[r->vtx];
            pos[0] = ((v->pos[0] << 5) + ((v->frac & 0x7C00) >> 10)) * 0.03125f;
            pos[1] = ((v->pos[1] << 5) + ((v->frac & 0x3E0) >> 5)) * 0.03125f;
            pos[2] = ((v->pos[2] << 5) + (v->frac & 0x1F)) * 0.03125f;
            pos[0] = pos[0] + off[0];
            pos[1] = pos[1] + off[1];
            pos[2] = pos[2] + off[2];
            if (pos[0] * 32.0f < 0.0f) {
                ipos[0] = pos[0] * 32.0f - 0.5f;
            } else {
                ipos[0] = pos[0] * 32.0f + 0.5f;
            }
            if (pos[1] * 32.0f < 0.0f) {
                ipos[1] = pos[1] * 32.0f - 0.5f;
            } else {
                ipos[1] = pos[1] * 32.0f + 0.5f;
            }
            if (pos[2] * 32.0f < 0.0f) {
                ipos[2] = pos[2] * 32.0f - 0.5f;
            } else {
                ipos[2] = pos[2] * 32.0f + 0.5f;
            }
            v->pos[0] = ipos[0] >> 5;
            v->pos[1] = ipos[1] >> 5;
            v->pos[2] = ipos[2] >> 5;
            v->frac = ((ipos[0] << 10) & 0x7C00) | ((ipos[1] << 5) & 0x3E0) | (ipos[2] & 0x1F);
        }
    }
    if (n == 0) {
        return;
    }
}
