/**
 * input.c - Controller input handling
 * Decompiled from func_80094D20, func_80094D68, func_80094DB0, func_80094E00
 */

#include "types.h"

// Controller pad structure (32 bytes each)
// Located at 0x80140BF0
typedef struct {
    /* 0x00 */ u16 buttons_pressed;
    /* 0x02 */ u16 buttons_held;
    /* 0x04 */ s8 stick_x;
    /* 0x05 */ s8 stick_y;
    /* 0x06 */ u8 unk_06[8];
    /* 0x0E */ u16 repeat_timer;
    /* 0x10 */ u8 unk_10[4];
    /* 0x14 */ u8 repeat_delay;
    /* 0x15 */ u8 flags;
    /* 0x16 */ s8 mode;
    /* 0x17 */ u8 unk_17;
    /* 0x18 */ s16 analog_min;
    /* 0x1A */ s16 analog_max;
    /* 0x1C */ s16 deadzone_min;
    /* 0x1E */ s16 deadzone_max;
} PadEntry;  // size = 0x20 (32 bytes)

// Flag definitions
#define PAD_FLAG_ENABLED   0x80
#define PAD_FLAG_SECONDARY 0x04

// External pad array
extern PadEntry pad_array[];  // 0x80140BF0

/**
 * Input_SetPadEnabledFlag - Enable or disable a controller pad
 * Address: 0x80094D20
 * @param pad_index: Which controller (0-3)
 * @param enable: 0 to disable, non-zero to enable
 */
void Input_SetPadEnabledFlag(s32 pad_index, s32 enable) {
    PadEntry *pad = &pad_array[pad_index];

    if (enable != 0) {
        pad->flags |= PAD_FLAG_ENABLED;
    } else {
        pad->flags &= ~PAD_FLAG_ENABLED;
    }
}

/**
 * Input_SetPadSecondaryFlag - Set or clear secondary flag (rumble/lock)
 * Address: 0x80094D68
 * @param pad_index: Which controller (0-3)
 * @param enable: 0 to clear, non-zero to set
 */
void Input_SetPadSecondaryFlag(s32 pad_index, s32 enable) {
    PadEntry *pad = &pad_array[pad_index];

    if (enable != 0) {
        pad->flags |= PAD_FLAG_SECONDARY;
    } else {
        pad->flags &= ~PAD_FLAG_SECONDARY;
    }
}

/**
 * Input_ConfigRepeatTimers - Configure button repeat/debounce parameters
 * Address: 0x80094DB0
 * @param pad_index: Which controller (0-3)
 * @param has_repeat: Non-zero enables repeat mode
 * @param repeat_delay: Delay before repeat starts (-1 to keep current)
 * @param repeat_timer: Timer value for repeat rate (-1 to keep current)
 */
void Input_ConfigRepeatTimers(s32 pad_index, s32 has_repeat, s32 repeat_delay, u32 repeat_timer) {
    PadEntry *pad = &pad_array[pad_index];
    u16 timer_val = (u16)repeat_timer;

    // Mode 2 means repeat is already configured, skip
    if (pad->mode == 2) {
        return;
    }

    // Set repeat mode (0 or 1) based on has_repeat
    pad->mode = (has_repeat != 0) ? 1 : 0;

    // Set repeat delay if non-negative
    if (repeat_delay >= 0) {
        pad->repeat_delay = (u8)repeat_delay;
    }

    // Set timer value if not 0xFFFF
    if (timer_val != 0xFFFF) {
        pad->repeat_timer = timer_val;
    }
}

/**
 * Input_SetAnalogBounds - Configure analog stick bounds/deadzone
 * Address: 0x80094E00
 * @param pad_index: Which controller (0-3)
 * @param analog_min: Minimum analog value (-128 to 127)
 * @param analog_max: Maximum analog value (-128 to 127)
 * @param deadzone_min: Deadzone minimum threshold
 * @param deadzone_max: Deadzone maximum threshold
 */
void Input_SetAnalogBounds(s32 pad_index, s16 analog_min, s16 analog_max,
                           s16 deadzone_min, s16 deadzone_max) {
    PadEntry *pad = &pad_array[pad_index];

    // Only set if analog_min >= 0 (negative means skip)
    if (analog_min >= 0) {
        pad->analog_min = analog_min;
        pad->analog_max = analog_max;
        pad->deadzone_min = deadzone_min;
        pad->deadzone_max = deadzone_max;
    }
}
