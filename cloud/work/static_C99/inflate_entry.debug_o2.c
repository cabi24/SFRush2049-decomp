/* flags: -g1 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern u32 gInflateSrc;
extern s32 gInflateToggle;
extern u8 gInflateBufferA[];
extern OSIoMesg gInflateDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern s32 __osPiRawStartDma(OSIoMesg *,s32,s32,u32,void *,u32,OSMesgQueue *);
extern void *sound_play_menu(s32,s32);
extern void inflate_flush_window(void *,s32);
extern s32 inflate_loop(void);
extern void dma_queue_sync(void *);
extern void osInvalICache_full(void *,s32);
s32 inflate_entry(u32 source, u8 *destination, s32 allocate_window)
{
    void *window;
    OSMesg message;
    gInflateOutPtr = destination;
    gInflateSrc = source;
    gInflateInPtr = gInflateBufferA;
    gInflateInEnd = gInflateInPtr;
    gInflateToggle = 1;
    osInvalDCache(gInflateInPtr,4096);
    __osPiRawStartDma(&gInflateDmaState,0,0,gInflateSrc,gInflateInPtr,4096,&gInflateMsgQueue);
    if (allocate_window) {
        window = sound_play_menu(0,12000);
        inflate_flush_window(window,12000);
    } else inflate_flush_window((void *)0x803FD120,12000);
    inflate_loop();
    if (allocate_window) dma_queue_sync(window);
    while (osRecvMesg(&gInflateMsgQueue,&message,0) == -1) { }
    osInvalICache_full(destination,gInflateOutPtr-destination);
    return gInflateOutPtr-destination;
}
