# Rush 2049 N64 Input and Controller System

This document provides comprehensive documentation of the input handling and controller system in the N64 version of San Francisco Rush 2049.

## Table of Contents

1. [Overview](#overview)
2. [Controller Button Mapping](#controller-button-mapping)
3. [Analog Stick Handling](#analog-stick-handling)
4. [Input Processing Pipeline](#input-processing-pipeline)
5. [Rumble/Vibration Support](#rumblevibration-support)
6. [Multiplayer Controller Assignment](#multiplayer-controller-assignment)
7. [Button Remapping System](#button-remapping-system)
8. [Menu Navigation Input](#menu-navigation-input)
9. [Comparison to Arcade Controls](#comparison-to-arcade-controls)
10. [Technical Implementation Details](#technical-implementation-details)

---

## Overview

Rush 2049 on N64 uses the standard N64 controller with full support for:
- Analog stick steering with dead zone handling
- Digital button inputs for throttle, brake, and actions
- Rumble Pak support for force feedback
- Up to 4 players in split-screen multiplayer
- Customizable button remapping

### Source Files

| File | Purpose |
|------|---------|
| `src/game/game.c` | Main input handling and controller processing |
| `src/libultra/os_cont.c` | Low-level controller reading (libultra) |
| `src/libultra/os_motor.c` | Rumble Pak motor control |

---

## Controller Button Mapping

### N64 Controller Structure (OSContPad)

The N64 controller data is read via libultra's `osContGetReadData()` and stored in the following format:

```c
/* Controller structure from osContPad */
struct OSContPad {
    u16 buttons;      /* Button bit mask */
    s8  stick_x;      /* Analog stick X (-128 to 127) */
    s8  stick_y;      /* Analog stick Y (-128 to 127) */
    u8  errno;        /* Error code */
};
```

### Button Bit Masks

| Button | Hex Mask | Description |
|--------|----------|-------------|
| A | `0x8000` | Primary action/Accelerate |
| B | `0x4000` | Secondary action/Brake |
| Z | `0x2000` | Z Trigger/Handbrake |
| Start | `0x1000` | Pause/Confirm |
| D-Up | `0x0800` | D-Pad Up |
| D-Down | `0x0400` | D-Pad Down |
| D-Left | `0x0200` | D-Pad Left |
| D-Right | `0x0100` | D-Pad Right |
| L | `0x0020` | Left shoulder/Shift down |
| R | `0x0010` | Right shoulder/Shift up |
| C-Up | `0x0008` | C-Button Up/View change |
| C-Down | `0x0004` | C-Button Down/Wing deploy |
| C-Left | `0x0002` | C-Button Left |
| C-Right | `0x0001` | C-Button Right |

### Default Racing Controls

| Action | Default Button | Player Structure Offset |
|--------|----------------|------------------------|
| Steering | Analog Stick X | `0xE0` (f32, -1.0 to 1.0) |
| Throttle | A Button | `0xE4` (f32, 0.0 to 1.0) |
| Brake | B Button | `0xE8` (f32, 0.0 to 1.0) |
| Handbrake | Z Trigger | `0xEC` (s32, 0 or 1) |
| Shift Up | R Shoulder | `0xF0` (s32) |
| Shift Down | L Shoulder | `0xF4` (s32) |
| View Change | C-Up | `0xF8` (s32) |
| Wing Deploy | C-Down | `0xFC` (s32) |
| Pause | Start | `0x100` (s32) |

### Analog Throttle/Brake (Alternate)

The game also supports analog throttle and brake via the analog stick Y-axis:
- **Stick Y > 10**: Partial throttle = `(stickY - 10) / 117.0`
- **Stick Y < -10**: Partial brake = `(-stickY - 10) / 117.0`

This provides analog control when not using the A/B buttons.

---

## Analog Stick Handling

### Dead Zone Implementation

The analog stick has a dead zone of approximately +/- 10 units to prevent drift:

```c
/* Dead zone implementation from player_read_input() @ line 13019 */
#define STICK_DEADZONE 10
#define STICK_RANGE 117.0f  /* 127 - 10 = usable range */

if (stickX > STICK_DEADZONE) {
    *steering = (f32)(stickX - STICK_DEADZONE) / STICK_RANGE;
} else if (stickX < -STICK_DEADZONE) {
    *steering = (f32)(stickX + STICK_DEADZONE) / STICK_RANGE;
} else {
    *steering = 0.0f;
}

/* Clamp to -1.0 to 1.0 range */
if (*steering > 1.0f) *steering = 1.0f;
if (*steering < -1.0f) *steering = -1.0f;
```

### Steering Sensitivity

The steering is converted to normalized values:
- **Output Range**: -1.0 (full left) to +1.0 (full right)
- **Dead Zone**: 10 units (approximately 8% of range)
- **Effective Range**: 117 units after dead zone subtraction

### Menu Navigation Thresholds

For menu navigation, the stick has higher thresholds to prevent accidental inputs:
- **Up/Down Threshold**: Stick Y > 50 or < -50
- **Left/Right Threshold**: Stick X > 50 or < -50

---

## Input Processing Pipeline

### Input Callback System

The input system uses a callback table for modular input processing:

```c
/* Global input state variables */
extern s32 controller_buttons;      /* Current button state */
extern s32 buttons_pressed;         /* Newly pressed buttons this frame */
extern s32 buttons_state;           /* Previous frame button state */
extern s32 input_repeat_delay;      /* Repeat delay counter */
extern s8 input_initialized_flag;   /* Input system ready */

/* Input callback table (10 entries) */
extern void (*input_callback_table[])(void);
extern void (*input_callback_end)(void);
```

### Input Processing Flow

1. **Controller Read**: `controller_read()` polls hardware
2. **Input Dispatch**: `input_handlers_process()` iterates callback table
3. **Player Input**: `player_read_input()` maps buttons to player controls
4. **State Update**: `player_update_state()` processes state machine

### Input Debouncing and Repeat

The menu system implements input repeat for held directions:

```c
/* Repeat logic for menu navigation */
#define INITIAL_DELAY 15  /* Frames before repeat starts */
#define REPEAT_RATE 10    /* Frames between repeats */

if (repeatDelay == 0 || repeatDelay > INITIAL_DELAY) {
    result = DIRECTION;
    if (repeatDelay == 0) {
        repeat_counter = 1;
    }
}
if (repeatDelay > 0) {
    repeat_counter = repeatDelay + 1;
}
```

---

## Rumble/Vibration Support

### Rumble Pak Detection

The game uses libultra's motor functions to control the Rumble Pak:

```c
extern void osMotorStart(void *pak);
extern void osMotorStop(void *pak);
extern void *motor_pak_handles[];  /* Per-controller Rumble Pak handles */
```

### Vibration Settings

```c
extern s32 vibration_setting;  /* 0 = off, 1 = on */
extern s32 rumble_state;       /* Current rumble state */
extern s32 rumble_timer;       /* Rumble duration timer */
extern s32 motor_vibe_state;   /* State machine for patterns */
```

### Menu Vibration Test Pattern

The options menu includes a vibration test with a distinctive pattern:

```c
/* Vibration pattern: short-short-long */
void menu_vibration_test(void) {
    /* Pattern states:
     * State 0: First short pulse (10 frames)
     * State 1: First pause (5 frames)
     * State 2: Second short pulse (10 frames)
     * State 3: Second pause (5 frames)
     * State 4: Long pulse (30 frames)
     * State 5: Complete
     */
    switch (vibePattern) {
        case 0:  /* First short pulse */
            if (vibeTimer >= 10) {
                osMotorStop(&motor_pak_handles[controller]);
                motor_vibe_state = 1;
            }
            break;
        /* ... additional states ... */
        case 4:  /* Long pulse */
            if (vibeTimer >= 30) {
                osMotorStop(&motor_pak_handles[controller]);
                motor_vibe_state = 5;
                vibeState = 0;  /* Done */
            }
            break;
    }
}
```

### Rumble Timing (60 FPS)

| Pattern Element | Duration (frames) | Duration (ms) |
|----------------|-------------------|---------------|
| Short pulse | 10 | ~167 |
| Pause | 5 | ~83 |
| Long pulse | 30 | ~500 |
| Total test | 60 | ~1000 |

---

## Multiplayer Controller Assignment

### Controller Slot System

Rush 2049 supports up to 4 players with dynamic join/leave:

```c
extern s32 controller_input_1;  /* Controller 1 raw input */
extern s32 controller_input_2;  /* Controller 2 raw input */
extern s32 controller_input_3;  /* Controller 3 raw input */
extern s32 gNumPlayers;         /* Active player count */
extern s32 mp_player_count;     /* Multiplayer player count */
```

### Player Join Flow

From `multiplayer_lobby()`:

```c
/* Check each player slot */
for (i = 0; i < numPlayers; i++) {
    s32 buttons = controllerInput[i] & 0xFFFF;

    /* Check for Start button to join */
    if (buttons & 0x1000) {  /* Start */
        if (effect_slots[i] == 0) {
            effect_slots[i] = 1;  /* Player joining */
        } else if (effect_slots[i] == 1) {
            effect_slots[i] = 2;  /* Player ready */
        }
    }

    /* Check for B button to leave */
    if (buttons & 0x4000) {  /* B */
        if (effect_slots[i] == 2) {
            effect_slots[i] = 1;  /* Unready */
        } else if (effect_slots[i] == 1) {
            effect_slots[i] = 0;  /* Leave */
        }
    }
}

/* Start game when 2+ players are ready */
if (joinedPlayers >= 2 && allReady) {
    /* 1.5 second countdown before game start */
    if (countdown_timer >= 90) {
        mp_player_count = joinedPlayers;
        gstate_byte = GSTATE_PREPLAY;
    }
}
```

### Player States

| State | Value | Description |
|-------|-------|-------------|
| Not Joined | 0 | Controller slot empty |
| Joined | 1 | Player joined, not ready |
| Ready | 2 | Player ready to race |

### Split-Screen Configuration

The game configures split-screen based on player count via `camera_split_screen_config(numPlayers)`.

---

## Button Remapping System

### Remappable Actions

From `menu_controller_remap()`:

| Index | Action | Default Button |
|-------|--------|----------------|
| 0 | ACCELERATE | A |
| 1 | BRAKE | B |
| 2 | HANDBRAKE | Z |
| 3 | LOOK BACK | R |
| 4 | CHANGE VIEW | L |
| 5 | HORN | C-UP |
| 6 | RESET CAR | C-DOWN |
| 7 | PAUSE | START |

### Remap State Machine

```c
extern s32 button_remap_state;   /* 0=normal, 1=waiting for press */
extern s32 remap_action_idx;     /* Action being remapped */
extern s32 remap_timeout_frames; /* Timeout counter (60 frames) */
extern s32 control_mapping[8];   /* Button ID for each action */
```

### Remap Flow

1. User selects action to remap
2. State changes to "waiting for button"
3. 60-frame (1 second) timeout starts
4. First button pressed is assigned
5. State returns to normal navigation

### Button ID Lookup

```c
/* Button detection priority */
if (buttons & 0x8000) buttonId = 0;       /* A */
else if (buttons & 0x4000) buttonId = 1;  /* B */
else if (buttons & 0x2000) buttonId = 2;  /* Z */
else if (buttons & 0x1000) buttonId = 3;  /* START */
else if (buttons & 0x0020) buttonId = 4;  /* L */
else if (buttons & 0x0010) buttonId = 5;  /* R */
else if (buttons & 0x0008) buttonId = 6;  /* C-UP */
else if (buttons & 0x0004) buttonId = 7;  /* C-DOWN */
else if (buttons & 0x0002) buttonId = 8;  /* C-LEFT */
else if (buttons & 0x0001) buttonId = 9;  /* C-RIGHT */
```

---

## Menu Navigation Input

### Menu Input Results

The `controller_get_input()` function returns these results:

| Result | Meaning |
|--------|---------|
| 0 | No input |
| 1 | Select (A) |
| 2 | Back (B) |
| 3 | Start pressed |
| 4 | Up (D-pad or stick) |
| 5 | Down (D-pad or stick) |
| 6 | Left (D-pad or stick) |
| 7 | Right (D-pad or stick) |
| 8 | L trigger (page up) |
| 9 | R trigger (page down) |

### Menu Sounds

| Sound ID | Trigger |
|----------|---------|
| 10 | Menu select/confirm |
| 11 | Menu back/cancel |
| 12 | Menu navigate |
| 13 | Locked item/error |

---

## Comparison to Arcade Controls

### Arcade Cabinet Hardware

The arcade version (Rush The Rock source reference) uses:
- **Force-feedback steering wheel** with motor control
- **Gas pedal** (analog, 0-4096)
- **Brake pedal** (analog, 0-4096)
- **Gear shifter** (discrete positions)
- **Multiple cabinet buttons** (Start, Black, Blue)

### Arcade Control Structure

```c
/* From reference/repos/rushtherock/game/modeldat.h */
typedef struct fastintyp {
    S32 lasttime;      /* Time of last iteration */
    F32 modeltime;     /* Model iteration time */
    S16 wheel;         /* Wheel position -0x800 to 0x800 */
    S16 swvel;         /* Steering wheel velocity */
    S16 modelrun;      /* Stall model if zero */
} fastintyp;

typedef struct mainintyp {
    S16 clutch;        /* Clutch 0 to 1.0 */
    S16 brake;         /* Brake 0 to 1.0 */
    S16 throttle;      /* Throttle 0 to 1.0 */
    S16 gear;          /* Current gear */
    S16 autotrans;     /* Auto transmission */
    S16 ignition;      /* Ignition state */
    /* ... */
} mainintyp;
```

### Key Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Steering | 270-degree wheel with force feedback | Analog stick |
| Throttle | Analog pedal | A button (digital) or stick Y |
| Brake | Analog pedal | B button (digital) or stick Y |
| Clutch | Available | Not available |
| Gear Shift | Physical shifter | L/R buttons |
| Force Feedback | Motor-driven wheel | Rumble Pak |

### Arcade Force Feedback

The arcade uses sophisticated steering wheel force feedback:

```c
/* From reference/repos/rushtherock/game/fswheel.c */
void ModelSteerForce(void) {
    /* Apply friction */
    swfrtor = (S16)(((S16)(adjswpos - swfrblk) * swfrk)/0x10);

    /* Set force based on model, friction, servo torque, damping */
    tottorque = torque + swfrtor - (swvel * gSWdamp);

    /* Apply force scale based on user setting */
    gFscale = force_frac[gFeedbackLevel][gForceOffset];
    scaletorque = (scaletorque * gFscale)/100;
}
```

The N64 version uses simpler Rumble Pak patterns as a substitute.

### Control Scaling Constant

Both versions use a control scaling constant:
- **CTLSCALE**: `1.0/0x1000` (1/4096)
- This normalizes raw input values to 0.0-1.0 range

---

## Technical Implementation Details

### Global Input Variables

| Variable | Address | Purpose |
|----------|---------|---------|
| `controller_buttons` | External | Current button state |
| `buttons_pressed` | External | Newly pressed this frame |
| `buttons_state` | External | Previous frame state |
| `input_repeat_delay` | External | Auto-repeat counter |
| `input_initialized_flag` | External | System ready flag |
| `gPlayerInputEnabled` | 0x801147C8 | Player input enabled |

### Player Input Structure Offsets

| Offset | Type | Description |
|--------|------|-------------|
| 0xE0 | f32 | Steering input (-1.0 to 1.0) |
| 0xE4 | f32 | Throttle input (0.0 to 1.0) |
| 0xE8 | f32 | Brake input (0.0 to 1.0) |
| 0xEC | s32 | Handbrake (0 or 1) |
| 0xF0 | s32 | Shift up pressed |
| 0xF4 | s32 | Shift down pressed |
| 0xF8 | s32 | View change pressed |
| 0xFC | s32 | Wing deploy |
| 0x100 | s32 | Pause pressed |
| 0x104 | s32 | Player state |
| 0x108 | s32 | State timer |
| 0x10C | s32 | Respawn countdown |
| 0x110 | s32 | Invincibility timer |

### Player States

| Value | State |
|-------|-------|
| 0 | Racing normally |
| 1 | Crashed/wrecked |
| 2 | Respawning |
| 3 | Finished race |
| 4 | In menu/paused |

### Initialization Functions

| Function | Purpose |
|----------|---------|
| `controller_init()` | Initialize controller hardware |
| `controller_read()` | Poll controller state |
| `input_system_init()` | Initialize input callback system |
| `input_handlers_process()` | Process input callback table |
| `player_read_input()` | Map controller to player controls |

---

## Save Data for Controls

Controller configuration is saved to Controller Pak:

```c
/* From save_game_to_pak() */
saveData[offset++] = vibration_setting;   /* Vibration on/off */
saveData[offset++] = controller_config;   /* Controller config preset */
```

The button remapping data is stored in the `control_mapping[]` array and persisted across sessions.

---

## See Also

- [Multiplayer System](multiplayer_system.md) - Split-screen and multiplayer details
- [Car Physics](car_physics.md) - How input affects vehicle behavior
- [Menu System](menu_system.md) - Menu navigation implementation
- [Save System](save_system.md) - Controller Pak save/load

---

*Generated from Rush 2049 N64 decompilation project analysis.*
