/**
 * @file display.c
 * @brief Display and viewport management
 *
 * Decompiled from asm/us/21F0.s
 * Handles viewport setup, display list management, and screen updates
 */

#include "types.h"

/* External OS functions */
extern s32 osSetGlobalIntMask(s32 mask);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void osWritebackDCache(void *dst, s32 size);
extern void osSetThreadPri(void *thread, s32 pri);
extern void osSetIntMask(s32 mask);
extern void osViSetSpecialFeatures(s32 val);

/* Game display functions */
extern void viewport_scale(f32 a0, f32 a1);  /* 0x800A7508 */

/* TV type from libultra */
extern s32 osTvType;

/* Display state structures */
typedef struct {
    s32 x_start;        /* 0x1C: packed (top << 16) | bottom */
    s32 time;           /* 0x20: frame time/counter */
    s32 width;          /* 0x28: width * 2 */
    s32 y_pos;          /* 0x2C: y position (low 16 bits) */
    s32 y_start;        /* 0x30: packed (top << 16) | bottom */
    s32 scale_x;        /* 0x3C: x scale factor */
    s32 flags;          /* 0x04: flags (bit 6 = interlace?) */
    s32 height;         /* 0x08: height */
    s32 y_offset;       /* 0x40: y offset (low 16 bits) */
    s32 scissor;        /* 0x44: scissor rect */
} DisplayState;

/* Global display state */
extern DisplayState *gCurrentDisplayState;     /* Current display state pointer */
extern DisplayState *gReferenceDisplayState;   /* Reference display state */
extern DisplayState gDisplayStateBuffer[2];    /* Double-buffered display states (0x50 bytes each) */

/* Viewport parameters */
extern s16 gViewportXStart;    /* Viewport X start */
extern s16 gViewportYStart;    /* Viewport Y start */
extern s16 gViewportXEnd;      /* Viewport X end base */
extern s16 gViewportYEnd;      /* Viewport Y end base */
extern s16 gViewportXOffset;   /* Viewport X offset */
extern s16 gViewportYOffset;   /* Viewport Y offset */
extern s16 gScreenWidth;       /* Screen width */
extern s16 gScreenHeight;      /* Screen height */
extern s16 gCurrentXOffset;    /* Current X offset */
extern s16 gCurrentYOffset;    /* Current Y offset */
extern s16 gDisplayMode;       /* Display mode (0=low res, 1=high res) */
extern s16 gPlayerViewCount;   /* Player count/mode */
extern s16 gDisplayUpdateCounter;   /* Display update counter */

/* Scaling factors */
extern f32 gScaleFactorX;      /* X scale factor */
extern f32 gScaleFactorY;      /* Y scale factor */
extern f32 gScaleMultiplierX;  /* X multiplier constant */
extern f32 gScaleMultiplierY;  /* Y multiplier constant */
extern f32 gScaleLowResY;      /* Low res Y scale */
extern f32 gScaleHighResY;     /* High res Y scale */

/* Viewport bounds tables */
extern s16 gViewportXOffsetTable[];   /* X offset table (indexed by mode * 4) */
extern s16 gViewportYOffsetTable[];   /* Y offset table */
extern s16 gViewportXBoundsTable[];   /* X bounds table */
extern s16 gViewportYBoundsTable[];   /* Y bounds table */
extern s16 gViewportBoundsTable[];    /* Viewport bounds (16 bytes per mode) */

/* Display mode table */
extern DisplayState gDisplayModeTable[];    /* Display mode parameters (0x50 bytes each) */

/**
 * Update display list with viewport interpolation
 * (0x800015F0 - display_update)
 *
 * Main display update function that:
 * - Calculates viewport scaling ratios
 * - Updates display list buffers
 * - Handles interlaced mode adjustments
 */
