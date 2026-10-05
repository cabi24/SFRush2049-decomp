/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
            scale = m->f1452 + m->f980 * 0.5f;
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
                if (m->f64 * m->f80 > 0) {
                    m->f320 -= m->f64 * 100.0f;
                }
            } else {
                if (m->f64 * m->f80 > 0 && m->f980 < 0.5f) {
                    m->f320 -= m->f64 * 100.0f * m->sub->f36;
                }
            }
        }
    }
}
