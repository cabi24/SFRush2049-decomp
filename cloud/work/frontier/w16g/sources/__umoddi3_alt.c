/* flags: -g0 -O1 -mips3 -32 -G 0 -non_shared */
/* __umoddi3_alt (static, boot 0x8000D9C0, 60 B, segment 0xe4f0 -> src/rom/lib_e4f0.c, unconverted)
 * = ultralib __ll_rem: reference/repos/ultralib (decompals e24c836) src/libc/ll.c, verbatim body.
 *   ultralib builds src/libc/ll.c at OPTFLAGS -O1 and MIPS_VERSION -mips3 -32 (makefiles/ido.mk); no
 *   BUILD_VERSION dependence. The project's GCC-style label (__umoddi3_alt) is kept because the slot and
 *   every caller's relocation use it (see RESULTS.md "labels").
 */

long long __umoddi3_alt(unsigned long long a0, long long a1) {
    return a0 % a1;
}
