/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "types.h"
#include "PR/os_message.h"
#include "PR/os_pi.h"
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern u32 gInflateSrc;
extern s32 gInflateToggle;
extern u8 gInflateBufferA[];
extern OSIoMesg gInflateDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
extern s32 sound_play_menu(s32);
extern void inflate_flush_window(s32, s32), dma_queue_sync(s32);
extern s32 inflate_loop(void);
extern void osInvalICache_full(void *, s32);
s32 inflate_entry(u32 source, u8 *destination, s32 allocate_window)
{
    OSMesg message;
    s32 window;
    gInflateOutPtr = destination;
    gInflateSrc = source;
    gInflateInPtr = gInflateBufferA;
    gInflateInEnd = gInflateInPtr;
    gInflateToggle = 1;
    osInvalDCache(gInflateInPtr, 4096);
    osPiStartDma(&gInflateDmaState, 0, 0, gInflateSrc, gInflateInPtr, 4096, &gInflateMsgQueue);
    if (allocate_window) {
        window = sound_play_menu(12000);
        inflate_flush_window(window, 12000);
    } else inflate_flush_window(0x803FD120, 12000);
    inflate_loop();
    if (allocate_window) dma_queue_sync(window);
    while (osRecvMesg(&gInflateMsgQueue, &message, 0) == -1) { }
    osInvalICache_full(destination, gInflateOutPtr - destination);
    return gInflateOutPtr - destination;
}
