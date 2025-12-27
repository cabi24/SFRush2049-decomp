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
extern void viewport_scale(f32 a0, f32 a1);  /* func_800A7508 */

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
extern DisplayState *D_8002B014;     /* Current display state pointer */
extern DisplayState *D_8002EBFC;     /* Reference display state */
extern DisplayState D_8002EC00[2];   /* Double-buffered display states (0x50 bytes each) */

/* Viewport parameters */
extern s16 D_8002ECA0;   /* Viewport X start */
extern s16 D_8002ECA2;   /* Viewport Y start */
extern s16 D_8002ECA4;   /* Viewport X end base */
extern s16 D_8002ECA6;   /* Viewport Y end base */
extern s16 D_8002ECA8;   /* Viewport X offset */
extern s16 D_8002ECAA;   /* Viewport Y offset */
extern s16 D_8002ECAC;   /* Screen width */
extern s16 D_8002ECAE;   /* Screen height */
extern s16 D_8002ECB0;   /* Current X offset */
extern s16 D_8002ECB2;   /* Current Y offset */
extern s16 D_8002EBF8;   /* Display mode (0=low res, 1=high res) */
extern s16 D_8002EBFA;   /* Player count/mode */
extern s16 D_8002B010;   /* Display update counter */

/* Scaling factors */
extern f32 D_8002AFB4;   /* X scale factor */
extern f32 D_8002AFB8;   /* Y scale factor */
extern f32 D_8002D4C0;   /* X multiplier constant */
extern f32 D_8002D4C4;   /* Y multiplier constant */
extern f32 D_8002D4C8;   /* Low res Y scale */
extern f32 D_8002D4CC;   /* High res Y scale */

/* Viewport bounds tables */
extern s16 D_8002AFF8[];   /* X offset table (indexed by mode * 4) */
extern s16 D_8002AFFA[];   /* Y offset table */
extern s16 D_8002AFFC[];   /* X bounds table */
extern s16 D_8002AFFE[];   /* Y bounds table */
extern s16 D_8002AFC8[];   /* Viewport bounds (16 bytes per mode) */

/* Display mode table */
extern DisplayState D_8002B1B0[];    /* Display mode parameters (0x50 bytes each) */

/**
 * Update display list with viewport interpolation
 * (func_800015F0 - display_update)
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
    if (D_8002EBFC == NULL) {
        return;
    }

    /* Calculate viewport bounds */
    x_start = D_8002ECA0 + D_8002ECB0;
    x_end = D_8002ECA4 + D_8002ECB0 + D_8002ECA8;
    y_start = D_8002ECA2 + D_8002ECB2;
    y_end = D_8002ECA6 + D_8002ECB2 + D_8002ECAA;

    /* Calculate scaling ratios */
    x_ratio = (f32)(D_8002ECA4 - D_8002ECA0) / (f32)(x_end - x_start);
    y_ratio = (f32)(D_8002ECA6 - D_8002ECA2) / (f32)(y_end - y_start);

    /* Disable interrupts */
    saved_mask = osSetGlobalIntMask(1);

    /* Toggle between double-buffered display states */
    if (D_8002B014 == &D_8002EC00[0]) {
        D_8002B014 = &D_8002EC00[1];
    } else {
        D_8002B014 = &D_8002EC00[0];
    }

    /* Copy current state from reference */
    memcpy(D_8002B014, D_8002EBFC, 0x50);

    /* Calculate interpolation factor based on frame times */
    old_time = (f32)(u32)D_8002B014->time;
    new_time = (f32)(u32)D_8002EBFC->time;
    interp_factor = 1.0f - (old_time / new_time);

    /* Adjust x_start based on interpolation */
    temp = (s32)((f32)x_start - (interp_factor * 5.0f));
    D_8002B014->x_start = (temp << 16) | x_end;

    /* Calculate and store time offset */
    y_offset = (s32)((f32)D_8002ECAC * x_ratio * D_8002D4C0);
    D_8002B014->time = y_offset;

    /* Store Y bounds */
    D_8002B014->y_start = ((y_start & 0x3FF) << 16) | (y_end & 0x3FF);

    /* Store width (doubled) */
    D_8002B014->width = D_8002ECAC * 2;

    /* Calculate height with aspect ratio adjustment */
    y_offset = (s32)((f32)D_8002ECAE * y_ratio * D_8002D4C4 + 16.5f);

    /* Check if scale changed */
    if (D_8002EBFC->scale_x != D_8002EBFC->width) {
        D_8002B014->scale_x = D_8002ECAC * 4;
        D_8002B014->height = D_8002ECAC * 2;
        y_offset >>= 1;
    } else {
        D_8002B014->scale_x = D_8002ECAC * 2;
        D_8002B014->height = D_8002ECAC;
    }

    /* Handle interlaced mode (bit 6 of flags) */
    if (D_8002B014->flags & 0x40) {
        D_8002B014->scissor = (((y_start + 2) & 0x3FF) << 16) | ((y_end + 2) & 0x3FF);
    } else {
        D_8002B014->scissor = ((y_start & 0x3FF) << 16) | (y_end & 0x3FF);
    }

    /* Align y_offset to 16-byte boundary */
    y_offset &= 0xFFF0;

    /* Store y_offset in position fields */
    D_8002B014->y_pos = (D_8002B014->y_pos & 0xFFFF0000) | y_offset;
    D_8002B014->y_offset = (D_8002B014->y_offset & 0xFFFF0000) | y_offset;

    /* Signal display update ready */
    D_8002B010 = 2;

    /* Flush display state to memory */
    osWritebackDCache(D_8002B014, 0x50);

    /* Restore interrupts */
    osSetGlobalIntMask(saved_mask);
}

