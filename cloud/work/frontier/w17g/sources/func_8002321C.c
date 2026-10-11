/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8002321C (static, boot 0x8002321C, 136 B, segment 0x22300 -> src/rom/lib_22300.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 20 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_command_handler_8002321C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT05-variables/nonmatch/func_8002321C.c (Astra / BT05-variables).
 * w17g TU-context edit: func_80016E68 (id -> data table lookup, lib_16320) is declared `void *` here and in
 * the other w17g caller (the D10 files used two different pointee types); assignment converts, no code change.
 * Shaping devices: none. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
extern void *func_80016E68(u16);
u32 func_8002321C(u32 volume, u16 curve)
{
    u8 *table;
    u32 high;
    u32 low;
    u32 difference;
    if (curve != 0) {
        if ((table = func_80016E68(curve)) != 0) {
            high = volume >> 16;
            low = volume & 0xFFFF;
            if (high < 127) {
                difference = low * (table[high + 1] - table[high]);
                volume = difference + (table[high] << 16);
            } else {
                volume = table[high] << 16;
            }
        }
    }
    return volume;
}
