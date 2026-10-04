/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Select the first enabled mode bit; shared result is genuine control-flow state. */
unsigned char func_80014374(unsigned int flags)
{
    unsigned char result;
    if (flags & 0x10000) {
        result = 2;
    } else if (flags & 0x20000) {
        result = 3;
    } else if (flags & 0x40000) {
        result = 4;
    } else if (flags & 0x80000) {
        result = 0;
    } else {
        result = 1;
    }
    return result;
}
