/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Display-list walker (0xDF end, 0xDE call/branch, 0xE1+0x04 branch-z); no quirks. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

void display_list_traverse(u32 *dl, u32 match, u32 replace, s32 flags, s32 (*cb)(u32 *)) {
    u32 w0;
    u32 addr;

    while (*dl != 0xDF000000) {
        if ((*dl & 0xFF000000) == 0xDE000000) {
            w0 = *dl++;
            addr = *dl;
            if (addr & 0x0F000000) {
                addr &= 0xF0FFFFFF;
            } else if (!(addr & 0xFF000000)) {
                addr += 0x80000000;
            }
            if ((flags & 8) && addr == match) {
                *dl = replace;
            }
            dl++;
            if (!(w0 & 0x00FF0000)) {
                if (!(flags & 4)) {
                    display_list_traverse((u32 *) addr, match, replace, flags, cb);
                }
            } else {
                dl = (u32 *) addr;
            }
        } else if ((*dl & 0xFF000000) == 0xE1000000) {
            addr = dl[1];
            dl += 2;
            if (addr & 0x0F000000) {
                addr &= 0xF0FFFFFF;
            } else if (!(addr & 0xFF000000)) {
                addr += 0x80000000;
            }
            if ((*dl & 0xFF000000) == 0x04000000) {
                dl += 2;
                if (flags & 2) {
                    dl = (u32 *) addr;
                } else if (flags & 1) {
                    display_list_traverse((u32 *) addr, match, replace, flags, cb);
                }
            } else {
                dl += 2;
            }
        } else if (cb != 0) {
            dl += cb(dl);
        } else {
            dl += 2;
        }
    }
}
