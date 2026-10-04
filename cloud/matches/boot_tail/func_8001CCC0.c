/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native reconstruction; original middleware names and full types are unknown. */
unsigned short func_8001CCC0(unsigned int value)
{
    if (value > 16383) {
        return 16383;
    }
    return value;
}
