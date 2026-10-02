/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern u8 * volatile gInflateInPtr;extern u8 *gInflateInEnd,*gInflateOutPtr;
extern volatile u32 gInflateBitBuf,gInflateBitCount;
extern u32 gInflateSrc;
extern s32 gInflateToggle,gLzssToggle;
extern u8 gInflateBufferA[],gInflateBufferB[];
extern OSIoMesg gInflateDmaState,gLzssDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void*,s32);
extern s32 inflate_read_bits(void),inflate_loop(void);
extern void inflate_io_wait(void);
s32 inflate_read_bits(void) {
 OSMesg msg;
 u8 *next,*ptr;u32 high;
 while(osRecvMesg(&gInflateMsgQueue,&msg,OS_MESG_NOBLOCK)==-1) {}
 if(gInflateToggle) {gInflateInPtr=gInflateBufferA;next=gInflateBufferB;gInflateToggle=0;}
 else {gInflateInPtr=gInflateBufferB;next=gInflateBufferA;gInflateToggle=1;}
 gInflateInEnd=gInflateInPtr+4096;
 gInflateSrc+=4096;
 osInvalDCache(gInflateInPtr,4096);
 __osPiRawStartDma(&gInflateDmaState,0,0,gInflateSrc,next,4096,&gInflateMsgQueue);
 gInflateInPtr+=2;
 ptr=gInflateInPtr;high=ptr[-1]<<8;return high|ptr[-2];
}
