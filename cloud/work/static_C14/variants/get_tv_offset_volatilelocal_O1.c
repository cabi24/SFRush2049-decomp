/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern u32 osTvType;
s32 get_tv_offset(void) { volatile s32 offset; if(osTvType==1) offset=0; else if(osTvType==0) offset=14; else offset=28; return offset; }
