/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800206AC (static, boot 0x800206AC, 372 B, segment 0x21210 -> src/rom/lib_21210.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 91 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_data_dispatch_800206AC.
 * Body + declarations verbatim from cloud/work/boot_tail/C13-controller-remainder/nonmatch/func_800206AC.c (Astra / C13-controller-remainder).
 * Shaping devices: none. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern void func_80020610(u8 controller, u8 channel, u8 set, u8 value);

void func_800206AC(u8 controller, u8 channel, u8 set, u16 value)
{
    if (controller < 64) {
        func_80020610(controller & 31, channel, set, value >> 7);
        func_80020610((controller & 31) + 32, channel, set, value & 127);
    } else if (controller == 128 || controller == 129) {
        func_80020610(controller & 254, channel, set, value >> 7);
        func_80020610((controller & 254) + 1, channel, set, value & 127);
    } else if (controller == 132 || controller == 133) {
        func_80020610(controller & 254, channel, set, value >> 7);
        func_80020610((controller & 254) + 1, channel, set, value & 127);
    } else {
        func_80020610(controller, channel, set, value >> 7);
    }
}
