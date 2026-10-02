/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned char u8;
void func_800BD080(u32 *dl,u32 value) {
    u8 op;
    for (;;dl+=2) {
        op=(dl[0]&0xFF000000)>>24;
        if ((op&0xC0)==0x40 || (op&0xC0)==0x80 || (op>=9 && op<0x40) || (op>=0xC0 && op<0xD6)) {
            continue;
        }
        if (op==0xFD) {
            dl[1]=value;
            return;
        }
        if (op==0xDF) {
            return;
        }
    }
}
