/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
void* memset(register void* dest, register int c, register unsigned n) {
    unsigned char* p = dest;
    int alignment;
    unsigned* words;
    if (c == 0) {
        alignment = (unsigned)p & 3;
        while(n && alignment > 0 && alignment != 4) {
            *p++ = 0;
            alignment++;
            n--;
        }
        words = (unsigned*)p;
        while(n >= 4) { *words++ = 0; n -= 4; }
        p = (unsigned char*)words;
    }
    while(n-- > 0) *p++ = c;
    return dest;
}