void display_update(void) {
    s32 x_start, x_end;
    s32 y_start, y_end;
    f32 x_ratio, y_ratio;
    s32 saved_mask;
    s32 y_offset;
    f32 old_time, new_time;
    f32 interp_factor;
    s32 temp;

    /* Early exit if no display state */
    if (gReferenceDisplayState == NULL) {
        return;
    }

    /* Calculate viewport bounds */
    x_start = gViewportXStart + gCurrentXOffset;
    x_end = gViewportXEnd + gCurrentXOffset + gViewportXOffset;
    y_start = gViewportYStart + gCurrentYOffset;
    y_end = gViewportYEnd + gCurrentYOffset + gViewportYOffset;

    /* Calculate scaling ratios */
    x_ratio = (f32)(gViewportXEnd - gViewportXStart) / (f32)(x_end - x_start);
    y_ratio = (f32)(gViewportYEnd - gViewportYStart) / (f32)(y_end - y_start);

    /* Disable interrupts */
    saved_mask = osSetGlobalIntMask(1);

    /* Toggle between double-buffered display states */
    if (gCurrentDisplayState == &gDisplayStateBuffer[0]) {
        gCurrentDisplayState = &gDisplayStateBuffer[1];
    } else {
        gCurrentDisplayState = &gDisplayStateBuffer[0];
    }

    /* Copy current state from reference */
    memcpy(gCurrentDisplayState, gReferenceDisplayState, 0x50);

    /* Calculate interpolation factor based on frame times */
    old_time = (f32)(u32)gCurrentDisplayState->time;
    new_time = (f32)(u32)gReferenceDisplayState->time;
    interp_factor = 1.0f - (old_time / new_time);

    /* Adjust x_start based on interpolation */
    temp = (s32)((f32)x_start - (interp_factor * 5.0f));
    gCurrentDisplayState->x_start = (temp << 16) | x_end;

    /* Calculate and store time offset */
    y_offset = (s32)((f32)gScreenWidth * x_ratio * gScaleMultiplierX);
    gCurrentDisplayState->time = y_offset;

    /* Store Y bounds */
    gCurrentDisplayState->y_start = ((y_start & 0x3FF) << 16) | (y_end & 0x3FF);

    /* Store width (doubled) */
    gCurrentDisplayState->width = gScreenWidth * 2;

    /* Calculate height with aspect ratio adjustment */
    y_offset = (s32)((f32)gScreenHeight * y_ratio * gScaleMultiplierY + 16.5f);

    /* Check if scale changed */
    if (gReferenceDisplayState->scale_x != gReferenceDisplayState->width) {
        gCurrentDisplayState->scale_x = gScreenWidth * 4;
        gCurrentDisplayState->height = gScreenWidth * 2;
        y_offset >>= 1;
    } else {
        gCurrentDisplayState->scale_x = gScreenWidth * 2;
        gCurrentDisplayState->height = gScreenWidth;
    }

    /* Handle interlaced mode (bit 6 of flags) */
    if (gCurrentDisplayState->flags & 0x40) {
        gCurrentDisplayState->scissor = (((y_start + 2) & 0x3FF) << 16) | ((y_end + 2) & 0x3FF);
    } else {
        gCurrentDisplayState->scissor = ((y_start & 0x3FF) << 16) | (y_end & 0x3FF);
    }

    /* Align y_offset to 16-byte boundary */
    y_offset &= 0xFFF0;

    /* Store y_offset in position fields */
    gCurrentDisplayState->y_pos = (gCurrentDisplayState->y_pos & 0xFFFF0000) | y_offset;
    gCurrentDisplayState->y_offset = (gCurrentDisplayState->y_offset & 0xFFFF0000) | y_offset;

    /* Signal display update ready */
    gDisplayUpdateCounter = 2;

    /* Flush display state to memory */
    osWritebackDCache(gCurrentDisplayState, 0x50);

    /* Restore interrupts */
    osSetGlobalIntMask(saved_mask);
}

/**
 * Set up viewport parameters
 * (0x80001B44 - viewport_setup)
 *
 * @param mode Display mode (0=low res, 1=high res)
 * @param player Player/viewport index
 * @param width Screen width
 * @param height Screen height
 */
void viewport_setup(s32 mode, s32 player, s32 width, s32 height) {
    DisplayState *state;

    /* Set scaling factors based on mode */
    if (mode == 0) {
        gScaleFactorX = 50.0f;  /* Low res X scale */
        gScaleFactorY = gScaleLowResY;
    } else {
        gScaleFactorX = 60.0f;  /* High res X scale */
        gScaleFactorY = gScaleHighResY;
    }

    /* Store mode parameters */
    gDisplayMode = (s16)mode;
    gPlayerViewCount = (s16)player;

    /* Select display state from table (0x50 bytes per entry) */
    gReferenceDisplayState = &gDisplayModeTable[player * 5];  /* player * 0x50 / sizeof */

    /* Extract viewport bounds from state */
    gViewportXStart = (gReferenceDisplayState->x_start >> 16) & 0x3FF;
    gViewportXEnd = gReferenceDisplayState->x_start & 0x3FF;
    gViewportYStart = (gReferenceDisplayState->y_start >> 16) & 0x3FF;
    gViewportYEnd = gReferenceDisplayState->y_start & 0x3FF;

    /* Set initial offsets if no current state */
    if (gCurrentDisplayState == NULL) {
        gViewportXOffset = gViewportXOffsetTable[mode * 4];
        gViewportYOffset = gViewportYOffsetTable[mode * 4];
        gCurrentXOffset = gViewportXBoundsTable[mode * 4];
        gCurrentYOffset = gViewportYBoundsTable[mode * 4];
    } else {
        /* Scale existing offsets */
        viewport_scale((f32)width / (f32)gScreenWidth,
                       (f32)height / (f32)gScreenHeight);
    }

    /* Store dimensions */
    gScreenWidth = (s16)width;
    gScreenHeight = (s16)height;

    /* Update display */
    display_update();
}

