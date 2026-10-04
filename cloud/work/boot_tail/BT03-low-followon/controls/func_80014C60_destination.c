/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void osWritebackDCache(void *, int);

void func_80014C60(short *buffer, unsigned int samples)
{
    short *destination;
    destination = buffer + samples;
    destination[0] = buffer[0];
    destination[1] = buffer[1];
    destination[2] = buffer[2];
    destination[3] = buffer[3];
    osWritebackDCache(destination, 8);
}
