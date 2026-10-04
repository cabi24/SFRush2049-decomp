/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native reconstruction; original middleware names and full types are unknown. */
extern int func_8001EDF4(unsigned int);
unsigned int func_800201D0(unsigned int value)
{
    if (func_8001EDF4(value) != -1) {
        return value;
    }
    return 0xFFFFFFFF;
}
