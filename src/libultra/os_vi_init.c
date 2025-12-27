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
extern s32 osTvType;         /* TV type: 0=PAL, 1=NTSC, 2=MPAL */
extern void *D_8002C400;     /* VI thread structure */
extern void *D_8002C430;     /* VI state structure */
extern void *D_8002C460;     /* VI queue pointer */
extern void *D_8002C464;     /* Current VI state */
extern void *D_8002C4E0;     /* PAL mode table */
extern void *D_8002C530;     /* NTSC mode table */
extern void *D_8002C580;     /* MPAL mode table */

/* External functions */
extern void func_80008590(void *ptr, s32 size);  /* memset/bzero */
extern void func_8000C670(void);  /* VI interrupt setup */

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
    func_80008590(&D_8002C400, 0x60);

    /* Set up pointers */
    D_8002C460 = &D_8002C400;
    D_8002C464 = &D_8002C430;

    /* Set initial state */
    *(u16 *)((u8 *)&D_8002C430 + 2) = 1;
    *(u16 *)((u8 *)D_8002C460 + 2) = 1;

    /* Set null next pointer */
    *(u32 *)((u8 *)D_8002C464 + 4) = 0x80000000;
    *(u32 *)((u8 *)D_8002C460 + 4) = 0x80000000;

    /* Select mode table based on TV type */
    if (osTvType == 0) {
        /* PAL */
        modeTable = &D_8002C4E0;
    } else if (osTvType == 2) {
        /* MPAL */
        modeTable = &D_8002C530;
    } else {
        /* NTSC (default) */
        modeTable = &D_8002C580;
    }

    /* Store mode table pointer */
    viState = D_8002C464;
    *(void **)((u8 *)viState + 8) = modeTable;

    /* Set state flags */
    *(u16 *)D_8002C464 = 0x20;

    /* Copy timing value from mode table */
    viState = D_8002C464;
    *(u32 *)((u8 *)viState + 0xC) = *(u32 *)((u8 *)(*(void **)((u8 *)viState + 8)) + 4);

    /* Wait for VI to be ready (current line < 11) */
    while (VI_CURRENT_REG >= 11) {
        ;
    }

    /* Disable VI */
    VI_STATUS_REG = 0;

    /* Set up VI interrupt */
    func_8000C670();
}
