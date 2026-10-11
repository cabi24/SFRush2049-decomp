/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014A0C (static, boot 0x80014A0C, 104 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 24 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_buffer_utility_80014A0C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-low-followon/func_80014A0C_NONMATCH.c (Astra / BT03-low-followon).
 * w17g TU-context edit (codegen re-proved): the D10 file's private record struct is replaced by the shared
 * AudioState/AudioSlot of include/boot_tail_audio_record.h (same 0x68-byte record, same offsets), which the
 * locked siblings in lib_11640.c already use; field renames: none.
 * Shaping devices: none. */
#include "boot_tail_audio_record.h"
void func_80014A0C(int index, unsigned short rate)
{
    if (rate <= 8192) {
        D_80038294[index].rate = rate;
    } else {
        D_80038294[index].rate = 8192;
    }
}
