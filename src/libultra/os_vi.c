/**
 * @file os_vi.c
 * @brief N64 OS Video Interface functions
 *
 * Decompiled from asm/us/7940.s, asm/us/79A0.s, asm/us/8920.s
 */

#include "types.h"

/* External OS functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32);

/* VI context pointers */
extern void *__osViCurr;
extern void *__osViNext;

/**
 * Get the current framebuffer address
 * @return Pointer to current framebuffer
 */
void *osViGetCurrentFramebuffer(void) {
    s32 savedMask;
    void *fb;

    savedMask = __osDisableInt();
    /* Access framebuffer at offset 0 in VI context struct */
    fb = *(void **)__osViCurr;
    __osRestoreInt(savedMask);
    return fb;
}

/**
 * Set the next framebuffer to display
 * @param frameBufPtr Pointer to framebuffer
 */
void osViSwapBuffer(void *frameBufPtr) {
    s32 savedMask;

    savedMask = __osDisableInt();
    /* Store framebuffer at offset 0 in VI next context struct */
    *(void **)__osViNext = frameBufPtr;
    __osRestoreInt(savedMask);
}

/* VI context structure */
extern void *D_8002C464;  /* System VI context */

/**
 * Get the configured framebuffer address
 * (func_800077D0 - osViGetFramebuffer)
 *
 * Returns the framebuffer pointer from the VI context.
 * This differs from osViGetCurrentFramebuffer which returns
 * the currently active framebuffer being displayed.
 *
 * @return Pointer to framebuffer
 */
void *osViGetFramebuffer(void) {
    s32 savedMask;
    void *fb;

    savedMask = __osDisableInt();
    fb = *(void **)((u8 *)D_8002C464 + 4);
    __osRestoreInt(savedMask);
    return fb;
}

/**
 * Set the VI mode
 * (func_80007590 - osViSetMode)
 *
 * Sets the video mode for the display.
 *
 * @param mode Pointer to OSViMode structure
 */
void osViSetMode(void *mode) {
    s32 savedMask;
    void *ctx;

    savedMask = __osDisableInt();

    ctx = D_8002C464;
    /* Store mode in context offset 0x04 */
    *(void **)((u8 *)ctx + 4) = mode;

    ctx = D_8002C464;
    /* Set mode change flag (bit 4) */
    *(u16 *)ctx = *(u16 *)ctx | 0x10;

    __osRestoreInt(savedMask);
}

/**
 * Enable or disable black screen
 * (func_80006DA0 - osViBlack)
 *
 * When enabled, the VI outputs black regardless of framebuffer content.
 *
 * @param black Non-zero to enable black screen, 0 to disable
 */
void osViBlack(s32 black) {
    s32 savedMask;

    savedMask = __osDisableInt();
    if (black) {
        *(u16 *)D_8002C464 |= 0x20;
    } else {
        *(u16 *)D_8002C464 &= ~0x20;
    }
    __osRestoreInt(savedMask);
}

/* VI special features flags */
#define OS_VI_GAMMA_ON          0x0001
#define OS_VI_GAMMA_OFF         0x0002
#define OS_VI_GAMMA_DITHER_ON   0x0004
#define OS_VI_GAMMA_DITHER_OFF  0x0008
#define OS_VI_DIVOT_ON          0x0010
#define OS_VI_DIVOT_OFF         0x0020
#define OS_VI_DITHER_FILTER_ON  0x0040
#define OS_VI_DITHER_FILTER_OFF 0x0080

/* External function to read VI status */
extern u32 func_8000D2A0(void);  /* Read VI_STATUS_REG */

/**
 * Set VI special features
 * (func_80007D20 - osViSetSpecialFeatures)
 *
 * Enables or disables special video interface features:
 * - Gamma correction
 * - Gamma dithering
 * - Divot filtering (removes 1-pixel gaps)
 * - Dither filtering
 *
 * @param features Bitmask of features to enable/disable
 */
void osViSetSpecialFeatures(u32 features) {
    s32 savedMask;
    u32 status;
    void *ctx;

    savedMask = __osDisableInt();

    status = func_8000D2A0();

    /* Check if VI is ready (bit 7 set = busy) */
    if (status & 0x80) {
        ctx = D_8002C464;

        /* Gamma on/off (bit 3 in features register) */
        if (features & OS_VI_GAMMA_ON) {
            *(u32 *)((u8 *)ctx + 0x0C) |= 0x08;
        }
        if (features & OS_VI_GAMMA_OFF) {
            *(u32 *)((u8 *)ctx + 0x0C) &= ~0x02;
        }

        /* Gamma dithering (bit 2) */
        if (features & OS_VI_GAMMA_DITHER_ON) {
            *(u32 *)((u8 *)ctx + 0x0C) |= 0x04;
        }
        if (features & OS_VI_GAMMA_DITHER_OFF) {
            *(u32 *)((u8 *)ctx + 0x0C) &= ~0x04;
        }

        /* Divot (bit 4) */
        if (features & OS_VI_DIVOT_ON) {
            *(u32 *)((u8 *)ctx + 0x0C) |= 0x10;
        }
        if (features & OS_VI_DIVOT_OFF) {
            *(u32 *)((u8 *)ctx + 0x0C) &= ~0x10;
        }

        /* Dither filter (bit 16) */
        if (features & OS_VI_DITHER_FILTER_ON) {
            *(u32 *)((u8 *)ctx + 0x0C) |= 0x10000;
            *(u32 *)((u8 *)ctx + 0x0C) &= ~0x300;
        }
        if (features & OS_VI_DITHER_FILTER_OFF) {
            void *mode;
            *(u32 *)((u8 *)ctx + 0x0C) &= ~0x10000;
            /* Copy filter mode from mode structure */
            mode = *(void **)((u8 *)ctx + 0x08);
            *(u32 *)((u8 *)ctx + 0x0C) |= (*(u32 *)((u8 *)mode + 0x04) & 0x300);
        }
    }

    /* Set update flag (bit 3) */
    *(u16 *)D_8002C464 |= 0x08;

    __osRestoreInt(savedMask);
}

/**
 * Set VI swap buffer callback
 * (func_80006ED0 - osViSetSwapBuffer)
 *
 * Configures the VI retrace callback settings.
 * This is called during scheduler setup to register
 * the swap buffer completion message.
 *
 * @param mq Message queue pointer for notifications
 * @param msg Message value to send on buffer swap
 * @param numFields Number of fields (1=single, 2=double)
 */
void osViSetSwapBuffer(void *mq, s32 msg, s16 numFields) {
    s32 savedMask;

    savedMask = __osDisableInt();

    /* Store settings in VI context */
    *(void **)((u8 *)D_8002C464 + 0x10) = mq;
    *(s32 *)((u8 *)D_8002C464 + 0x14) = msg;
    *(s16 *)((u8 *)D_8002C464 + 0x02) = numFields;

    __osRestoreInt(savedMask);
}
