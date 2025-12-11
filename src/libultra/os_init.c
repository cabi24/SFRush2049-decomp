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

/* External system variables */
extern s32 osTvType;        /* TV type (0=PAL, 1=NTSC, 2=MPAL) */
extern s32 osResetType;     /* Reset type (0=cold, 1=NMI) */
extern u8 osAppNMIBuffer[64];  /* NMI save buffer */

/* External data */
extern s32 D_800367D0;      /* Initialization state flag */
extern u32 D_8002C360;      /* System parameter high */
extern u32 D_8002C364;      /* System parameter low */
extern u32 D_8002C368;      /* System clock frequency */

/* PI timing parameters for DOM1 (cartridge) */
extern u8 D_800367E4;       /* Domain 1 flags */
extern u8 D_800367E5;       /* Domain 1 latency */
extern u8 D_800367E6;       /* Domain 1 page size */
extern u8 D_800367E7;       /* Domain 1 release */
extern u8 D_800367E8;       /* Domain 1 pulse width */

/* PI timing parameters for DOM2 (expansion) */
extern u8 D_8003685C;       /* Domain 2 flags */
extern u8 D_8003685D;       /* Domain 2 latency */
extern u8 D_8003685E;       /* Domain 2 page size */
extern u8 D_8003685F;       /* Domain 2 release */
extern u8 D_80036860;       /* Domain 2 pulse width */

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
    /* Save Domain 1 (cartridge) parameters */
    D_800367E4 = 7;  /* Flags */
    D_800367E5 = (u8)PI_BSD_DOM1_LAT_REG;
    D_800367E8 = (u8)PI_BSD_DOM1_PWD_REG;
    D_800367E6 = (u8)PI_BSD_DOM1_PGS_REG;
    D_800367E7 = (u8)PI_BSD_DOM1_RLS_REG;

    /* Save Domain 2 (expansion) parameters */
    D_8003685C = 7;  /* Flags */
    D_8003685D = (u8)PI_BSD_DOM2_LAT_REG;
    D_80036860 = (u8)PI_BSD_DOM2_PWD_REG;
    D_8003685E = (u8)PI_BSD_DOM2_PGS_REG;
    D_8003685F = (u8)PI_BSD_DOM2_RLS_REG;
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
    u32 sp24, sp20;

    D_800367D0 = 1;  /* Mark as initializing */

    /* Set status register - enable coprocessors */
    __osSetSR(__osGetSR() | 0x20000000);

    /* Set cause register */
    __osSetCause(0x01000800);

    /* Wait for PIF to be ready */
    while (osPiRawReadIo(0x1FC007FC, &pifData) != 0) {
        /* spin */
    }

    /* Write PIF initialization command */
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

    /* Calculate timing parameters */
    sp20 = __osSetTimerIntr(D_8002C360 | ((u64)D_8002C364 << 32));
    sp24 = (u32)sp20;
    sp20 = __osSetCompare(sp20);
    D_8002C360 = (u32)sp20;

    /* Clear NMI buffer on cold boot */
    if (osResetType == 0) {
        bzero(osAppNMIBuffer, 0x40);
    }

    /* Set system clock based on TV type */
    if (osTvType == 0) {
        /* PAL */
        D_8002C368 = OS_CLOCK_RATE_MPAL;
    } else if (osTvType == 2) {
        /* MPAL */
        D_8002C368 = OS_CLOCK_RATE_PAL;
    } else {
        /* NTSC (default) */
        D_8002C368 = OS_CLOCK_RATE_NTSC;
    }

    /* Check for hardware error condition */
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
