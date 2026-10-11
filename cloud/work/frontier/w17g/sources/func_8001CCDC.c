/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001CCDC (static, boot 0x8001CCDC, 936 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 181 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): SetFXParameters.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-chain-dispatch/func_8001CCDC_NONMATCH.c (Astra / BT03-chain-dispatch).
 * w17g edit: the D10 file's private `Emitter` (flags08 @0x08, identifier34 @0x34, fade40 @0x40) is replaced by
 * the shared StateNode of include/boot_tail_sample_contract.h (same offsets; fade40 -> fade; the D10 `*const` parameter qualifier dropped to match that declaration), which is how the
 * later locked caller in lib_1cf90.c already declares this function. Codegen re-verified (LOCK/TU, IDO 7.1).
 * Shaping devices: none. */
/* SetFXParameters source lead: AxioDL/musyx, snd3d.c, CC0-1.0.
 * The N64 body has four dedicated controller helpers and no paraInfo loop.
 * All six O32 inputs, including the unused yPan, are proven by native callers.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#include "boot_tail_sample_contract.h"
extern u8 func_8001CC9C(u8);
extern u16 func_8001CCC0(u32);
extern int func_8001B7C0(u32, u8);
extern int func_8001B29C(u32, u8);
extern int func_8001B3A0(u32, u8);
extern int func_8001B4A4(u32, u16);
void func_8001CCDC(StateNode *em, float vol, float xPan, float yPan,
                   float zPan, float doppler)
{
    u32 identifier;
    identifier = em->identifier34;
    if (em->flags08 & 0x100000) {
        func_8001B7C0(identifier, func_8001CC9C((u32)((em->fade * vol) * 127.0f)));
    } else {
        func_8001B7C0(identifier, func_8001CC9C((u32)(vol * 127.0f)));
    }
    func_8001B29C(identifier, func_8001CC9C((u32)((1.0f + xPan) * 64.0f)));
    func_8001B3A0(identifier, func_8001CC9C((u32)((1.0f - zPan) * 64.0f)));
    func_8001B4A4(identifier, func_8001CCC0(doppler * 8192.0f));
}
