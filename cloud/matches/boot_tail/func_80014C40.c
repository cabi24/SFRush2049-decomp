/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void osWritebackDCache(void *, int);

void func_80014C40(void *buffer, unsigned int samples)
{
    osWritebackDCache(buffer, samples * 2);
}
