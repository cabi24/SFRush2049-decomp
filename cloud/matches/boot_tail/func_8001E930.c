/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Unsigned time-unit scaling; see C11-small/README.md for provenance. */
void func_8001E930(unsigned int *time)
{
    *time *= 256U;
}
