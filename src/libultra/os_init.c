/**
 * @file os_init.c
 * @brief N64 OS initialization functions
 *
 * Decompiled from asm/us/8A80.s
 * Contains system initialization and hardware setup
 */

#include "types.h"

/* Hardware register addresses */
#define AI_CONTROL_REG      (*(vu32 *)0xA4500008)
#define AI_DACRATE_REG      (*(vu32 *)0xA4500010)
#define AI_BITRATE_REG      (*(vu32 *)0xA4500014)

#define PI_BSD_DOM1_LAT_REG (*(vu32 *)0xA4600014)
#define PI_BSD_DOM1_PWD_REG (*(vu32 *)0xA4600018)
#define PI_BSD_DOM1_PGS_REG (*(vu32 *)0xA460001C)
#define PI_BSD_DOM1_RLS_REG (*(vu32 *)0xA4600020)
#define PI_BSD_DOM2_LAT_REG (*(vu32 *)0xA4600024)
#define PI_BSD_DOM2_PWD_REG (*(vu32 *)0xA4600028)
#define PI_BSD_DOM2_PGS_REG (*(vu32 *)0xA460002C)
#define PI_BSD_DOM2_RLS_REG (*(vu32 *)0xA4600030)

/* System clock frequencies */
#define OS_CLOCK_RATE_NTSC  0x02E6D354   /* 48,681,812 Hz */
#define OS_CLOCK_RATE_PAL   0x02E6025C   /* 48,628,316 Hz */
#define OS_CLOCK_RATE_MPAL  0x02F5B2D2   /* 49,656,530 Hz */

/* External system variables (set by IPL3/boot) */
extern s32 osTvType;                /* TV type (0=PAL, 1=NTSC, 2=MPAL) */
extern s32 osResetType;             /* Reset type (0=cold, 1=NMI) */
extern u8 osAppNMIBuffer[64];       /* NMI save buffer */

/* External data */
extern s32 __osInitialized;         /* OS initialization state flag */
extern u32 __osTimerCountHi;        /* Timer count (high 32 bits) */
extern u32 __osTimerCountLo;        /* Timer count (low 32 bits) */
extern u32 osClockRate;             /* System clock frequency (Hz) */

/* PI timing parameters for DOM1 (cartridge ROM) */
extern u8 __osPiDom1Flags;          /* Domain 1 configuration flags */
extern u8 __osPiDom1Latency;        /* Domain 1 latency */
extern u8 __osPiDom1PageSize;       /* Domain 1 page size */
extern u8 __osPiDom1Release;        /* Domain 1 release duration */
extern u8 __osPiDom1Pulse;          /* Domain 1 pulse width */

/* PI timing parameters for DOM2 (SRAM/Flash) */
extern u8 __osPiDom2Flags;          /* Domain 2 configuration flags */
extern u8 __osPiDom2Latency;        /* Domain 2 latency */
extern u8 __osPiDom2PageSize;       /* Domain 2 page size */
extern u8 __osPiDom2Release;        /* Domain 2 release duration */
extern u8 __osPiDom2Pulse;          /* Domain 2 pulse width */

/* Exception vector code */
extern u32 __osExceptionVector[4];  /* Exception handler code (16 bytes) */

/* External functions */
extern void osInvalICache(void *addr, s32 size);
extern void osInvalDCache(void *addr, s32 size);
extern void bzero(void *addr, s32 size);
extern void __osSetSR(u32 sr);
extern u32 __osGetSR(void);
extern void __osSetCause(u32 cause);
extern s32 osPiRawReadIo(u32 devAddr, u32 *data);
extern s32 osPiRawWriteIo(u32 devAddr, u32 data);
extern void __osViInit(void);
extern void __osSiInit(void);
extern u64 __osSetTimerIntr(u64 time);
extern u64 __osSetCompare(u64 time);
extern u32 __osSiRawReadIo(void);

/**
 * Save PI timing parameters
 * (func_80008128 - __osInitialize_common)
 *
 * Saves the current PI BSD (bus speed) timing parameters for both
 * cartridge and expansion domains.
 */
