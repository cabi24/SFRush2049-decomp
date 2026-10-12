/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "types.h"
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern s32 sound_play_menu(s32);
extern void inflate_flush_window(s32, s32), dma_queue_sync(s32);
extern s32 inflate_loop(void);
s32 inflate_entry_alt(u8 *src, s32 size, u8 *dst) {
    s32 window;
    gInflateOutPtr = dst;
    gInflateInPtr = src;
    gInflateInEnd = gInflateInPtr + size;
    window = sound_play_menu(12000);
    inflate_flush_window(window, 12000);
    inflate_loop();
    dma_queue_sync(window);
    return gInflateOutPtr - dst;
}
