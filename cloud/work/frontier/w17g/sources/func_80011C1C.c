/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80011C1C (static, boot 0x80011C1C, 104 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 14 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): initialize_audio_record.
 * Body + declarations verbatim from cloud/work/boot_tail/BT02-medium/nonmatch/func_80011C1C.c (Astra / BT02-medium).
 * w17g TU-context edit (codegen re-proved): the D10 file's private record struct is replaced by the shared
 * AudioState/AudioSlot of include/boot_tail_audio_record.h (same 0x68-byte record, same offsets), which the
 * locked siblings in lib_11640.c already use; field renames: enabled->pending (0x01), gain->rate (0x04), fields18[i]->unknown18[i] (0x18), fields40[0..3]->value40..value46 (0x40..0x46).
 * Shaping devices: none. */
#include "boot_tail_audio_record.h"
void func_80011C1C(unsigned short index, unsigned short flags)
{
    AudioState *audio;
    audio = &D_80038294[index];
    audio->active = 0;
    audio->flags = flags;
    audio->rate = 4096;
    audio->pending = 1;
    audio->unknown18[0] = 0;
    audio->unknown18[1] = 0;
    audio->unknown18[2] = 0;
    audio->unknown18[3] = 0;
    audio->value40 = 0;
    audio->value42 = 0;
    audio->value44 = 0;
    audio->value46 = 0;
}
