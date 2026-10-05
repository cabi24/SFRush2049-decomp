/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    struct { f32 l.r[3]; f32 inv; f32 l.scr[2]; f32 l.d[3]; } l;
    f32 inv;
    Port *p;

    l.d[0] = pos[0] - view->pos[0];
    l.d[1] = pos[1] - view->pos[1];
    l.d[2] = pos[2] - view->pos[2];
    func_800A61B0(l.d, l.r, (f32 *) view);
    if (l.r[2] < 2.5f) {
        l.r[2] = 2.5f;
    }
    inv = 1.0f / l.r[2];
    p = &D_8017A510[idx];
    l.scr[0] = l.r[0] * inv * p->unk1C * p->unk24 + p->unk2C;
    l.scr[1] = p->unk30 - l.r[1] * inv * p->unk20 * p->unk28;
    if (l.scr[0] < -32768.0f) {
        sout[0] = -32768;
    } else if (l.scr[0] > 32767.0f) {
        sout[0] = 32767;
    } else {
        sout[0] = l.scr[0];
    }
    if (l.scr[1] < -32768.0f) {
        sout[1] = -32768;
    } else if (l.scr[1] > 32767.0f) {
        sout[1] = 32767;
    } else {
        sout[1] = l.scr[1];
    }
    if (vout != 0) {
        vout[0] = l.r[0];
        vout[1] = l.r[1];
        vout[2] = l.r[2];
    }
}
