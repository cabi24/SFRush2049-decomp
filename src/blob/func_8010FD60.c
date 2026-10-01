/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Map a physical/cart address into the uncached cartridge address window. */
int func_8010FD60(int arg0) {
    return (arg0 & 0x0FFFFFFF) | 0xB0000000;
}
