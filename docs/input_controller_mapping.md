# Input/Controller System Mapping

This document details the input and controller system for Rush 2049 N64, comparing it with the arcade source from Rush The Rock.

## Overview

The arcade version used physical steering wheel, gas/brake pedals, and cabinet buttons. The N64 version adapts these to the N64 controller's analog stick and buttons.

## N64 Controller Hardware

### OSContPad Structure

From `include/PR/os_cont.h`:

```c
typedef struct OSContPad {
    u16 button;         /* Button states */
    s8  stick_x;        /* Analog stick X (-80 to 80) */
    s8  stick_y;        /* Analog stick Y (-80 to 80) */
    u8  errno;          /* Error number */
} OSContPad;
```

### Button Bit Masks

| Button | Mask | Official Name | Description |
|--------|------|---------------|-------------|
| A | 0x8000 | `CONT_A` | Primary action (throttle) |
| B | 0x4000 | `CONT_B` | Secondary action (brake) |
| Z | 0x2000 | `CONT_G` | Trigger (handbrake) |
| Start | 0x1000 | `CONT_START` | Pause/menu |
| D-Up | 0x0800 | `CONT_UP` | Digital pad up |
| D-Down | 0x0400 | `CONT_DOWN` | Digital pad down |
| D-Left | 0x0200 | `CONT_LEFT` | Digital pad left |
| D-Right | 0x0100 | `CONT_RIGHT` | Digital pad right |
| L | 0x0020 | `CONT_L` | Left shoulder (shift down) |
| R | 0x0010 | `CONT_R` | Right shoulder (shift up) |
| C-Up | 0x0008 | `CONT_E` | Camera view |
| C-Down | 0x0004 | `CONT_D` | Wing deploy |
| C-Left | 0x0002 | `CONT_C` | Alternate function |
| C-Right | 0x0001 | `CONT_F` | Alternate function |

## Controller Button Mappings (Default)

### Racing Controls

| Function | N64 Input | Arcade Equivalent |
|----------|-----------|-------------------|
| Steering | Analog Stick X | Steering Wheel |
| Throttle | A button | Gas Pedal |
| Brake | B button | Brake Pedal |
| Handbrake/E-brake | Z trigger | N/A (arcade extension) |
| Shift Up | R trigger | Gear Shifter |
| Shift Down | L trigger | Gear Shifter |
| Camera View | C-Up | View Button |
| Wing Deploy | C-Down | N/A (N64 exclusive) |
| Pause | Start | Start Button |

### Alternate Analog Input

From `src/game/game.c` (player_read_input):

```c
/* Forward on stick = partial throttle */
if (stickY > 10) {
    *throttle = (f32)(stickY - 10) / 117.0f;
}

/* Back on stick = partial brake */
if (stickY < -10) {
    *brake = (f32)(-stickY - 10) / 117.0f;
}
```

This allows analog throttle/brake control via the stick's Y-axis when A/B buttons are not pressed.

## Analog Stick Handling

### Steering Processing

From `src/game/game.c`:

```c
/* Convert analog stick to steering (-1.0 to 1.0) */
/* Apply deadzone of ~10 */
if (stickX > 10) {
    *steering = (f32)(stickX - 10) / 117.0f;
} else if (stickX < -10) {
    *steering = (f32)(stickX + 10) / 117.0f;
} else {
    *steering = 0.0f;
}

/* Clamp steering */
if (*steering > 1.0f) *steering = 1.0f;
if (*steering < -1.0f) *steering = -1.0f;
```

### Deadzone Configuration

The input system supports configurable deadzones via `PadEntry` structure:

```c
typedef struct {
    /* ... */
    s16 analog_min;     /* 0x18 */
    s16 analog_max;     /* 0x1A */
    s16 deadzone_min;   /* 0x1C */
    s16 deadzone_max;   /* 0x1E */
} PadEntry;
```

Configured via `Input_SetAnalogBounds()` at address `0x80094E00`.

## Input Processing Pipeline

### High-Level Flow

```
1. osContStartReadData() - Begin async controller read
2. osContGetReadData()   - Get raw button/stick data
3. PadEntry update       - Process into internal format
4. controls_read_pad()   - Convert to ControlInput
5. player_read_input()   - Map to player state offsets
6. Physics update        - Apply inputs to car model
```

### Key Functions

