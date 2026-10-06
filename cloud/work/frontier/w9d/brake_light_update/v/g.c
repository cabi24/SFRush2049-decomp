/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * NOT A MATCH: 74 of 112 words differ (see ../RESULTS.md).
 * Not a brake-light routine (historical label): world -> screen projection for view `idx`.
 * d = pos - view->pos; r = d rotated into the view frame (func_800A61B0); r.z clamped to >= 2.5;
 * screen x = r.x / r.z * port[0x1C] * port[0x24] + port[0x2C], y = port[0x30] - r.y / r.z * port[0x20] * port[0x28],
 * both saturated to s16; optionally returns the view-space vector.
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

void brake_light_update(s32 idx, f32 *pos, View *view, f32 *vout, s16 *sout) {
    struct { f32 r[3]; f32 pad; f32 scr[2]; f32 d[3]; } s;
    Port *p;
    f32 inv;

    s.d[0] = pos[0] - view->pos[0];
    s.d[1] = pos[1] - view->pos[1];
    s.d[2] = pos[2] - view->pos[2];
    func_800A61B0(s.d, s.r, (f32 *) view);
    if (s.r[2] < 2.5f) {
        s.r[2] = 2.5f;
    }
    inv = 1.0f / s.r[2];
    p = &D_8017A510[idx];
    s.scr[0] = s.r[0] * inv * p->unk1C * p->unk24 + p->unk2C;
    s.scr[1] = p->unk30 - s.r[1] * inv * p->unk20 * p->unk28;
    if (s.scr[0] < -32768.0f) {
        sout[0] = -32768;
    } else if (s.scr[0] > 32767.0f) {
        sout[0] = 32767;
    } else {
        sout[0] = s.scr[0];
    }
    if (s.scr[1] < -32768.0f) {
        sout[1] = -32768;
    } else if (s.scr[1] > 32767.0f) {
        sout[1] = 32767;
    } else {
        sout[1] = s.scr[1];
    }
    if (vout != 0) {
        vout[0] = s.r[0];
        vout[1] = s.r[1];
        vout[2] = s.r[2];
    }
}