/**
 * Process pending display updates
 * (0x80001D60)
 */
void display_process(void) {
    if (gDisplayUpdateCounter == 0) {
        return;
    }

    gDisplayUpdateCounter--;

    if (gDisplayUpdateCounter == 0) {
        /* Check if positions match */
        if ((gReferenceDisplayState->y_pos & 0xFFFF) == (gCurrentDisplayState->y_pos & 0xFFFF)) {
            osSetIntMask(0);
        }

        osSetThreadPri(gCurrentDisplayState, 0);
        osViSetSpecialFeatures(0xAA);
    }
}

/**
 * Get TV type offset for frame timing
 * (0x80001DFC)
 *
 * @return Frame offset based on TV type (0=PAL, 14=NTSC-J, 28=NTSC-U)
 */
s32 get_tv_offset(void) {
    if (osTvType == 1) {
        /* PAL */
        return 0;
    } else if (osTvType == 0) {
        /* NTSC-J */
        return 14;
    } else {
        /* NTSC-U or other */
        return 28;
    }
}

/**
 * Set thread priority for display state
 * (0x80001E58)
 */
void display_set_priority(void) {
    osSetThreadPri(gReferenceDisplayState, 0);
}

/**
 * Get current viewport position
 * (0x80001E84 - get_viewport_pos)
 *
 * @param out_x Output X position
 * @param out_y Output Y position
 */
void get_viewport_pos(s32 *out_x, s32 *out_y) {
    *out_x = gViewportXBoundsTable[gDisplayMode * 4];
    *out_y = gViewportYBoundsTable[gDisplayMode * 4];
}

/**
 * Get viewport offset from base position
 * (0x80001ECC - get_viewport_offset)
 *
 * @param out_x Output X offset
 * @param out_y Output Y offset
 */
void get_viewport_offset(s32 *out_x, s32 *out_y) {
    *out_x = gCurrentXOffset - gViewportXBoundsTable[gDisplayMode * 4];
    *out_y = gCurrentYOffset - gViewportYBoundsTable[gDisplayMode * 4];
}

/**
 * Update viewport with bounds checking
 * (0x80001F2C - update_viewport)
 *
 * @param delta_x X position delta
 * @param delta_y Y position delta
 */
void update_viewport(s32 delta_x, s32 delta_y) {
    s32 x_pos, y_pos;
    s32 x_end, y_end;
    s16 *bounds;

    if (gCurrentDisplayState == NULL) {
        return;
    }

    /* Calculate new positions */
    x_pos = gViewportXStart + delta_x;
    y_pos = gViewportYStart + delta_y;
    x_end = gViewportXEnd + delta_x + gViewportXOffset;
    y_end = gViewportYEnd + delta_y + gViewportYOffset;

    /* Get bounds table for current mode (16 bytes per entry) */
    bounds = &gViewportBoundsTable[gDisplayMode * 8];

    /* Check X bounds and update if within range */
    if (bounds[0] < x_pos && x_pos < bounds[1] &&
        bounds[2] < x_end && x_end < bounds[3]) {
        gCurrentXOffset = gViewportXBoundsTable[gDisplayMode * 4] + delta_x;
    }

    /* Check Y bounds and update if within range */
    bounds = &gViewportBoundsTable[gDisplayMode * 8];
    if (bounds[4] < y_pos && y_pos < bounds[5] &&
        bounds[6] < y_end && y_end < bounds[7]) {
        gCurrentYOffset = gViewportYBoundsTable[gDisplayMode * 4] + delta_y;
    }

    /* Refresh display */
    display_update();
}
