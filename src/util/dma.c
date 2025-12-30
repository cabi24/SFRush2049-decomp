/**
 * @file dma.c
 * @brief DMA and decompression wrapper functions
 *
 * Decompiled from asm/us/3140.s
 * Contains functions for synchronizing DMA transfers during decompression.
 */

#include "types.h"
#include "PR/os_message.h"

/* External OS functions */
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
extern s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

/* External decompression functions */
extern s32 lzss_decode(void *src, void *dst);
extern s32 inflate_entry(void *src, void *dst, s32 use_heap);

/* DMA message queue */
extern OSMesgQueue gDmaMessageQueue;      /* D_8002F190 */
extern OSMesg gDmaMessageBuffer[];        /* D_8002F1A8 */

/* DMA initialized flag */
extern u8 gDmaInitialized;                /* D_8002B030 */

/**
 * Initialize DMA message queue
 * (func_80002540 - dma_queue_init)
 *
 * Creates a message queue for DMA synchronization and
 * posts an initial message to indicate ready state.
 */
void dma_queue_init(void) {
    gDmaInitialized = 1;
    osCreateMesgQueue(&gDmaMessageQueue, gDmaMessageBuffer, 1);
    osJamMesg(&gDmaMessageQueue, NULL, 0);
}

/**
 * Wait for DMA completion
 * (func_80002598 - dma_wait)
 *
 * Waits for a DMA transfer to complete. If blocking is requested,
 * blocks until complete. Otherwise polls.
 *
 * @param blocking Non-zero to block, 0 to poll
 * @return 1 if ready, 0 if not ready (polling only)
 */
s32 dma_wait(s32 blocking) {
    OSMesg msg;

    /* Initialize queue if not done */
    if (gDmaInitialized == 0) {
        dma_queue_init();
    }

    if (blocking != 0) {
        /* Blocking receive */
        osRecvMesg(&gDmaMessageQueue, &msg, 1);
    } else {
        /* Non-blocking receive */
        if (osRecvMesg(&gDmaMessageQueue, &msg, 0) == -1) {
            return 0;  /* Not ready */
        }
    }
    return 1;  /* Ready */
}

/**
 * Signal DMA complete
 * (func_8000262C - dma_signal)
 *
 * Posts a message to indicate DMA transfer is complete.
 */
void dma_signal(void) {
    osJamMesg(&gDmaMessageQueue, NULL, 0);
}

/**
 * LZSS decompress with DMA synchronization
 * (func_80002660 - lzss_decompress)
 *
 * Waits for DMA ready, performs LZSS decompression,
 * then signals completion.
 *
 * @param src Source compressed data
 * @param dst Destination buffer
 * @return Decompressed size, or 0 if DMA not ready
 */
s32 lzss_decompress(void *src, void *dst) {
    s32 result;

    if (dma_wait(1) == 0) {
        return 0;
    }

    result = lzss_decode(src, dst);
    dma_signal();

    return result;
}

/**
 * Inflate decompress with DMA synchronization
 * (func_800026C0 - inflate_decompress)
 *
 * Waits for DMA ready, performs inflate decompression,
 * then signals completion.
 *
 * @param src Source compressed data
 * @param dst Destination buffer
 * @param use_heap Whether to use heap allocation
 * @return Decompressed size, or 0 if DMA not ready
 */
s32 inflate_decompress(void *src, void *dst, s32 use_heap) {
    s32 result;

    if (dma_wait(1) == 0) {
        return 0;
    }

    result = inflate_entry(src, dst, use_heap);
    dma_signal();

    return result;
}
