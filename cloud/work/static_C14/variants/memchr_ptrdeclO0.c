/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"

u8 *memchr(register u8 *src, register u32 c, register s32 count) { u8 *p; p=src; while(count--) { if(*p==c) return p; p++; } return NULL; }
