/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001785C (static, boot 0x8001785C, 84 B, segment 0x17dc0 -> src/rom/lib_17dc0.c, converted)
 * COMPILER: IDO 7.1 (decompals ido-static-recomp v1.2 ido-7.1-recomp-linux). Exact (true=0 strict=0
 * reloc_blind=0, objdump -dr identical) only under 7.1; IDO 5.3 scores 17 word(s) off, which
 * is why D10 recorded it COMPLETE-NONMATCH. See w17g RESULTS.md "IDO 7.1".
 * Semantics (D10 ledger name hypothesis, unchanged): audio_channel_control_8001785C.
 * Body + declarations verbatim from cloud/work/boot_tail/BT03-high-medium/nonmatch/func_8001785C.c (Astra / BT03-high-medium).
 * Shaping devices: none. */
typedef unsigned char u8;
typedef struct MapRecord { u8 unknown00[5]; u8 key; u8 unknown06[2]; } MapRecord;
void func_8001785C(u8 *map, MapRecord *record)
{
    u8 index;
    for (index = 0; index < 128; index++) map[index] = 255;
    index = 0;
    while (record->key != 255) {
        map[record->key] = index;
        index++;
        record++;
    }
}
