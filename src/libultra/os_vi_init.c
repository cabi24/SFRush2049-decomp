/**
 * @file os_vi_init.c
 * @brief VI (Video Interface) initialization
 *
 * Decompiled from asm/us/D140.s
 */

#include "types.h"

/* VI registers */
#define VI_STATUS_REG  (*(vu32 *)0xA4400000)
#define VI_CURRENT_REG (*(vu32 *)0xA4400010)

/* External data */
extern s32 osTvType;            /* TV type: 0=PAL, 1=NTSC, 2=MPAL */
extern void *__osViCurr;        /* Current VI context (being displayed) */
extern void *__osViNext;        /* Next VI context (pending swap) */
extern void *__osViCurrPtr;     /* Pointer to current VI context */
extern void *__osViNextPtr;     /* Pointer to next VI context */
extern void *osViModePalLan1;   /* PAL mode timing table */
extern void *osViModeNtscLan1;  /* NTSC mode timing table */
extern void *osViModeMpalLan1;  /* MPAL mode timing table */

/* External functions */
extern void bzero(void *ptr, s32 size);
extern void __osViSwapContext(void);

/**
 * Initialize Video Interface
 * (func_8000C540 - osViInit)
 *
 * Sets up the VI hardware and thread structures.
 */
void osViInit(void) {
    void *modeTable;
    void *viState;

    /* Clear VI structures */
    bzero(&__osViCurr, 0x60);

    /* Set up pointers */
    __osViCurrPtr = &__osViCurr;
    __osViNextPtr = &__osViNext;

    /* Set initial state */
    *(u16 *)((u8 *)&__osViNext + 2) = 1;
    *(u16 *)((u8 *)__osViCurrPtr + 2) = 1;

    /* Set null next pointer */
    *(u32 *)((u8 *)__osViNextPtr + 4) = 0x80000000;
    *(u32 *)((u8 *)__osViCurrPtr + 4) = 0x80000000;

    /* Select mode table based on TV type */
    if (osTvType == 0) {
        /* PAL */
        modeTable = &osViModePalLan1;
    } else if (osTvType == 2) {
        /* MPAL */
        modeTable = &osViModeNtscLan1;
    } else {
        /* NTSC (default) */
        modeTable = &osViModeMpalLan1;
    }

    /* Store mode table pointer */
    viState = __osViNextPtr;
    *(void **)((u8 *)viState + 8) = modeTable;

    /* Set state flags */
    *(u16 *)__osViNextPtr = 0x20;

    /* Copy timing value from mode table */
    viState = __osViNextPtr;
    *(u32 *)((u8 *)viState + 0xC) = *(u32 *)((u8 *)(*(void **)((u8 *)viState + 8)) + 4);

    /* Wait for VI to be ready (current line < 11) */
    while (VI_CURRENT_REG >= 11) {
        ;
    }

    /* Disable VI */
    VI_STATUS_REG = 0;

    /* Set up VI interrupt */
    __osViSwapContext();
}
