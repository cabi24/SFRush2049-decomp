/* flags: -g0 -O1 -mips3 -32 -G 0 -non_shared */
/* __qdivrem (static, boot 0x8000DA88, 96 B, segment 0xe4f0 -> src/rom/lib_e4f0.c, unconverted)
 * = ultralib __ull_divremi: reference/repos/ultralib (decompals e24c836) src/libc/ll.c, verbatim body.
 *   ultralib builds src/libc/ll.c at OPTFLAGS -O1 and MIPS_VERSION -mips3 -32 (makefiles/ido.mk); no
 *   BUILD_VERSION dependence. The project's GCC-style label (__qdivrem) is kept because the slot and
 *   every caller's relocation use it (see RESULTS.md "labels").
 */

void __qdivrem(unsigned long long* div, unsigned long long* rem, unsigned long long a2, unsigned short a3) {
    *div = a2 / a3;
    *rem = a2 % a3;
}
