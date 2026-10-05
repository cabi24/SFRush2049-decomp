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
    f32 d[3];
    f32 scr[2];
    f32 inv;
    f32 r[3];
    Port *p;
    s32 i;

    d[0] = pos[0] - view->pos[0];
    d[1] = pos[1] - view->pos[1];
    d[2] = pos[2] - view->pos[2];
    func_800A61B0(d, r, (f32 *) view);
    if (r[2] < 2.5f) {
        r[2] = 2.5f;
    }
    inv = 1.0f / r[2];
    p = &D_8017A510[idx];
    scr[0] = r[0] * inv * p->unk1C * p->unk24 + p->unk2C;
    scr[1] = p->unk30 - r[1] * inv * p->unk20 * p->unk28;
    for (i = 0; i < 2; i++) {
        if (scr[i] < -32768.0f) {
            sout[i] = -32768;
        } else if (scr[i] > 32767.0f) {
            sout[i] = 32767;
        } else {
            sout[i] = scr[i];
        }
    }
    if (vout != 0) {
        for (i = 0; i < 3; i++) {
            vout[i] = r[i];
        }
    }
}
