/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * exhaust_smoke_effect (historical label) @ 0x800A5744, 452 bytes: viewport projection set-up.
 * Fills the 72-byte view record D_8017A510[idx] from (fovx deg, fovy deg, width, height, near, far):
 * aspect = width / height (height halved when the frame-buffer height D_8002AFC4 > 240),
 * fov in radians (fovy derived from fovx through tanf/atanf when not given; both pi/2 and the
 * flag at +8 set when fovx <= 0), tan(fov/2) and 0.5/tan for both axes. Returns the record.
 * func_800A557C is tanf, func_8008C720 is atanf. No arcade ancestor found (N64 renderer code).
 *
 * State: code identical (113/113 words; EQUAL in the whole-program unit), own-rodata unverified by
 * the unpatched scorer: three literals at 0x80123BC8 (3C8EFA35 3C8EFA35 3FC90FDB), checked by hand
 * against the compiled .rodata (same three words, same order).
 *
 * Shaping facts (each one is needed):
 * - the halvings are written `/ 2.0f` (uopt does not share the 0.5 it turns them into); `* 0.5f`
 *   shares one constant in $f20 and adds an sdc1/ldc1 pair;
 * - the two reciprocals are spelled `0.5f /` and `.5f /`: different spellings are different
 *   constants (retail loads 0.5 twice, `mtc1 at,$f4; mtc1 at,$f8`); the same spelling shares one;
 * - `* 2` (int literal) gives mul.s by 2.0; `* 2.0f` becomes add.s;
 * - `fy = fx = 1.5707964f` (fy is loaded, fx is the copy);
 * - the two degree-to-radian literals stay two rodata words (IDO does not pool them).
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef struct View {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 ortho;
    /* 0x0C */ f32 fovx;
    /* 0x10 */ f32 fovy;
    /* 0x14 */ f32 tanx;
    /* 0x18 */ f32 tany;
    /* 0x1C */ f32 invx;
    /* 0x20 */ f32 invy;
    /* 0x24 */ f32 width;
    /* 0x28 */ f32 height;
    /* 0x2C */ f32 nearz;
    /* 0x30 */ f32 farz;
    /* 0x34 */ f32 aspect;
    /* 0x38 */ s32 pad[4];
} View;

extern View D_8017A510[];
extern s32 D_8002AFC4;
f32 func_800A557C(f32);
f32 func_8008C720(f32);

View *exhaust_smoke_effect(s32 idx, f32 fovx, f32 fovy, f32 width, f32 height, f32 nearz, f32 farz) {
    View *v = &D_8017A510[idx];
    f32 fx;
    f32 fy;
    f32 h;

    v->width = width;
    v->height = height;
    v->nearz = nearz;
    v->farz = farz;
    if (D_8002AFC4 > 240) {
        v->aspect = width / (height / 2.0f);
    } else {
        v->aspect = width / height;
    }
    if (fovx > 0.0f) {
        fx = fovx * 0.017453292f;
        if (fovy > 0.0f) {
            fy = fovy * 0.017453292f;
        } else {
            if (D_8002AFC4 > 240) {
                h = height / 2.0f;
            } else {
                h = height;
            }
            fy = func_8008C720(func_800A557C(fx / 2.0f) * h / width) * 2;
        }
        v->ortho = 0;
    } else {
        fy = fx = 1.5707964f;
        v->ortho = 1;
    }
    v->fovx = fx;
    v->fovy = fy;
    v->tanx = func_800A557C(fx / 2.0f);
    v->tany = func_800A557C(v->fovy / 2.0f);
    v->invy = 0.5f / v->tany;
    v->invx = .5f / v->tanx;
    return v;
}
