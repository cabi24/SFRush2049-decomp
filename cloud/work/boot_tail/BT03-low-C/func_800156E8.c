/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern int func_8001558C(unsigned short, unsigned short, unsigned int, unsigned int, unsigned char);

int func_800156E8(unsigned short group, unsigned short item, unsigned int context, unsigned int option)
{
    return func_8001558C(group, item, context, option, 0);
}
