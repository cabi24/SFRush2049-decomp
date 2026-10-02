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
extern s32 sound_play_menu(s32,s32);
extern void inflate_flush_window(s32,s32),dma_queue_sync(s32);
s32 inflate_entry_alt(u8 *src, s32 size, u8 * volatile dst) {
 s32 window;
 gInflateOutPtr=dst;
 gInflateInPtr=src;
 gInflateInEnd=gInflateInPtr+size;
 window=sound_play_menu(0,12000);
 inflate_flush_window(window,12000);
 inflate_loop();
 dma_queue_sync(window);
 return gInflateOutPtr-dst;
}
