/* flags: -g0 -O1 -mips3 -32 -G 0 -non_shared */
/* __udivdi3 (static, boot 0x8000D958, 60 B, segment 0xe4f0 -> src/rom/lib_e4f0.c, unconverted)
 * = ultralib __ull_div: reference/repos/ultralib (decompals e24c836) src/libc/ll.c, verbatim body.
 *   ultralib builds src/libc/ll.c at OPTFLAGS -O1 and MIPS_VERSION -mips3 -32 (makefiles/ido.mk); no
 *   BUILD_VERSION dependence. The project's GCC-style label (__udivdi3) is kept because the slot and
 *   every caller's relocation use it (see RESULTS.md "labels").
 */

unsigned long long __udivdi3(unsigned long long a0, unsigned long long a1) {
    return a0 / a1;
}
