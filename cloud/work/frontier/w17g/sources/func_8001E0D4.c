/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001E0D4 (static, boot 0x8001E0D4, 8 B, segment 0x1cf90 -> src/rom/lib_1cf90.c, converted)
 * Empty no-argument hook (`jr ra; nop`; the following nop is segment alignment). Locked callers in
 * src/rom/lib_113e0.c already declare it `extern void func_8001E0D4(void);`. No shaping devices. */
void func_8001E0D4(void)
{
}
