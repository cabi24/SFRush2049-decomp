/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1) */
/* inflate_read_bits - lib_5610 (ido5.3s). Input refill for NEEDBITS (gzip's fill_inbuf role):
 * wait for the PI DMA of the next 4 KB input block to complete (poll gInflateMsgQueue), swap
 * the two input buffers (gInflateToggle selects which one was just filled), set the read
 * window [gInflateInPtr, gInflateInPtr + 4096), advance the ROM source address by 4096,
 * invalidate the new block in the data cache and start the DMA of the following block into
 * the other buffer; then consume and return the first 16-bit little-endian word.
 * From static_C20/inflate_read_bits.c (via w25a's renamed copy): w26a drops the `volatile`
 * qualifiers and the ptr/high locals (retail reads gInflateInPtr once for the end pointer and
 * the cache call, and the 8-byte frame residual was the two locals), stores gInflateToggle
 * before `next` in the else arm (as1 schedule), and writes the word low | high << 8 as in
 * inflate_block. No shaping devices. */
#include "types.h"
#include "PR/os_message.h"
#include "PR/os_pi.h"

extern u8 *gInflateInPtr, *gInflateInEnd;
extern u32 gInflateSrc;
extern s32 gInflateToggle;
extern u8 gInflateBufferA[], gInflateBufferB[];
extern OSIoMesg gInflateDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);

s32 inflate_read_bits(void) {
    OSMesg msg;
    u8 *next;

    while (osRecvMesg(&gInflateMsgQueue, &msg, OS_MESG_NOBLOCK) == -1) {
    }
    if (gInflateToggle) {
        gInflateInPtr = gInflateBufferA;
        next = gInflateBufferB;
        gInflateToggle = 0;
    } else {
        gInflateInPtr = gInflateBufferB;
        gInflateToggle = 1;
        next = gInflateBufferA;
    }
    gInflateInEnd = gInflateInPtr + 4096;
    gInflateSrc += 4096;
    osInvalDCache(gInflateInPtr, 4096);
    osPiStartDma(&gInflateDmaState, 0, 0, gInflateSrc, next, 4096, &gInflateMsgQueue);
    gInflateInPtr += 2;
    return gInflateInPtr[-2] | (gInflateInPtr[-1] << 8);
}
