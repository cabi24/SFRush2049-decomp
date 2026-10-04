/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void *(*D_80038014)(void *);
void *func_80014CAC(void *address)
{
    unsigned int region;
    region = (unsigned int)address & 0xFF000000;
    if (region != 0x80000000 && region != 0xB0000000) {
        address = D_80038014(address);
    }
    return address;
}
