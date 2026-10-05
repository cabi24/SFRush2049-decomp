/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * NOT a strict match: score.py prints "MATCH (2 section-relative relocations
 * unverified: .rodata+0x0 at +0x154, .rodata+0x0 at +0x158)". Text words are
 * identical; the one unverified reference is this function's own literal
 * 0.0025f, and retail has 0x3B23D70A (0.0025f) at the referenced address
 * 0x80123E74.
 *
 * Historical label camera_blend_between is misleading: for each of the
 * D_80150F80 - 1 64-byte objects at D_80150F38 it transforms the offset from
 * the car into car space (func_800A61B0) and, if the object is 0..225 ahead
 * and within 14 laterally, lowers *out to 0.75 (closer than 125) or to a ramp
 * 0.75..1.0.
 *
 * Quirks (both are literal spellings, no dead code):
 * - the last `val = 1;` is an int literal: with `1.0f` it joins the hoisted
 *   1.0f web (9 words differ);
 * - 0.0025f must stay a literal: as `extern f32 D_80123E74` IDO hoists the
 *   address into s6 and everything shifts (which is why this cannot be made
 *   strict with the extern-symbol trick used for random_int).
 */
typedef float f32;
typedef int s32;
typedef signed char s8;

typedef struct {
    char pad0[0x794];
    f32 unk794[3];
    f32 unk7A0[3][3];
} Car;

typedef struct {
    char pad0[0x28];
    f32 unk28[3];
    char pad34[0xC];
} Obj;

extern s32 D_80150F80;
extern Obj *D_80150F38;

extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2);

void camera_blend_between(Car *car, f32 *out) {
    Obj *p;
    f32 diff[3];
    f32 loc[3];
    f32 val;

    if (D_80150F80 != 0) {
        for (p = D_80150F38; p < &D_80150F38[D_80150F80 - 1]; p++) {
            diff[0] = p->unk28[0] - car->unk794[0];
            diff[1] = p->unk28[1] - car->unk794[1];
            diff[2] = p->unk28[2] - car->unk794[2];
            func_800A61B0(diff, loc, car->unk7A0[0]);
            if (loc[2] < 0.0f) {
                continue;
            }
            if (loc[2] > 225.0f) {
                continue;
            }
            if (fabsf(loc[0]) > 14.0f) {
                continue;
            }
            if (loc[2] < 125.0f) {
                val = 0.75f;
            } else if (loc[2] < 225.0f) {
                val = 1.0f - (225.0f - loc[2]) * 0.00251f;
            } else {
                val = 1;
            }
            if (val < *out) {
                *out = val;
            }
        }
    }
}
