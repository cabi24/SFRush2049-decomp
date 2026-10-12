/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* func_8001E0E0 - lib_1ece0 (IDO 7.1). Voice volume/pan conversion: the N64 MusyX ancestor of the GameCube
 * hw_volconv.c CalcBus (doldecomp/ttyd libs/musyx; shape only): 16.16 volume -> gain interpolated in the
 * 129-entry volume table D_8002CA40 (musyx_vol_tab), 8.22 surround pan -> span output (gain * pan_tab *
 * 0.7079), the front gain scaled by the mirrored surround pan, then left/right from the pan (centre 0x800000:
 * fixed 0.7079 / -0.7079 split), and the aux send from its own 16.16 level; every output * 32767 to u16.
 * D_8002CC44 = pan_tab {0, 0.7079, 1}. Caller: func_80014A74 (lib_11640, locked).
 * w23c levers (w21b best true=2215 -> 0):
 * - The constants are natural literals (32767.0f, 0.7079f, -0.7079f): retail's D_8002D910..D_8002D920 are this
 *   function's own literal pool (46fffe00 3f3538ef 3f3538ef bf3538ef 3f3538ef, IDO 7.1 does not merge equal
 *   literals; the compiled .rodata is byte-identical). 32767.0f is then a constant web, numbered after every
 *   variable/expression web, so the in-order p2 colouring gives it $f20 (callee-saved) after the six caller-saved
 *   FP registers are taken, as retail; an extern (or a `scale` variable) is web 0 and takes $f0 (true=925..2910).
 * - The front-surround fraction goes through `fraction` (a statement before factor): its expression web is then
 *   numbered before factor's variable web ($f12 / $f14 as retail); the other five interpolations are
 *   indifferent (64-variant sweep, all equal).
 * - -Wab,-r4300_mul: retail has the R4300 nop between back-to-back mul.s (a NEUTRAL_FLAGS flag for the segment).
 * Owned data: the five literals are 20 bytes of .rodata at 0x8002D910 (data.bin 0x6140): needs an owned .rodata
 *   slot (logical extent, kind "constants"; func_8001E440's D_8002D924 follows), as func_8001C860 (w21i).
 * No devices, no unused locals, no pad arrays, no false prototypes, no implicit declarations. */
#include "types.h"
extern const float D_8002CA40[129];
extern const float D_8002CC44[3];

void func_8001E0E0(u16 *left, u16 *right, u32 volume, u32 pan,
                   u32 span, u16 *span_output, u32 aux, u16 *aux_output)
{
    float fraction;
    float gain;
    float factor;
    const float *entry;

    if (volume > 0x7F0000U) {
        volume = 0x7F0000U;
    }
    entry = &D_8002CA40[volume >> 16];
    gain = (1.0f - (float)(volume & 0xFFFFU) * (1.0f / 65536.0f)) * entry[0] +
           entry[1] * ((float)(volume & 0xFFFFU) * (1.0f / 65536.0f));
    fraction = (float)(span & 0x3FFFFFU) * (1.0f / 4194304.0f);
    entry = &D_8002CC44[span >> 22];
    factor = (1.0f - fraction) * entry[0] +
           entry[1] * fraction;
    *span_output = (int)(gain * factor * 0.7079f * 32767.0f);
    span = 0x800000U - span;
    if (span >= 0x800000U) {
        span = 0x7F0000U;
    }
    entry = &D_8002CC44[span >> 22];
    factor = (1.0f - (float)(span & 0x3FFFFFU) * (1.0f / 4194304.0f)) * entry[0] +
           entry[1] * ((float)(span & 0x3FFFFFU) * (1.0f / 4194304.0f));
    gain *= factor;
    if (pan == 0x800000U) {
        *left = (int)(gain * 0.7079f * 32767.0f);
        *right = (int)(gain * -0.7079f * 32767.0f);
    } else {
        entry = &D_8002CC44[pan >> 22];
        factor = (1.0f - (float)(pan & 0x3FFFFFU) * (1.0f / 4194304.0f)) * entry[0] +
               entry[1] * ((float)(pan & 0x3FFFFFU) * (1.0f / 4194304.0f));
        *left = (int)(gain * factor * 32767.0f);
        pan = 0x800000U - pan;
        if (pan >= 0x800000U) {
            pan = 0x7F0000U;
        }
        entry = &D_8002CC44[pan >> 22];
        factor = (1.0f - (float)(pan & 0x3FFFFFU) * (1.0f / 4194304.0f)) * entry[0] +
               entry[1] * ((float)(pan & 0x3FFFFFU) * (1.0f / 4194304.0f));
        *right = (int)(gain * factor * 32767.0f);
    }
    if (aux > 0x7F0000U) {
        aux = 0x7F0000U;
    }
    entry = &D_8002CA40[aux >> 16];
    gain = (1.0f - (float)(aux & 0xFFFFU) * (1.0f / 65536.0f)) * entry[0] +
           entry[1] * ((float)(aux & 0xFFFFU) * (1.0f / 65536.0f));
    *aux_output = (int)(gain * 0.7079f * 32767.0f);
}