| Address | Function | Description |
|---------|----------|-------------|
| 0x80094D20 | `Input_SetPadEnabledFlag` | Enable/disable controller |
| 0x80094D68 | `Input_SetPadSecondaryFlag` | Set rumble/lock flag |
| 0x80094DB0 | `Input_ConfigRepeatTimers` | Configure button repeat |
| 0x80094E00 | `Input_SetAnalogBounds` | Set analog deadzone |
| 0x80009854 | `osContGetReadData` | Get raw controller data |

### Internal Pad Structure

From `src/game/input.c`:

```c
typedef struct {
    /* 0x00 */ u16 buttons_pressed;   /* Just pressed this frame */
    /* 0x02 */ u16 buttons_held;      /* Currently held */
    /* 0x04 */ s8 stick_x;
    /* 0x05 */ s8 stick_y;
    /* 0x06 */ u8 unk_06[8];
    /* 0x0E */ u16 repeat_timer;
    /* 0x10 */ u8 unk_10[4];
    /* 0x14 */ u8 repeat_delay;
    /* 0x15 */ u8 flags;              /* PAD_FLAG_ENABLED = 0x80 */
    /* 0x16 */ s8 mode;               /* 0=normal, 1=repeat, 2=locked */
    /* 0x17 */ u8 unk_17;
    /* 0x18 */ s16 analog_min;
    /* 0x1A */ s16 analog_max;
    /* 0x1C */ s16 deadzone_min;
    /* 0x1E */ s16 deadzone_max;
} PadEntry;  /* size = 0x20 (32 bytes) */
```

Located at RAM address `0x80140BF0`.

## Arcade Source Comparison

### Arcade Input Structures

From `reference/repos/rushtherock/game/modeldat.h`:

```c
/* Fast input - updated every model iteration */
typedef struct fastintyp {
    S32 lasttime;       /* Time of last model iteration */
    F32 modeltime;      /* Model iteration time (seconds) */
    S16 wheel;          /* Wheel position -1.0 to 1.0 scaled */
    S16 swvel;          /* Steering wheel velocity */
    S16 modelrun;       /* Stall model if zero */
} fastintyp;

/* Main input - updated less frequently */
typedef struct mainintyp {
    S16 clutch;         /* Clutch position 0 to 1.0 */
    S16 brake;          /* Brake position 0 to 1.0 */
    S16 throttle;       /* Throttle position 0 to 1.0 (if !KILLMAININ) */
    S16 gear;           /* Current gear 0=neutral */
    S16 autotrans;      /* 1 if automatic trans, 0 manual */
    S16 ignition;       /* 1=on, 0=off, 2=end-game */
    S16 startermotor;   /* Non zero if starter engaged */
    S16 crashthreshold; /* Crash threshold, tens of pounds */
} mainintyp;
```

### Arcade Controls Processing

From `reference/repos/rushtherock/game/controls.c`:

```c
void controls(MODELDAT *m) {
    /* Steering from wheel */
    m->steerangle = m->steergain * CTLSCALE * m->fastin.wheel;

    /* Clutch, brake, throttle from pedals */
    m->clutch = m->mainin.clutch * CTLSCALE;
    m->brake = m->mainin.brake * CTLSCALE;
    m->throttle = m->mainin.throttle * CTLSCALE;

    /* Gear from shifter */
    if (!m->autotrans)
        m->commandgear = m->gear = m->mainin.gear;

    /* Calculate brake torque per tire */
    for (i = 0; i < 4; i++) {
        m->torque[i] = -m->brakegain[i] * m->brake * rolloff;
    }
}
```

### Arcade Button Definitions

From `reference/repos/rushtherock/game/controls.h`:

```c
#define SW_RADIO        0x8         /* Music/radio button */
#define SW_REVERSE      0x00000100  /* Reverse gear */
#define SW_FIRST        0x00000200  /* 1st gear */
#define SW_SECOND       0x00000400  /* 2nd gear */
#define SW_THIRD        0x00000800  /* 3rd gear */
#define SW_FOURTH       0x00001000  /* 4th gear */
#define SW_ABORT        SW_START    /* Abort button */
```

From `reference/repos/rushtherock/game/io.h`:

```c
#define BLUEBUTTON  SW0BIT1   /* Blue cabinet button */
#define BLACKBUTTON SW0BIT0   /* Black cabinet button */
#define STARTBUTTON SW1BIT0   /* Start button */

#define JOYRIGHT    SW0BIT4
#define JOYLEFT     SW0BIT2
#define JOYUP       SW0BIT3
#define JOYDOWN     SW0BIT5
```

