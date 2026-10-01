/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed int s32;
s32 func_8010FD60(s32 arg0) {
    return (arg0 & 0x0FFFFFFF) | 0xB0000000;
}
