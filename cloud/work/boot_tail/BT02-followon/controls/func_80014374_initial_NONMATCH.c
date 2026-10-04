/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* REJECTED INITIAL NONMATCH: direct-return source leaves a two-word pool residual. */
unsigned char func_80014374(unsigned int flags)
{
    if (flags & 0x10000) {
        return 2;
    }
    if (flags & 0x20000) {
        return 3;
    }
    if (flags & 0x40000) {
        return 4;
    }
    if (flags & 0x80000) {
        return 0;
    }
    return 1;
}
