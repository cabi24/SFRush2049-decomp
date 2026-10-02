/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
u8 *func_800BE6A4(u8 *destination,u8 *source) {
    u8 *out,*in; u8 c;
    out=destination; in=source;
    if (*in==255) {
        *out++=*in++;
        while (in[0]!=0 || in[1]!=0) {
            *out++=*in++;
            *out++=*in++;
        }
        *out++=0;
        *out=0;
    } else {
        while ((*out++=*in++)!=0) {}
    }
    return destination;
}