## Vibration/Rumble Feedback

### Rumble Pak Support

The game supports N64 Rumble Pak via libultra motor functions:

| Address | Function | Description |
|---------|----------|-------------|
| 0x80009F20 | `osMotorInit` | Initialize rumble pak |
| 0x8000A088 | `__osMotorAccess` | Build rumble pak PIF command |
| 0x8000A194 | `osMotorStart` | Start motor (write 0x01 to 0xC000) |
| 0x8000A2F0 | `osMotorStop` | Stop motor (write 0x00 to 0xC000) |

### Rumble Feedback Events

From analysis of `src/game/game.c`:

1. **Collision Impact** - Rumble on car-car or car-wall collision
2. **Landing** - Rumble when landing from jumps
3. **Menu Test** - Short-short-long pattern in options menu

### Menu Vibration Test Pattern

```c
void menu_vibration_test(void) {
    /* Pattern: short-short-long */
    /* Pulse 1: 10 frames on, 5 frames off */
    /* Pulse 2: 10 frames on, 5 frames off */
    /* Pulse 3: 30 frames on */
}
```

## Player Input State Offsets

From `player_read_input()` in game.c:

| Offset | Type | Field | Description |
|--------|------|-------|-------------|
| 0xE0 | f32 | steering | -1.0 to 1.0 left/right |
| 0xE4 | f32 | throttle | 0.0 to 1.0 gas amount |
| 0xE8 | f32 | brake | 0.0 to 1.0 brake amount |
| 0xEC | s32 | handbrake | 0 or 1 |
| 0xF0 | s32 | shiftUp | R button pressed |
| 0xF4 | s32 | shiftDown | L button pressed |
| 0xF8 | s32 | viewChange | C-Up pressed |
| 0xFC | s32 | wingDeploy | C-Down pressed |
| 0x100 | s32 | pausePressed | Start pressed |

## Controller Configuration

The game stores controller configuration in save data:

```c
extern s32 controller_config;    /* Current config preset */
extern s32 vibration_setting;    /* 0=off, 1=on */
```

### Configuration Menu

Function `menu_controller_config()` at address `0x800CFE74` handles:
- Button remapping options
- Analog sensitivity adjustment
- Vibration enable/disable
- Configuration save/load

## N64 vs Arcade Input Mapping Summary

| Feature | Arcade | N64 Adaptation |
|---------|--------|----------------|
| Steering | Physical wheel with force feedback | Analog stick X-axis |
| Throttle | Analog pedal (0-100%) | A button (digital) or stick Y+ (analog) |
| Brake | Analog pedal (0-100%) | B button (digital) or stick Y- (analog) |
| Handbrake | N/A | Z trigger |
| Gear Shift | H-pattern shifter | L/R triggers (sequential) |
| View Change | Cabinet button | C-Up |
| Wing Deploy | N/A (N64 exclusive) | C-Down |
| Force Feedback | Steering wheel motor | Rumble Pak |
| Clutch | Physical pedal | Removed (auto-clutch) |

## Related Files

### N64 Source Files

- `src/game/input.c` - Controller input handling
- `src/game/game.c` - Input processing in player_read_input()
- `src/libultra/os_cont.c` - Low-level controller interface
- `include/PR/os_cont.h` - Controller structure definitions

### Arcade Reference Files

- `reference/repos/rushtherock/game/controls.c` - Control data to model
- `reference/repos/rushtherock/game/controls.h` - Switch/button definitions
- `reference/repos/rushtherock/game/io.h` - I/O port definitions
- `reference/repos/rushtherock/game/modeldat.h` - Input structures

## Implementation Notes

1. **Deadzone Handling**: The N64 stick has a hardware deadzone, but software applies an additional deadzone of ~10 units to account for stick wear and drift.

2. **Analog vs Digital**: While arcade used true analog pedals, N64 primarily uses digital buttons with optional analog fallback via stick Y-axis.

3. **Force Feedback Translation**: Arcade steering wheel force feedback translates to N64 Rumble Pak pulses for collisions and landings.

4. **Automatic Transmission**: N64 defaults to automatic transmission since the controller lacks a proper shifter.

5. **Wing System**: The wing/stunt system is N64-exclusive and uses C-Down button, not present in arcade.
