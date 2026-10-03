/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void osWritebackDCache(void *, int);

void func_80014C60(short *buffer, unsigned int samples)
{
    int i;
    for (i = 0; i < 4; ++i) {
        buffer[samples + i] = buffer[i];
    }
    osWritebackDCache(buffer + samples, 8);
}
