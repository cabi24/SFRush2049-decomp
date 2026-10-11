/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022CD4 - lib_22300 (IDO 7.1). MusyX macro command "set pitch" (DoSetPitch, GameCube MusyX
 * synthmacros.c; doldecomp/ttyd libs/musyx, read as reference only): convert the commanded
 * frequency (command word0 bits 8..23) against the sample's base frequency (state +0x5C low
 * 24 bits; key in the top byte) into a note (+0x50, stored as an unaligned u16) and a detune
 * in cents (+0xC0): the octave count `no` from the 4.12 ratio, then the semitone `i` by scanning
 * the 13-entry 2^(n/12) table kf down from 11; the downward branch subtracts (N64 variant: no
 * key clamp, negated detune). Sets bit 0x100 of +0x24 and returns 0.
 * kf is the function-local static table (MusyX `static u16 kf[13]`): the object's .data (0x20 B,
 * kf + 6 zero bytes of section padding) equals ROM 0x8002D460..0x8002D47F; the only objdump-dr
 * rows that differ from the inventory target are the four kf relocations (.data+0x16 here, the
 * splat label D_8002D476 = &kf[11] there). Installing it needs an OWNED .data slot for lib_22300
 * (w21a/RESULTS.md: data.bin offset 0x5c90, size 0x20, owner func_80022CD4).
 * Loops: `for (i = 11; i >= 0; i--)` with an unsigned i (the condition is always true and
 * compiles to nothing) gives retail's test-at-top scan in both copies; `for (i = 11;; i--)` lets
 * uopt lay the first copy's exit inline (true=500). uopt strength-reduces kf[i] to the a3 cursor.
 * Scores the same with and without -Wab,-r4300_mul (the ROM-TU build passes it). Private PitchState/PitchCommand views (suffixed names; offsets only). No shaping
 * devices, no unused locals, no pad arrays, no false prototypes. */
#include "types.h"
#pragma pack(1)
typedef struct PitchState_80022CD4 {
    u8 unknown00[0x24];
    u32 flags24;
    u8 unknown28[0x28];
    u16 note50;
    u8 unknown52[0xA];
    u32 sample5C;
    u8 unknown60[0x60];
    s8 detuneC0;
} PitchState_80022CD4;
#pragma pack(0)
typedef struct PitchCommand_80022CD4 { u32 word[2]; } PitchCommand_80022CD4;
u8 func_80022CD4(PitchState_80022CD4 *state, PitchCommand_80022CD4 *command)
{
    u32 f;
    u32 of;
    u32 i;
    u32 frq;
    u32 ofrq;
    u32 no;
    static u16 kf[13] = {
        4096, 4339, 4597, 4871, 5160, 5467, 5792, 6137, 6502, 6888, 7298, 7732, 8192,
    };

    frq = (command->word[0] >> 8) & 0xFFFF;
    ofrq = state->sample5C & 0xFFFFFF;
    if (ofrq == frq) {
        state->note50 = (u8)(state->sample5C >> 24);
        state->detuneC0 = 0;
    } else if (ofrq < frq) {
        f = (frq << 12) / ofrq;
        of = f >> 12;
        for (no = 0; no < 11; no++) {
            if (of < (1 << (no + 1))) {
                break;
            }
        }
        f /= 1 << no;
        for (i = 11; i >= 0; i--) {
            if (f > kf[i]) {
                break;
            }
        }
        state->note50 = (u8)(state->sample5C >> 24) + no * 12 + i;
        state->detuneC0 = ((f - kf[i]) * 100) / (kf[i + 1] - kf[i]);
    } else {
        f = (ofrq << 12) / frq;
        of = f >> 12;
        for (no = 0; no < 11; no++) {
            if (of < (1 << (no + 1))) {
                break;
            }
        }
        f /= 1 << no;
        for (i = 11; i >= 0; i--) {
            if (f > kf[i]) {
                break;
            }
        }
        state->note50 = (u8)(state->sample5C >> 24) - no * 12 - i;
        state->detuneC0 = -(s8)(((f - kf[i]) * 100) / (kf[i + 1] - kf[i]));
    }
    state->flags24 |= 0x100;
    return 0;
}
