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

/*
 * ============================================================================
 * Arcade-compatible control system
 * Based on: reference/repos/rushtherock/game/controls.c, pots.c, modeldat.h
 * ============================================================================
 */

/* Control scale constants (arcade: modelcom.h) */
#define CTLSCALE        (1.0f / 4096.0f)    /* 1.0 for control inputs (0x1000 = 1.0) */
#define INVCTLSCALE     0x1000              /* Inverse control scale */
#define STEER_SCALE     (1.0f / 127.0f)     /* Analog stick to normalized (-1 to 1) */
#define STEER_MAX_ANGLE (0.5f)              /* Max steer angle in radians */

/* Gear constants (arcade: drivsym.h) */
#define MINGEAR         1                   /* Minimum gear for auto trans */
#define MAXGEAR         4                   /* Number of forward speeds */
#define GEAR_NEUTRAL    0
#define GEAR_REVERSE    (-1)

/* Pot types (arcade: controls.h) */
typedef enum {
    ADC_ACCEL,
    ADC_BRAKE,
    ADC_CLUTCH,
    ADC_WHEEL,
    NUM_POTS
} POT_TYPES;

/* Wheel range (arcade: controls.h) */
#ifndef WHEEL_RANGE
#define WHEEL_RANGE         (0xc00 >> 2)    /* Range of steering wheel choices */
#endif
#define RAW_WHEEL_RANGE     2048

/* Switch definitions (arcade: controls.h) */
#define SW_EDGES            0x0FFFFFFFF
#define SW_LEVELS           0x0
#define SW_RADIO            0x8             /* Radio button */
#define SW_REVERSE          0x00000100      /* Reverse gear */
#define SW_FIRST            0x00000200      /* Shifter gear 1 */
#define SW_SECOND           0x00000400      /* Shifter gear 2 */
#define SW_THIRD            0x00000800      /* Shifter gear 3 */
#define SW_FOURTH           0x00001000      /* Shifter gear 4 */

/*
 * FastInput - Fast input data (sent every model iteration)
 * Based on arcade: modeldat.h:fastintyp
 */
typedef struct FastInput {
    s32     lasttime;       /* Time of last model iteration (microsec / 10) */
    f32     modeltime;      /* Model iteration time (seconds) */
    s16     wheel;          /* Wheel position -1.0 to 1.0 scaled */
    s16     swvel;          /* Steering wheel velocity */
    s16     modelrun;       /* Stall model if zero */
} FastInput;

/*
 * MainInput - Main input data (sent less frequently)
 * Based on arcade: modeldat.h:mainintyp
 */
typedef struct MainInput {
    s16     clutch;         /* Clutch position 0 to 1.0 scaled */
    s16     brake;          /* Brake position 0 to 1.0 scaled */
    s16     throttle;       /* Throttle position 0 to 1.0 scaled */
    s16     gear;           /* Current gear (0=neutral, -1=reverse, 1-4=forward) */
    s16     autotrans;      /* 1 if automatic trans, 0 manual */
    s16     ignition;       /* 1 if ignition on, 0 off, 2 end-game */
    s16     startermotor;   /* Non-zero if starter engaged */
    s16     crashthreshold; /* Crash threshold, tens of pounds */
} MainInput;

/*
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
extern ControlInput control_input[4];

/* External game state (arcade: globals.h) - gstate defined in game.h */
/* extern u8 gstate; -- use GState from game.h instead */
extern s32 end_game_flag;
extern s32 coast_flag;

/* Game state constants */
#define GS_COUNTDOWN    5

/*
 * Arcade-compatible control function prototypes
 * Based on: controls.c, pots.c
 */

/* Initialize controls for a model (arcade: controls.c:initcontrols) */
void initcontrols(void *model_ptr);

/* Process controls for a model (arcade: controls.c:controls) */
void controls(void *model_ptr);

/* Read N64 controller and convert to control inputs */
void controls_read_pad(s32 pad_index, ControlInput *ctrl);

/* Apply control inputs to physics state */
void controls_apply_to_physics(ControlInput *ctrl, void *phys_ptr);

/* Read gas and brake pedals (arcade: pots.c:ReadGasAndBrake) */
void ReadGasAndBrake(void);

/* Get normalized analog stick values */
f32 Input_GetStickX(s32 pad_index);
f32 Input_GetStickY(s32 pad_index);

/* Button state queries */
s32 Input_IsButtonPressed(s32 pad_index, u16 button);
s32 Input_IsButtonHeld(s32 pad_index, u16 button);

#endif /* INPUT_H */
