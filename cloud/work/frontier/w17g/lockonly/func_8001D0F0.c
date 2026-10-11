/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001D0F0 (static, boot 0x8001D0F0, 208 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 17 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_medium_tail_8001D0F0.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-medium-tail/nonmatch/func_8001D0F0.c (Astra / BT03-high-medium-tail).
 * Shaping devices: none. */
typedef unsigned char u8;
typedef struct Vector { float x, y, z; } Vector;
typedef struct Emitter {
    u8 unknown00[12];
    Vector position;
    Vector velocity;
    unsigned int unknown24;
    float gain28;
    float current2C;
} Emitter;
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
int func_8001D0F0(Emitter *state, const Vector *position, const Vector *velocity, u8 level)
{
    if (D_8002C630) {
        func_80014594();
        state->position = *position;
        state->velocity = *velocity;
        state->gain28 = (float)level / 127.0f;
        if (state->gain28 < state->current2C) state->current2C = state->gain28;
        func_800145DC();
        return 1;
    }
    return 0;
}
