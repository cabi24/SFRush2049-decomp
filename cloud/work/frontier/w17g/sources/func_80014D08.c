/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014D08 (static, boot 0x80014D08, 12 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * Empty two-argument hook (same code as func_80014CFC). Its caller func_800163A8 (lib_16320) passes
 * a0 = &record->descriptor (record + 0xC) and a1 = record->data (lw 4(record)) when a sample record's reference
 * count drops to zero, i.e. the release counterpart of func_80014CFC. Parameter types follow those call-site
 * values (pointer-to-pointer, pointer); any two word types give the same code. No shaping devices. */
void func_80014D08(void **descriptor, void *data)
{
}
