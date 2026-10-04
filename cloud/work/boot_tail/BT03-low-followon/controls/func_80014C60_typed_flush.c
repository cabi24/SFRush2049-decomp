/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void osWritebackDCache(void *, int);

void func_80014C60(short *buffer, unsigned int samples)
{
    unsigned int offset;
    short *destination;
    offset = samples * 2;
    destination = (short *)((unsigned char *)buffer + offset);
    destination[0] = buffer[0];
    destination[1] = buffer[1];
    destination[2] = buffer[2];
    destination[3] = buffer[3];
    osWritebackDCache(buffer + samples, 8);
}
