/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D660 (static, boot 0x8001D660, 260 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 23 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_larger_8001D660.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-larger/nonmatch/func_8001D660.c (Astra / BT03-high-larger).
 * w17g TU-context edits (codegen-neutral, re-proved): private struct view renamed SpatialState_8001D660 (lib_1cf90.c
 * already defines a SpatialState view for the locked func_8001D5C0); func_8001D5C0 keeps that TU prototype with the
 * TU's SpatialState typedef copied verbatim, and the call casts `state`.
 * Shaping devices: the (SpatialState *) argument cast (type view only, no code). */
typedef unsigned char u8;
typedef struct Vector { float x, y, z; } Vector;
typedef struct SpatialState_8001D660 {
    u8 unknown00[12];
    Vector position;
    Vector velocity;
    Vector forward;
    Vector side;
    Vector up;
    float inverse[12];
    u8 unknown78[12];
    float gain84;
} SpatialState_8001D660;
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
typedef struct SpatialState { unsigned char unknown00[12]; Vector position; unsigned char unknown18[12]; Vector forward; Vector side; Vector up; float inverse[12]; } SpatialState;
extern void func_8001D5C0(SpatialState *);
int func_8001D660(SpatialState_8001D660 *state, const Vector *position, const Vector *velocity,
                  const Vector *forward, const Vector *up, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        state->position = *position;
        state->velocity = *velocity;
        state->forward = *forward;
        state->up.x = -up->x;
        state->up.y = -up->y;
        state->up.z = -up->z;
        func_8001D5C0((SpatialState *)state);
        state->gain84 = (float)level / 127.0f;
        func_800145DC();
        return 1;
    }
    return 0;
}
