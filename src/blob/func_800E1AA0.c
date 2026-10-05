/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E1AA0 (0x800E1AA0, 400 bytes): N64-only car drag/brake term (no arcade ancestor
 * found; 12000.0f = 100 * 120 is an N64 constant).  Subtracts from the car's f320 accumulator:
 *  - unless both tyre-surface words (+1548, +1552) are 8: f1824 * min(f1008,100) * 120 * scale,
 *    scale = min(f1452 + f980 * 0.5, 1); the clamped case is written with the folded 12000.0f
 *    (own .rodata 0x801243C0, verified by the scorer);
 *  - when f72 >= 0: if h1994 != 0 and f64 * f80 > 0, f64 * 100; otherwise, when f64 * f80 > 0
 *    and f980 < 0.5, f64 * 100 * sub->f36.
 * Skipped entirely when flags2004 & 0x10.  Field names are offsets (car record as in
 * src/blob/groups/func_800AD4C8: w1548[4] surface types, flags2004).
 * Shaping: `scale = f1452; scale += f980 * 0.5f;` (two statements) puts f1452 straight into
 * f0 and fixes the whole FP ring; the f72 test is written with int 0 (`f72 < 0`) so that its
 * zero is a separate constant web from the 0.0f of the two product tests (with 0.0f everywhere
 * uopt hoists one shared zero into the join block).  Also matches at -O2.
 */
typedef float f32; typedef int s32; typedef short s16; typedef unsigned char u8;
typedef struct Sub { u8 p0[36]; f32 f36; } Sub;
typedef struct Car {
    u8 p0[4];
    Sub *sub;
    u8 p8[56];
    f32 f64;
    u8 p68[4];
    f32 f72;
    u8 p76[4];
    f32 f80;
    u8 p84[236];
    f32 f320;
    u8 p324[656];
    f32 f980;
    u8 p984[24];
    f32 f1008;
    u8 p1012[440];
    f32 f1452;
    u8 p1456[92];
    s32 w1548;
    s32 w1552;
    u8 p1556[268];
    f32 f1824;
    u8 p1828[166];
    s16 h1994;
    u8 p1996[8];
    s32 flags2004;
} Car;

void func_800E1AA0(Car *m) {
    f32 scale;
    f32 speed;

    if (!(m->flags2004 & 0x10)) {
        if (m->w1548 != 8 || m->w1552 != 8) {
            scale = m->f1452;
            scale += m->f980 * 0.5f;
            if (scale > 1.0f) {
                scale = 1.0f;
            }
            speed = m->f1008;
            if (speed > 100.0f) {
                m->f320 -= m->f1824 * 12000.0f * scale;
            } else {
                m->f320 -= m->f1824 * speed * 120.0f * scale;
            }
        }
        if (!(m->f72 < 0)) {
            if (m->h1994 != 0) {
                if (m->f64 * m->f80 > 0.0f) {
                    m->f320 -= m->f64 * 100.0f;
                }
            } else {
                if (m->f64 * m->f80 > 0.0f && m->f980 < 0.5f) {
                    m->f320 -= m->f64 * 100.0f * m->sub->f36;
                }
            }
        }
    }
}