static void __osSavePiTiming(void) {
    /* Save Domain 1 (cartridge ROM) parameters */
    __osPiDom1Flags = 7;
    __osPiDom1Latency = (u8)PI_BSD_DOM1_LAT_REG;
    __osPiDom1Pulse = (u8)PI_BSD_DOM1_PWD_REG;
    __osPiDom1PageSize = (u8)PI_BSD_DOM1_PGS_REG;
    __osPiDom1Release = (u8)PI_BSD_DOM1_RLS_REG;

    /* Save Domain 2 (SRAM/Flash) parameters */
    __osPiDom2Flags = 7;
    __osPiDom2Latency = (u8)PI_BSD_DOM2_LAT_REG;
    __osPiDom2Pulse = (u8)PI_BSD_DOM2_PWD_REG;
    __osPiDom2PageSize = (u8)PI_BSD_DOM2_PGS_REG;
    __osPiDom2Release = (u8)PI_BSD_DOM2_RLS_REG;
}

/**
 * Initialize N64 operating system
 * (func_80007E80 - osInitialize)
 *
 * Performs complete system initialization:
 * - Sets up CPU registers (SR, Cause)
 * - Installs exception vectors
 * - Initializes PI timing
 * - Sets up VI and SI subsystems
 * - Configures system clock based on TV type
 * - Initializes audio interface
 */
void osInitialize(void) {
    u32 pifData;
    u32 timerVal;

    __osInitialized = 1;  /* Mark OS as initializing */

    /* Set status register - enable coprocessors (CU1 for FPU) */
    __osSetSR(__osGetSR() | 0x20000000);

    /* Clear cause register except for timer interrupt pending */
    __osSetCause(0x01000800);

    /* Wait for PIF to be ready */
    while (osPiRawReadIo(0x1FC007FC, &pifData) != 0) {
        /* spin */
    }

    /* Write PIF initialization command (set bit 3) */
    while (osPiRawWriteIo(0x1FC007FC, pifData | 8) != 0) {
        /* spin */
    }

    /* Copy exception vector to RAM addresses */
    /* TLB refill handler at 0x80000000 */
    *(u32 *)0x80000000 = __osExceptionVector[0];
    *(u32 *)0x80000004 = __osExceptionVector[1];
    *(u32 *)0x80000008 = __osExceptionVector[2];
    *(u32 *)0x8000000C = __osExceptionVector[3];

    /* XTLB refill handler at 0x80000080 */
    *(u32 *)0x80000080 = __osExceptionVector[0];
    *(u32 *)0x80000084 = __osExceptionVector[1];
    *(u32 *)0x80000088 = __osExceptionVector[2];
    *(u32 *)0x8000008C = __osExceptionVector[3];

    /* Cache error handler at 0x80000100 */
    *(u32 *)0x80000100 = __osExceptionVector[0];
    *(u32 *)0x80000104 = __osExceptionVector[1];
    *(u32 *)0x80000108 = __osExceptionVector[2];
    *(u32 *)0x8000010C = __osExceptionVector[3];

    /* General exception handler at 0x80000180 */
    *(u32 *)0x80000180 = __osExceptionVector[0];
    *(u32 *)0x80000184 = __osExceptionVector[1];
    *(u32 *)0x80000188 = __osExceptionVector[2];
    *(u32 *)0x8000018C = __osExceptionVector[3];

    /* Invalidate caches for exception handlers */
    osInvalICache((void *)0x80000000, 0x190);
    osInvalDCache((void *)0x80000000, 0x190);

    /* Save PI timing parameters */
    __osSavePiTiming();

    /* Initialize VI subsystem */
    __osViInit();

    /* Initialize SI subsystem */
    __osSiInit();

    /* Initialize timer - combine hi/lo into 64-bit value */
    timerVal = __osSetTimerIntr(__osTimerCountHi | ((u64)__osTimerCountLo << 32));
    timerVal = __osSetCompare(timerVal);
    __osTimerCountHi = timerVal;

    /* Clear NMI buffer on cold boot */
    if (osResetType == 0) {
        bzero(osAppNMIBuffer, 0x40);
    }

    /* Set system clock based on TV type */
    if (osTvType == 0) {
        /* PAL */
        osClockRate = OS_CLOCK_RATE_MPAL;
    } else if (osTvType == 2) {
        /* MPAL */
        osClockRate = OS_CLOCK_RATE_PAL;
    } else {
        /* NTSC (default) */
        osClockRate = OS_CLOCK_RATE_NTSC;
    }

    /* Check for hardware error condition (PIF CIC error) */
    if (__osSiRawReadIo() & 0x1000) {
        /* Infinite loop on hardware error */
        for (;;) {
            /* hang */
        }
    }

    /* Initialize Audio Interface */
    AI_CONTROL_REG = 1;        /* Enable AI DMA */
    AI_DACRATE_REG = 0x3FFF;   /* Maximum DAC rate divider */
    AI_BITRATE_REG = 0xF;      /* Maximum bit rate */
}
