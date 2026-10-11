/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014BF8 (static, boot 0x80014BF8, 32 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * One-line forwarder to the locked func_80011074 (lib_11640.c: if the pending audio buffer D_800382E8 is set,
 * stop and free it). The locked caller func_80020558 (lib_207b0.c) declares it `extern void
 * func_80014BF8(void);`. No shaping devices. */
extern void func_80011074(void);
void func_80014BF8(void)
{
    func_80011074();
}
