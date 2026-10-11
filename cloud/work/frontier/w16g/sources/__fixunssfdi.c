/* flags: -g0 -O1 -mips3 -32 -G 0 -non_shared */
/* __fixunssfdi (static, boot 0x80009AC8, 156 B, segment 0xa5f0 -> src/rom/lib_a5f0.c, unconverted)
 * = ultralib __f_to_ull: reference/repos/ultralib (decompals e24c836) src/libc/llcvt.c, verbatim body.
 *   ultralib builds src/libc/llcvt.c at OPTFLAGS -O1 and MIPS_VERSION -mips3 -32 (makefiles/ido.mk); no
 *   BUILD_VERSION dependence. The project's GCC-style label (__fixunssfdi) is kept because the slot and
 *   every caller's relocation use it (see RESULTS.md "labels").
 * DATA: the object carries a .rodata literal (the 2^63 bias); the TU must own it at the
 *   retail address (owned-rodata slot) - see RESULTS.md. Not promotable by body splice alone.
 */

unsigned long long __fixunssfdi(float f) {
    return f;
}
