/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Historical label only. Per-view fog/clip setup: when a weather/fog mode
 * D_8013F1D8 is active, view `index` (72-byte records at D_8017A510) gets the
 * per-track defaults (near clip D_8011E810, far clip 1000, fog distance
 * D_8011E7F0, global D_80151AA0 from D_8011E7A8, all [track D_8014A108][mode]).
 * Then the nearest fog region of the current track (D_8011E76C[D_8014978C],
 * 12-byte records {x, z, radius, near, far, fog}, terminated by radius 0)
 * that contains the camera position blends near/far/fog towards the region's
 * values by distance / radius. No arcade ancestor identified.
 *
 * Shaping quirks:
 *  - the view pointer is built through an integer cast
 *    ((s32) D_8017A510 + index * 72). Retail reloads D_8013F1D8 and D_8014A108
 *    after every store to the view, which a plain &D_8017A510[index] does not
 *    do (uopt then knows the stores cannot alias those globals; 228 words
 *    differ). The original was probably a pointer whose base uopt could not
 *    see; the exact spelling is not known.
 *  - typed struct fields, not byte-offset macros: with *(T *)((u8 *)p + off)
 *    accesses cfe orders the lerp operands differently (36 words, FP temps);
 *  - `nearest = region;` before `closest = dist;`.
 * -O3 only (at -O2 the position argument is homed: 99/231 words differ).
 * Prior attempt: cloud/work/ipa-groups/codex_init_region_countdown_a160
 * (187/231 there because it was built as an internal group member; alone
 * the same source is 38 words off).
 */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s16 x;
    s16 z;
    s16 radius;
    s16 near_clip;
    s16 far_clip;
    s16 fog;
} Region; /* 0xC */

typedef struct {
    char pad0[0x3C];
    f32 fog;        /* 0x3C */
    u16 near_clip;  /* 0x40 */
    u16 far_clip;   /* 0x42 */
    char pad44[4];
} View; /* 0x48 */

extern s8 D_8013F1D8;
extern s8 D_8014978C;
extern s16 D_8014A108;
extern s16 D_8011E810[][4];
extern s16 D_8011E7F0[][4];
extern f32 D_8011E7A8[][4];
extern f32 D_80151AA0;
extern Region *D_8011E76C[];
extern View D_8017A510[];

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_800AB18C(s32 index, f32 *pos) {
    View *view;
    Region *region;
    Region *nearest;
    s16 dist;
    s16 closest;
    f32 dx;
    f32 dz;
    f32 ratio;

    if (D_8013F1D8) {
        view = (View *) ((s32) D_8017A510 + index * 72);
        view->near_clip = D_8011E810[D_8014A108][D_8013F1D8];
        view->far_clip = 1000;
        view->fog = D_8011E7F0[D_8014A108][D_8013F1D8];
        D_80151AA0 = D_8011E7A8[D_8014A108][D_8013F1D8];
        region = D_8011E76C[D_8014978C];
        if (region) {
            nearest = 0;
            closest = 32767;
            while (region->radius) {
                dx = region->x - pos[0];
                dz = region->z - pos[2];
                dist = sqrtf(dx * dx + dz * dz);
                if (dist < region->radius && dist < closest) {
                    nearest = region;
                    closest = dist;
                }
                region++;
            }
            if (nearest) {
                if (nearest->radius <= view->fog) {
                    ratio = (f32) closest / nearest->radius;
                } else {
                    if (closest < view->fog - nearest->fog) {
                        ratio = closest / (view->fog - nearest->fog);
                    } else {
                        return;
                    }
                }
                view->near_clip = nearest->near_clip + ratio * (view->near_clip - nearest->near_clip);
                view->far_clip = nearest->far_clip + ratio * (view->far_clip - nearest->far_clip);
                if (view->near_clip > view->far_clip - 4) {
                    view->near_clip = view->far_clip - 4;
                }
                view->fog = nearest->fog + ratio * (view->fog - nearest->fog);
                D_80151AA0 = nearest->fog + ratio * (D_80151AA0 - nearest->fog);
            }
        }
    }
}
