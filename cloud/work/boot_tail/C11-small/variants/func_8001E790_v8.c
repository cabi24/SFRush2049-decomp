/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* MusyX sndRand source-family lead; see C11-small/README.md for provenance. */
extern unsigned int D_8002CC50;

unsigned int func_8001E790(void)
{
    unsigned int result;

    D_8002CC50 *= 2822053219U;
    result = D_8002CC50 >> 6;
    return result & 0xFFFFU;
}
