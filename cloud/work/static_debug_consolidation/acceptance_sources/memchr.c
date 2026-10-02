/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
u8 *memchr(u8 *src, u32 c, s32 count) { u8 *p=src; while(count--) { if(*p==c) return p; p++; } return NULL; }
