/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void osWritebackDCache(void *, int);

void func_80014C60(short *buffer, int samples)
{
    buffer[samples] = buffer[0];
    buffer[samples + 1] = buffer[1];
    buffer[samples + 2] = buffer[2];
    buffer[samples + 3] = buffer[3];
    osWritebackDCache(buffer + samples, 8);
}
