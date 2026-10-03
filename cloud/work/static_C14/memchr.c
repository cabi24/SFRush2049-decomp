/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"

u8 *memchr(u8 *src, u32 c, s32 count) { u8 *p=src; while(count--) { if(*p==c) return p; p++; } return NULL; }
