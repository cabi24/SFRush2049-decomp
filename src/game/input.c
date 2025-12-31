/**
 * input.c - Controller input handling
 * Decompiled from 0x80094D20, 0x80094D68, 0x80094DB0, 0x80094E00
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

/******* ARCADE-COMPATIBLE CONTROL FUNCTIONS *******/
/* Based on arcade: controls.c */

/* Control scale - converts 16-bit input to 0-1 range */
#define CTLSCALE        (1.0f / 32767.0f)
#define STEER_SCALE     (1.0f / 127.0f)   /* Analog stick to radians */
#define STEER_MAX_ANGLE (0.5f)            /* Max steer angle in radians */

/* N64 button definitions */
#define BTN_A           0x8000
#define BTN_B           0x4000
#define BTN_Z           0x2000
#define BTN_START       0x1000
#define BTN_DU          0x0800
#define BTN_DD          0x0400
#define BTN_DL          0x0200
#define BTN_DR          0x0100
#define BTN_L           0x0020
#define BTN_R           0x0010
#define BTN_CU          0x0008
#define BTN_CD          0x0004
#define BTN_CL          0x0002
#define BTN_CR          0x0001

/* External game state */
extern u8 gstate;
extern s32 end_game_flag;
extern s32 coast_flag;

/* Game state constants */
#define GS_COUNTDOWN    5

/**
 * ControlInput - Per-player control state
 * Maps N64 controller to arcade-style inputs
 */
typedef struct ControlInput {
    f32     throttle;       /* 0-1 throttle position */
    f32     brake;          /* 0-1 brake position */
    f32     steerangle;     /* Steering angle in radians */
    f32     clutch;         /* 0-1 clutch position */
    s32     gear;           /* Current gear (0=neutral, 1-6) */
    s32     autotrans;      /* Auto transmission enabled */
    s32     view_change;    /* View change button pressed */
    s32     horn;           /* Horn button pressed */
    s32     pause;          /* Pause button pressed */
} ControlInput;

/* Control state for each player */
ControlInput control_input[4];

/**
 * controls_read_pad - Read N64 controller and convert to control inputs
 * Based on arcade: controls.c:controls()
 *
 * @param pad_index Which controller (0-3)
 * @param ctrl Output control state
 */
void controls_read_pad(s32 pad_index, ControlInput *ctrl) {
    PadEntry *pad;
    s32 stick_x;
    f32 brake_rolloff;

    if (pad_index < 0 || pad_index > 3) {
        return;
    }

    pad = &pad_array[pad_index];

    /* Skip if pad not enabled */
    if (!(pad->flags & PAD_FLAG_ENABLED)) {
        return;
    }

    /* Steering from analog stick X */
    stick_x = pad->stick_x;

    /* Apply deadzone */
    if (stick_x > pad->deadzone_min && stick_x < pad->deadzone_max) {
        stick_x = 0;
    }

    ctrl->steerangle = (f32)stick_x * STEER_SCALE * STEER_MAX_ANGLE;

    /* Throttle from A button or R trigger */
    if (pad->buttons_held & (BTN_A | BTN_R)) {
        ctrl->throttle = 1.0f;
    } else {
        ctrl->throttle = 0.0f;
    }

    /* Brake from B button or Z trigger */
    if (pad->buttons_held & (BTN_B | BTN_Z)) {
        ctrl->brake = 1.0f;
    } else {
        ctrl->brake = 0.0f;
    }

    /* View change from C buttons */
    ctrl->view_change = (pad->buttons_pressed & (BTN_CU | BTN_CD | BTN_CL | BTN_CR)) ? 1 : 0;

    /* Horn from L button */
    ctrl->horn = (pad->buttons_held & BTN_L) ? 1 : 0;

    /* Pause from Start */
    ctrl->pause = (pad->buttons_pressed & BTN_START) ? 1 : 0;

    /* Default to auto transmission */
    ctrl->autotrans = 1;
    ctrl->clutch = 1.0f;
    ctrl->gear = 1;

    /* During countdown, lock controls */
    if (gstate == GS_COUNTDOWN) {
        ctrl->gear = 0;
        ctrl->clutch = 1.0f;
        ctrl->brake = 1.0f;
        ctrl->throttle = 0.0f;
    }

    /* End of game braking */
    if (coast_flag || end_game_flag) {
        ctrl->brake = 1.0f;
        ctrl->throttle = 0.0f;
        ctrl->clutch = 1.0f;
    }
}

/**
 * controls_apply_to_physics - Apply control inputs to physics state
 *
 * @param ctrl Control input state
 * @param phys Physics state to update
 */
void controls_apply_to_physics(ControlInput *ctrl, void *phys_ptr) {
    /* Cast to CarPhysics - defined in physics.h */
    /* For now, directly update the fields */
    f32 *phys = (f32 *)phys_ptr;

    /* This would map to CarPhysics fields */
    /* phys->throttle = ctrl->throttle; */
    /* phys->brake = ctrl->brake; */
    /* phys->steerangle = ctrl->steerangle; */
}

/**
 * ReadGasAndBrake - Read gas and brake pedals
 * Based on arcade: mdrive.c:ReadGasAndBrake()
 *
 * On arcade, this reads physical pedals.
 * On N64, we map buttons to 0-1 range.
 */
void ReadGasAndBrake(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        controls_read_pad(i, &control_input[i]);
    }
}

/**
 * Input_GetStickX - Get normalized analog stick X value
 *
 * @param pad_index Which controller (0-3)
 * @return Normalized value -1.0 to 1.0
 */
f32 Input_GetStickX(s32 pad_index) {
    PadEntry *pad;
    s32 stick_x;

    if (pad_index < 0 || pad_index > 3) {
        return 0.0f;
    }

    pad = &pad_array[pad_index];
    stick_x = pad->stick_x;

    /* Apply deadzone */
    if (stick_x > pad->deadzone_min && stick_x < pad->deadzone_max) {
        return 0.0f;
    }

    return (f32)stick_x / 127.0f;
}

/**
 * Input_GetStickY - Get normalized analog stick Y value
 *
 * @param pad_index Which controller (0-3)
 * @return Normalized value -1.0 to 1.0
 */
f32 Input_GetStickY(s32 pad_index) {
    PadEntry *pad;

    if (pad_index < 0 || pad_index > 3) {
        return 0.0f;
    }

    pad = &pad_array[pad_index];
    return (f32)pad->stick_y / 127.0f;
}

/**
 * Input_IsButtonPressed - Check if button was just pressed this frame
 *
 * @param pad_index Which controller (0-3)
 * @param button Button mask to check
 * @return Non-zero if button was just pressed
 */
s32 Input_IsButtonPressed(s32 pad_index, u16 button) {
    if (pad_index < 0 || pad_index > 3) {
        return 0;
    }

    return (pad_array[pad_index].buttons_pressed & button) ? 1 : 0;
}

/**
 * Input_IsButtonHeld - Check if button is currently held
 *
 * @param pad_index Which controller (0-3)
 * @param button Button mask to check
 * @return Non-zero if button is held
 */
s32 Input_IsButtonHeld(s32 pad_index, u16 button) {
    if (pad_index < 0 || pad_index > 3) {
        return 0;
    }

    return (pad_array[pad_index].buttons_held & button) ? 1 : 0;
}
