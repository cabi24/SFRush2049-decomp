/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern u8 * volatile gInflateInPtr, * volatile gInflateInEnd, * volatile gInflateOutPtr;
extern volatile u32 gInflateBitBuf,gInflateBitCount;
extern u32 gInflateSrc;
extern s32 gInflateToggle,gLzssToggle;
extern u8 gInflateBufferA[],gInflateBufferB[];
extern OSIoMesg gInflateDmaState,gLzssDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void*,s32);
extern s32 inflate_read_bits(void),inflate_loop(void);
extern u8 *inflate_io_wait(u8 **input_out);
extern u32 gLzssSrc;
u8 *inflate_io_wait(u8 **input_out) {
 OSMesg msg;
 u8 *fill,*next;
 while(osRecvMesg(&gInflateMsgQueue,&msg,OS_MESG_NOBLOCK)==-1) {}
 if(gLzssToggle==1) {fill=gInflateBufferB;next=gInflateBufferA;}
 else {fill=gInflateBufferA;next=gInflateBufferB;}
 osInvalDCache(fill,4096);
 __osPiRawStartDma(&gLzssDmaState,0,0,gLzssSrc,fill,4096,&gInflateMsgQueue);
 gLzssSrc+=4096;
 gLzssToggle^=1;
 *input_out=next;return next;
}

s32 lzss_decode(u32 src,u8 *dst) {
 u8 *input=gInflateBufferA,*start=dst,*copy;
 s32 remaining=0,flags_left=0,length;
 u32 flags,offset,byte;
 OSMesg msg;
 gLzssToggle=1;
 osInvalDCache(input,4096);
 __osPiRawStartDma(&gLzssDmaState,0,0,src,input,4096,&gInflateMsgQueue);
 gLzssSrc=src+4096;
 for(;;) {
  if(flags_left==0) {
   if(remaining<=0) {input=inflate_io_wait(&input);remaining=4096;}
   flags=*input++;remaining--;flags_left=8;
  }
  flags_left--;
  if(flags&1) {
   if(remaining<=0) {input=inflate_io_wait(&input);remaining=4096;}
   byte=*input++;remaining--;*dst++=byte;
  } else {
   if(remaining<=0) {input=inflate_io_wait(&input);remaining=4096;}
   byte=*input++;remaining--;offset=(byte&240)<<4;length=byte&15;
   if(remaining<=0) {input=inflate_io_wait(&input);remaining=4096;}
   byte=*input++;remaining--;offset=(offset+byte)&4095;
   if(offset==0 && length==0) {
    while(osRecvMesg(&gInflateMsgQueue,&msg,OS_MESG_NOBLOCK)==-1) {}
    return dst-start;
   }
   length++;
   copy=dst-offset;
   do {*dst++=*copy++;} while(--length>=0);
  }
  flags>>=1;
 }
}

void inflate_flush_window(s32 arg0, s32 arg1) {
    gDisplayListHead = arg0;
    gDisplayListEnd = arg1;
    *(s32 *)&gDisplayListSize = 0;
}

s32 huft_alloc(s32 arg0)
{
  int new_var;
  gDisplayListSize += arg0;
  new_var = gDisplayListSize;
  new_var = (new_var - arg0) + gDisplayListHead;
  if (1)
  {
  }
  return new_var;
}