/**
 * Set up viewport parameters
 * (func_80001B44 - viewport_setup)
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
        D_8002AFB4 = 50.0f;  /* Low res X scale */
        D_8002AFB8 = D_8002D4C8;
    } else {
        D_8002AFB4 = 60.0f;  /* High res X scale */
        D_8002AFB8 = D_8002D4CC;
    }

    /* Store mode parameters */
    D_8002EBF8 = (s16)mode;
    D_8002EBFA = (s16)player;

    /* Select display state from table (0x50 bytes per entry) */
    D_8002EBFC = &D_8002B1B0[player * 5];  /* player * 0x50 / sizeof */

    /* Extract viewport bounds from state */
    D_8002ECA0 = (D_8002EBFC->x_start >> 16) & 0x3FF;
    D_8002ECA4 = D_8002EBFC->x_start & 0x3FF;
    D_8002ECA2 = (D_8002EBFC->y_start >> 16) & 0x3FF;
    D_8002ECA6 = D_8002EBFC->y_start & 0x3FF;

    /* Set initial offsets if no current state */
    if (D_8002B014 == NULL) {
        D_8002ECA8 = D_8002AFF8[mode * 4];
        D_8002ECAA = D_8002AFFA[mode * 4];
        D_8002ECB0 = D_8002AFFC[mode * 4];
        D_8002ECB2 = D_8002AFFE[mode * 4];
    } else {
        /* Scale existing offsets */
        viewport_scale((f32)width / (f32)D_8002ECAC,
                       (f32)height / (f32)D_8002ECAE);
    }

    /* Store dimensions */
    D_8002ECAC = (s16)width;
    D_8002ECAE = (s16)height;

    /* Update display */
    display_update();
}

/**
 * Process pending display updates
 * (func_80001D60)
 */
void display_process(void) {
    if (D_8002B010 == 0) {
        return;
    }

    D_8002B010--;

    if (D_8002B010 == 0) {
        /* Check if positions match */
        if ((D_8002EBFC->y_pos & 0xFFFF) == (D_8002B014->y_pos & 0xFFFF)) {
            osSetIntMask(0);
        }

        osSetThreadPri(D_8002B014, 0);
        osViSetSpecialFeatures(0xAA);
    }
}

/**
 * Get TV type offset for frame timing
 * (func_80001DFC)
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
 * (func_80001E58)
 */
void display_set_priority(void) {
    osSetThreadPri(D_8002EBFC, 0);
}

/**
 * Get current viewport position
 * (func_80001E84 - get_viewport_pos)
 *
 * @param out_x Output X position
 * @param out_y Output Y position
 */
void get_viewport_pos(s32 *out_x, s32 *out_y) {
    *out_x = D_8002AFFC[D_8002EBF8 * 4];
    *out_y = D_8002AFFE[D_8002EBF8 * 4];
}

/**
 * Get viewport offset from base position
 * (func_80001ECC - get_viewport_offset)
 *
 * @param out_x Output X offset
 * @param out_y Output Y offset
 */
void get_viewport_offset(s32 *out_x, s32 *out_y) {
    *out_x = D_8002ECB0 - D_8002AFFC[D_8002EBF8 * 4];
    *out_y = D_8002ECB2 - D_8002AFFE[D_8002EBF8 * 4];
}

/**
 * Update viewport with bounds checking
 * (func_80001F2C - update_viewport)
 *
 * @param delta_x X position delta
 * @param delta_y Y position delta
 */
void update_viewport(s32 delta_x, s32 delta_y) {
    s32 x_pos, y_pos;
    s32 x_end, y_end;
    s16 *bounds;

    if (D_8002B014 == NULL) {
        return;
    }

    /* Calculate new positions */
    x_pos = D_8002ECA0 + delta_x;
    y_pos = D_8002ECA2 + delta_y;
    x_end = D_8002ECA4 + delta_x + D_8002ECA8;
    y_end = D_8002ECA6 + delta_y + D_8002ECAA;

    /* Get bounds table for current mode (16 bytes per entry) */
    bounds = &D_8002AFC8[D_8002EBF8 * 8];

    /* Check X bounds and update if within range */
    if (bounds[0] < x_pos && x_pos < bounds[1] &&
        bounds[2] < x_end && x_end < bounds[3]) {
        D_8002ECB0 = D_8002AFFC[D_8002EBF8 * 4] + delta_x;
    }

    /* Check Y bounds and update if within range */
    bounds = &D_8002AFC8[D_8002EBF8 * 8];
    if (bounds[4] < y_pos && y_pos < bounds[5] &&
        bounds[6] < y_end && y_end < bounds[7]) {
        D_8002ECB2 = D_8002AFFE[D_8002EBF8 * 4] + delta_y;
    }

    /* Refresh display */
    display_update();
}
