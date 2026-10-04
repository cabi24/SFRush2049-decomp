/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
extern u32 func_80019AA8(unsigned char *);
void func_8001E940(u32 *value, unsigned char *state)
{
    u32 rate;
    rate = func_80019AA8(state);
    *value = (((*value << 16) / rate) * 1000U) >> 5;
}
