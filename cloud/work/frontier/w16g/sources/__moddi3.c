/* flags: -g0 -O1 -mips3 -32 -G 0 -non_shared */
/* __moddi3 (static, boot 0x8000DAE8, 156 B, segment 0xe4f0 -> src/rom/lib_e4f0.c, unconverted)
 * = ultralib __ll_mod: reference/repos/ultralib (decompals e24c836) src/libc/ll.c, verbatim body.
 *   ultralib builds src/libc/ll.c at OPTFLAGS -O1 and MIPS_VERSION -mips3 -32 (makefiles/ido.mk); no
 *   BUILD_VERSION dependence. The project's GCC-style label (__moddi3) is kept because the slot and
 *   every caller's relocation use it (see RESULTS.md "labels").
 */

long long __moddi3(long long a0, long long a1) {
    long long tmp = a0 % a1;

    if ((tmp < 0 && a1 > 0) || (tmp > 0 && a1 < 0)) {
        tmp += a1;
    }
    return tmp;
}
