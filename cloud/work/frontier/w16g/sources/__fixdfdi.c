/* flags: -g0 -O1 -mips3 -32 -G 0 -non_shared */
/* __fixdfdi (static, boot 0x800099F0, 28 B, segment 0xa5f0 -> src/rom/lib_a5f0.c, unconverted)
 * = ultralib __d_to_ll: reference/repos/ultralib (decompals e24c836) src/libc/llcvt.c, verbatim body.
 *   ultralib builds src/libc/llcvt.c at OPTFLAGS -O1 and MIPS_VERSION -mips3 -32 (makefiles/ido.mk); no
 *   BUILD_VERSION dependence. The project's GCC-style label (__fixdfdi) is kept because the slot and
 *   every caller's relocation use it (see RESULTS.md "labels").
 */

long long __fixdfdi(double d) {
    return d;
}
