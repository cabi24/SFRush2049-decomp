/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014CFC (static, boot 0x80014CFC, 12 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * Empty two-argument hook: `sw a0,0(sp); jr ra; sw a1,4(sp)` is IDO -O2's code for an empty body with two
 * declared word parameters (both homed). The locked caller func_800162AC (src/rom/lib_16320.c) declares it
 * `extern void func_80014CFC(void **, void **);` and passes (&descriptor, &records->data) after resolving a
 * sample's data pointer, i.e. a stripped "descriptor/data fix-up" hook. No shaping devices. */
void func_80014CFC(void **descriptor, void **data)
{
}
