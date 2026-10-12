/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* rmonPrintf (static, boot 0x8000870C, 36 B incl. alignment, segment 0x92f0 -> src/rom/lib_92f0.c, IDO 5.3)
 * = ultralib src/libc/syncprintf.c rmonPrintf, VERSION_J branch (`BUILD_VERSION <= VERSION_J`) with _FINALROM:
 * the body is compiled out, leaving the two declared locals (`ans`, `ap`) = the 8-byte frame and the varargs
 * home stores. Ultralib text with the `#ifndef _FINALROM` block removed (the build is FINALROM); `va_list` spelled as its IDO definition (char *).
 * w24b: no shaping devices (the two unused locals are ultralib's own declarations, not added). */
typedef char *va_list;

void rmonPrintf(const char* fmt, ...) {
    int ans;
    va_list ap;

}
