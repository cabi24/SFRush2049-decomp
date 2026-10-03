/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Allocation callback wrapper; see cloud/work/boot_tail/C11-small/README.md. */
extern void *(*D_80038018)(unsigned int size, unsigned int mode);

void *func_8001E740(unsigned int size)
{
    return D_80038018(size, 0);
}
