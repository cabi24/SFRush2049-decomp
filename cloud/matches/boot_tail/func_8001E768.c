/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Release callback wrapper; see cloud/work/boot_tail/C11-small/README.md. */
extern void (*D_8003801C)(void *allocation);

void func_8001E768(void *allocation)
{
    D_8003801C(allocation);
}
