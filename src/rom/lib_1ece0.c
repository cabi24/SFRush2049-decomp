/* GENERATED ROM-aligned TU — segment 0x1ece0 (rom/lib_1ece0)
 * layout map 9574159704610867be8d0c06826fb45c38860c9806fbcd314befaf1c5d1b3e92; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — func_8001E0E0
 * Source:   cloud/work/frontier/w23c/sources/func_8001E0E0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w23c/sources/func_8001E0E0.c:func_8001E0E0 (score0); rom_tu.h context: cloud/work/frontier/w23i/ctx71_B.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-11 — func_8001E440
 * Source:   cloud/work/frontier/w17g/sources/func_8001E440.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w17g/sources/func_8001E440.c:func_8001E440 (score0); rom_tu.h context: cloud/work/frontier/w17k/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern const float D_8002D924;
u16 func_8001E440(u16 value)
{
    return (u16)(u32)((float)value * D_8002D924);
}

/* PROMOTED 2026-10-11 — func_8001E50C
 * Source:   cloud/work/frontier/w23c/sources/func_8001E50C.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w23c/sources/func_8001E50C.c:func_8001E50C (score0); rom_tu.h context: cloud/work/frontier/w23c/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern const float D_8002C640[];
extern const float D_8002C840[];
extern int D_8004F800;
u32 func_8001E50C(u8 note, u32 encoded_value)
{
    u8 reference;
    float value;
    if (encoded_value == 0xFFFFFFFFU) {
        encoded_value = 0x40005622U;
    }
    reference = encoded_value >> 24;
    if (note != reference) {
        value = (reference < note ? D_8002C640[note - reference] : D_8002C840[reference - note]) *
                (float)(encoded_value & 0xFFFFFFU);
    } else {
        value = (float)(encoded_value & 0xFFFFFFU);
    }
    return (u32)((value * 4096.0f) / (float)D_8004F800);
}

/* PROMOTED 2026-10-04 — func_8001E688
 * Source:   cloud/matches/boot_tail/func_8001E688.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/matches/boot_tail/func_8001E688.c:func_8001E688 (score0); rom_tu.h context: cloud/work/boot_tail_promotion/context.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
double func_8001E688(double value)
{
    return (unsigned int)value;
}

