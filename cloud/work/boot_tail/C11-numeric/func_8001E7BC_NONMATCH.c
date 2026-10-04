/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
extern const short D_8002CC54[1024];
short func_8001E7BC(u16 phase)
{
    phase &= 4095;
    if (phase < 1024) {
        return D_8002CC54[phase];
    }
    if (phase < 2048) {
        return D_8002CC54[1023 - (phase & 1023)];
    }
    if (phase < 3072) {
        return -D_8002CC54[phase & 1023];
    }
    return -D_8002CC54[1023 - (phase & 1023)];
}
