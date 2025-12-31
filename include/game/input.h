/**
 * input.h - Controller input handling definitions
 */

#ifndef INPUT_H
#define INPUT_H

#include "types.h"

/* Controller pad structure (32 bytes each) */
/* Located at 0x80140BF0 */
typedef struct {
    /* 0x00 */ u16 buttons_pressed;   /* Buttons just pressed this frame */
    /* 0x02 */ u16 buttons_held;      /* Buttons currently held */
    /* 0x04 */ s8 stick_x;            /* Analog stick X (-128 to 127) */
    /* 0x05 */ s8 stick_y;            /* Analog stick Y (-128 to 127) */
    /* 0x06 */ u8 unk_06[8];          /* Unknown padding */
    /* 0x0E */ u16 repeat_timer;      /* Auto-repeat timer value */
    /* 0x10 */ u8 unk_10[4];          /* Unknown padding */
    /* 0x14 */ u8 repeat_delay;       /* Auto-repeat delay */
    /* 0x15 */ u8 flags;              /* Controller flags */
    /* 0x16 */ s8 mode;               /* Input mode (0=normal, 1=repeat, 2=locked) */
    /* 0x17 */ u8 unk_17;             /* Unknown */
    /* 0x18 */ s16 analog_min;        /* Analog minimum bound */
    /* 0x1A */ s16 analog_max;        /* Analog maximum bound */
    /* 0x1C */ s16 deadzone_min;      /* Deadzone minimum */
    /* 0x1E */ s16 deadzone_max;      /* Deadzone maximum */
} PadEntry;  /* size = 0x20 (32 bytes) */

/* Flag definitions */
#define PAD_FLAG_ENABLED   0x80    /* Bit 7: Controller enabled */
#define PAD_FLAG_SECONDARY 0x04    /* Bit 2: Secondary mode (rumble/lock) */

/* N64 button definitions */
#define BUTTON_CRIGHT   0x0001
#define BUTTON_CLEFT    0x0002
#define BUTTON_CDOWN    0x0004
#define BUTTON_CUP      0x0008
#define BUTTON_R        0x0010
#define BUTTON_L        0x0020
#define BUTTON_DRIGHT   0x0100
#define BUTTON_DLEFT    0x0200
#define BUTTON_DDOWN    0x0400
#define BUTTON_DUP      0x0800
#define BUTTON_START    0x1000
#define BUTTON_Z        0x2000
#define BUTTON_B        0x4000
#define BUTTON_A        0x8000

/* External pad array (4 controllers max) */
extern PadEntry pad_array[4];  /* 0x80140BF0 */

/* Function prototypes */
void Input_SetPadEnabledFlag(s32 pad_index, s32 enable);
void Input_SetPadSecondaryFlag(s32 pad_index, s32 enable);
void Input_ConfigRepeatTimers(s32 pad_index, s32 has_repeat, s32 repeat_delay, u32 repeat_timer);
void Input_SetAnalogBounds(s32 pad_index, s16 analog_min, s16 analog_max,
                           s16 deadzone_min, s16 deadzone_max);

#endif /* INPUT_H */
