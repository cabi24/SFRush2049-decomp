/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern u8 *gInflateInPtr,*gInflateInEnd,*gInflateOutPtr;
extern u32 gInflateBitBuf,gInflateBitCount;
extern u32 gInflateSrc;
extern s32 gInflateToggle,gLzssToggle;
extern u8 gInflateBufferA[],gInflateBufferB[];
extern OSIoMesg gInflateDmaState,gLzssDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void*,s32);
extern s32 inflate_read_bits(void),inflate_loop(void);
extern void inflate_io_wait(void);
s32 inflate_stored(void) {
 u32 count=gInflateBitCount, bits=gInflateBitBuf, n,value;
 u32 alignment=count&7;
 bits>>=alignment;count-=alignment;
 while(count<16) {if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;value=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else value=inflate_read_bits();bits|=value<<count;count+=16;}
 n=bits&65535;bits>>=16;count-=16;
 while(count<16) {if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;value=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else value=inflate_read_bits();bits|=value<<count;count+=16;}
 if(n!=((~bits)&65535)) return 1;
 bits>>=16;count-=16;
 while(n--) {
  while(count<8) {if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;value=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else value=inflate_read_bits();bits|=value<<count;count+=16;}
  *gInflateOutPtr=(u8)bits;gInflateOutPtr++;
  bits>>=8;count-=8;
 }
 gInflateBitBuf=bits;gInflateBitCount=count;
 return 0;
}
