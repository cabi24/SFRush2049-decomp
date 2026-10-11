/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80014BD8 (static, boot 0x80014BD8, 32 B, segment 0x11640 -> src/rom/lib_11640.c, converted)
 * One-line forwarder: calls func_80010E80 (lib_11640, still GLOBAL_ASM; it reads lhu 0(a0), so it takes a
 * pointer) with its own argument untouched in a0. The locked caller func_80020528 (src/rom/lib_207b0.c)
 * declares it `extern void func_80014BD8(void *);` and passes its state pointer. No shaping devices. */
extern void func_80010E80(void *state);
void func_80014BD8(void *state)
{
    func_80010E80(state);
}
