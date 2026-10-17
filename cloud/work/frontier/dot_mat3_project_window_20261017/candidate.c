/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* ZOID_ProjectWindow (0x800A6244, 448 bytes).
 * N64 world-to-screen projection. The real window getter is inlined by stock
 * IDO; this genuine call boundary produces the native frame without padding.
 * Keep the N64 2.5f depth clamp and projection scale, which differ from MAT3.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ f32 uvs[3][3];
    /* 0x24 */ f32 pos[3];
} View;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0x14];
} Port; /* 0x48 */

extern Port D_8017A510[];

void func_800A61B0(f32 *v, f32 *out, f32 *m);

void *func_800A6078(s32 idx);

void brake_light_update(s32 idx, f32 *pos, View *view, f32 *vout, s16 *sout) {
    f32 d[3];
    f32 scr[2];
    f32 inv;
    f32 r[3];
    Port *p;

    d[0] = pos[0] - view->pos[0];
    d[1] = pos[1] - view->pos[1];
    d[2] = pos[2] - view->pos[2];
    func_800A61B0(d, r, (f32 *) view);
    if (r[2] < 2.5f) {
        r[2] = 2.5f;
    }
    p = func_800A6078(idx);
    inv = 1.0f / r[2];
    scr[0] = r[0] * inv * p->unk1C * p->unk24 + p->unk2C;
    scr[1] = p->unk30 - r[1] * inv * p->unk20 * p->unk28;
    if (scr[0] < -32768.0f) {
        sout[0] = -32768;
    } else if (scr[0] > 32767.0f) {
        sout[0] = 32767;
    } else {
        sout[0] = scr[0];
    }
    if (scr[1] < -32768.0f) {
        sout[1] = -32768;
    } else if (scr[1] > 32767.0f) {
        sout[1] = 32767;
    } else {
        sout[1] = scr[1];
    }
    if (vout != 0) {
        vout[0] = r[0];
        vout[1] = r[1];
        vout[2] = r[2];
    }
}
