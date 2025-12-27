/**
 * @file os_ai.c
 * @brief Audio Interface (AI) functions
 *
 * Decompiled from asm/us/CA70.s
 * Contains functions for audio DMA and sample rate configuration
 */

#include "types.h"

/* Hardware register addresses (AI Base: 0xA4500000) */
#define AI_DRAM_ADDR_REG    (*(vu32 *)0xA4500000)  /* AI_DRAM_ADDR: DRAM address for DMA */
#define AI_LEN_REG          (*(vu32 *)0xA4500004)  /* AI_LEN: Length of DMA transfer */
#define AI_CONTROL_REG      (*(vu32 *)0xA4500008)  /* AI_CONTROL: AI control register */
#define AI_STATUS_REG       (*(vu32 *)0xA450000C)  /* AI_STATUS: AI status register */
#define AI_DACRATE_REG      (*(vu32 *)0xA4500010)  /* AI_DACRATE: DAC sample rate divider */
#define AI_BITRATE_REG      (*(vu32 *)0xA4500014)  /* AI_BITRATE: Bit rate divider */

/* DAC rate calculation constants */
#define AI_MIN_DAC_RATE     0x84    /* Minimum valid DAC rate divider */
#define AI_MAX_BIT_RATE     16      /* Maximum bit rate value */

/* External data */
extern u8 __osAiDmaQueueHead;       /* Audio double buffer flag */
extern u32 osClockRate;             /* System clock frequency (46.875 MHz) */

/* External functions */
extern s32 __osAiDeviceBusy(void);           /* Check if AI DMA is full */
extern u32 osVirtualToPhysical(void *addr);  /* Convert virtual to physical */

/**
 * Set audio DMA buffer
 * (func_8000BE70 - osAiSetNextBuffer)
 *
 * Queues an audio buffer for DMA to the AI.
 * Uses double buffering with 0x2000 byte alignment.
 *
 * @param addr Address of audio buffer
 * @param size Size of audio data in bytes
 * @return 0 on success, -1 if AI DMA queue is full
 */
s32 osAiSetNextBuffer(void *addr, u32 size) {
    void *bufAddr;
    u32 physAddr;

    /* Check if AI can accept another buffer */
    if (__osAiDeviceBusy() != 0) {
        return -1;
    }

    bufAddr = addr;

    /* Handle double buffering */
    if (__osAiDmaQueueHead != 0) {
        /* Use alternate buffer (0x2000 bytes before) */
        bufAddr = (u8 *)addr - 0x2000;
    }

    /* Check alignment and toggle buffer flag */
    if (((u32)addr + size & 0x1FFF) == 0) {
        __osAiDmaQueueHead = 1;
    } else {
        __osAiDmaQueueHead = 0;
    }

    /* Convert to physical address and set registers */
    physAddr = osVirtualToPhysical(bufAddr);
    AI_DRAM_ADDR_REG = physAddr;
    AI_LEN_REG = size;

    return 0;
}

/**
 * Set audio sample rate
 * (func_8000BF00 - osAiSetFrequency)
 *
 * Configures the AI DAC rate and bit rate for the specified
 * output frequency.
 *
 * Formula: dacRate = osClockRate / frequency
 * The actual frequency returned is osClockRate / dacRate
 *
 * @param frequency Desired output frequency in Hz
 * @return Actual output frequency, or -1 on error
 */
s32 osAiSetFrequency(u32 frequency) {
    f32 fSysFreq;
    f32 fFreq;
    f32 dacRate;
    s32 dRate;
    s32 bitRate;

    /* Get system clock frequency (46.875 MHz) */
    fSysFreq = (f32)(s32)osClockRate;
    fFreq = (f32)frequency;

    /* Calculate DAC rate: (sysFreq / freq) + 0.5 */
    dacRate = fSysFreq / fFreq + 0.5f;

    /* Convert to integer with saturation */
    dRate = (s32)dacRate;
    if (dRate < 0) {
        dRate = -1;
    }

    /* Validate range (minimum AI_MIN_DAC_RATE) */
    if (dRate < AI_MIN_DAC_RATE) {
        return -1;
    }

    /* Calculate bit rate: dacRate / 66 */
    bitRate = dRate / 66;
    if (bitRate > AI_MAX_BIT_RATE) {
        bitRate = AI_MAX_BIT_RATE;
    }

    /* Set hardware registers */
    AI_DACRATE_REG = dRate - 1;
    AI_BITRATE_REG = bitRate - 1;

    /* Return actual frequency */
    return (s32)osClockRate / dRate;
}
