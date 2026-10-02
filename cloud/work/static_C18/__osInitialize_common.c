/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
typedef struct { u32 inst1, inst2, inst3, inst4; } ExceptionVector;
extern ExceptionVector __osException;
extern s32 gSpTaskState;
extern OSTime gAudioDmaCounter;
extern u32 osResetType;
extern u8 osAppNMIBuffer[];
extern u32 __osGetSR(void), osCauseGet(void);
extern void __osSetSR(u32), __osSetFpcCsr(u32), __osPiReadDeviceType(void), __osTlbFlush(void), __osTlbInit(void);
extern s32 osPiReadIo(u32,u32*), osPiWriteWord(u32,u32);
extern void osInvalICache_full(void*,s32);
void __osInitialize_common(void) {
 u32 pifdata;
 u32 clock = 0;
 gSpTaskState=1;
 __osSetSR(__osGetSR() | 0x20000000);
 __osSetFpcCsr(0x01000800);
 while(osPiReadIo(0x1fc007fc,&pifdata)) {}
 while(osPiWriteWord(0x1fc007fc,pifdata|8)) {}
 *(ExceptionVector*)0x80000000 = __osException;
 *(ExceptionVector*)0x80000080 = __osException;
 *(ExceptionVector*)0x80000100 = __osException;
 *(ExceptionVector*)0x80000180 = __osException;
 osWritebackDCache((void*)0x80000000,0x190);
 osInvalICache_full((void*)0x80000000,0x190);
 __osPiReadDeviceType();
 __osTlbFlush();
 __osTlbInit();
 gAudioDmaCounter=gAudioDmaCounter*3/4;
 if(osResetType==0) bzero(osAppNMIBuffer,0x40);
 if(osTvType==0) gAudioDmaBufferPtr=0x2f5b2d2;
 else if(osTvType==2) gAudioDmaBufferPtr=0x2e6025c;
 else gAudioDmaBufferPtr=0x2e6d354;
 if(osCauseGet()&0x1000) { while(1) {} }
 *(volatile u32*)0xa4500008=1;
 *(volatile u32*)0xa4500010=0x3fff;
 *(volatile u32*)0xa4500014=0xf;
}
