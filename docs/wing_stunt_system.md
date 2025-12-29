# Rush 2049 Wing and Stunt System

## Overview

San Francisco Rush 2049 features a unique wing and stunt system not present in the arcade Rush The Rock source code. This document details the mechanics, functions, and data structures discovered during decompilation of the N64 version.

The wing system allows cars to deploy retractable wings mid-air, enabling gliding behavior and enhanced aerial control. Combined with the stunt detection and scoring system, this creates Rush 2049's signature "Stunt Mode" gameplay.

## Key Differences from Arcade Source

| Feature | Rush The Rock (Arcade) | Rush 2049 (N64) |
|---------|------------------------|-----------------|
| Wings | Not present | Deployable wings with lift/drag physics |
| Stunts | No stunt system | Full stunt detection and scoring |
| Air Control | Basic | Enhanced with wing physics |
| Game Modes | Race only | Race, Battle, Stunt |
| Arenas | None | 8 dedicated stunt arenas |

The arcade source (`reference/repos/rushtherock/`) contains no equivalent wing or stunt code. This is entirely new functionality added for Rush 2049.

---

## Wing Mechanics

### Wing Deployment

Wings are deployed by pressing **C-Down** (or holding **Z** button per loading tips). The deployment process is animated over 10 frames.

**Key Functions:**
- `wing_deploy()` - Deploys the car's wings
- `wing_retract()` - Retracts the wings
- `wing_state_get()` - Returns current wing state

**Wing States:**
| Value | State | Description |
|-------|-------|-------------|
| 0 | Retracted | Wings fully retracted, no aero effects |
| 1 | Deploying | Wings extending (animation in progress) |
| 2 | Deployed | Wings fully extended, full aero effects |

### Wing Physics

When deployed, wings apply aerodynamic forces based on real physics formulas:

```c
/* Lift force: L = 0.5 * rho * v^2 * S * Cl */
liftForce = 0.5f * airDensity * speedSq * wingArea * liftCoef * wingAngle;

/* Drag force: D = 0.5 * rho * v^2 * S * Cd */
dragForce = 0.5f * airDensity * speedSq * wingArea * dragCoef * wingAngle;
```

**Physical Constants:**
| Constant | Value | Units |
|----------|-------|-------|
| Air Density | 0.002377 | slug/ft^3 |
| Wing Area | 20.0 | ft^2 |
| Base Lift Coefficient | 0.5 - 2.0 | - |
| Base Drag Coefficient | 0.1 - 0.4 | - |

### Glide Physics Function

The `glide_physics_apply()` function implements the gliding behavior:

1. **Lift Force**: Counteracts gravity, reducing fall rate
2. **Drag Force**: Reduces forward speed proportionally
3. **Control Forces**: Enhanced pitch/roll control from player input
4. **Angular Damping**: 0.98 multiplier per axis for stability

Wings provide enhanced air control - angular velocity from stick input is multiplied by `wingAngle * 2.0` when deployed.

---

## Car Structure Offsets (Wing/Stunt Data)

### Wing Data (0x1D0 - 0x1F0)

| Offset | Type | Name | Description |
|--------|------|------|-------------|
| 0x1D0 | f32 | comboTimer | Timer for combo windows |
| 0x1D4 | s32 | wingState | 0=retracted, 1=deploying, 2=deployed |
| 0x1D8 | f32 | deployTimer | Animation timer (0-10 frames) |
| 0x1DC | f32 | wingAngle | 0.0 = retracted, 1.0 = fully deployed |
| 0x1E0 | f32 | liftCoef | Current lift coefficient |
| 0x1E4 | f32 | dragCoef | Current drag coefficient |
| 0x1E8 | f32 | liftForce | Applied lift force |
| 0x1EC | f32 | dragForce | Applied drag force |

### Airborne State (0x1C0 - 0x1D0)

| Offset | Type | Name | Description |
|--------|------|------|-------------|
| 0x1C0 | s32 | airborne | 1 if car is in air |
| 0x1C4 | s32 | airTime | Frames spent airborne |
| 0x1C8 | s32 | wasAirborne | Previous frame's airborne state |

### Stunt Tracking (0x180 - 0x1A0, 0x300 - 0x400)

| Offset | Type | Name | Description |
|--------|------|------|-------------|
| 0x118 | s32[4] | roadCode | Wheel contact (4=AIR for all = airborne) |
| 0x180 | s32 | stuntState | Stunt detection state machine |
| 0x194 | f32[3] | accumRot | Accumulated rotation (roll, pitch, yaw) |
| 0x1A0 | f32 | airTimeFloat | Air time in seconds |
| 0x300 | s32 | stuntState2 | Alternative stunt state tracking |
| 0x304 | s32 | currentStunt | Bitfield of detected stunts |
| 0x308 | s32 | comboCount | Current combo count |
| 0x30C | s32 | comboTimer | Combo timeout |
| 0x310 | s32 | totalScore | Accumulated stunt score |
| 0x320 | f32[3] | stuntRotation | Rotation accumulators |
| 0x330 | f32[3] | landingAngle | Orientation at takeoff |
| 0x340 | s32 | wingTime | Frames with wings deployed |
| 0x350 | s32[16] | comboHistory | Recent stunts for combo detection |
| 0x390 | s32 | bestCombo | Best combo achieved |
| 0x394 | s32 | comboScore | Current combo's point value |

---

## Stunt Detection

### Stunt Types

Stunts are detected by measuring accumulated rotation around each axis:

| ID | Stunt | Axis | Threshold | Points |
|----|-------|------|-----------|--------|
| 0x01 | Right Barrel Roll | Roll (Z) | +360 deg | 500 |
| 0x02 | Left Barrel Roll | Roll (Z) | -360 deg | 500 |
| 0x04 | Front Flip | Pitch (X) | +360 deg | 750 |
| 0x08 | Back Flip | Pitch (X) | -360 deg | 750 |
| 0x10 | Clockwise Spin | Yaw (Y) | +360 deg | 300 |
| 0x20 | Counter-Clockwise Spin | Yaw (Y) | -360 deg | 300 |
| 0x40 | High-Speed Rotation | Any | >3.0 rad/s | 200 |
| 0x80 | Inverted | - | Upside down | Bonus |

### Multi-Rotation Stunts

| Trick | Rotations | ID Pattern | Points |
|-------|-----------|------------|--------|
| Single Flip | 1 | 1, 2 | 750 |
| Double Flip | 2 | 11, 12 | 1500 |
| Triple Flip | 3 | 21, 22 | 2250 |
| Single Barrel Roll | 1 | 3, 4 | 500 |
| Double Barrel Roll | 2 | 13, 14 | 1000 |
| Triple Barrel Roll | 3 | 23, 24 | 1500 |
| 180 Spin | 0.5 | 5, 6 | 150 |
| 360 Spin | 1 | 15, 16 | 300 |
| 540 Spin | 1.5 | 25, 26 | 450 |
| 720 Spin | 2 | 35, 36 | 600 |

### Combo Stunts

| Combo | Components | Bonus |
|-------|------------|-------|
| Corkscrew | Barrel Roll + Flip | +1000 |
| Rodeo | Flip + Spin | +800 |
| Triple Roll | 3 consecutive barrel rolls | +2000 |
| Triple Flip | 3 consecutive flips | +3000 |
| Variety | 5+ different stunt types | +1500 |

---

## Detection Functions

### Main Detection: `trick_detect()`

The main dispatcher for stunt detection:

```c
s32 trick_detect(void *car) {
    /* Check if all wheels are in air (roadCode = 4) */
    for (i = 0; i < 4; i++) {
        if (roadCode[i] != 4) {
            isAirborne = 0;
            break;
        }
    }

    if (!isAirborne) {
        /* Just landed - finalize any active trick */
        return *currentTrick;
    }

    /* Check rotation-based tricks */
    trick = combo_end(car);       /* Front/back flip */
    trick = stunt_score_add(car); /* Barrel roll */
    trick = spin_update(car);     /* Spin */

    return trick;
}
```

### Flip Detection: `flip_detect()` / `combo_end()`

Detects front and back flips by measuring pitch rotation:

- **Front Flip**: Negative pitch rotation (nose down first)
- **Back Flip**: Positive pitch rotation (nose up first)
- **Threshold**: 270 degrees (4.71 radians) minimum
- **Maximum**: Triple flip (990+ degrees)

### Barrel Roll Detection: `barrel_roll_detect()` / `stunt_score_add()`

Detects lateral barrel rolls by measuring roll rotation:

- **Left Roll**: Positive roll (counter-clockwise from behind)
- **Right Roll**: Negative roll (clockwise from behind)
- **Threshold**: 270 degrees minimum
- **Maximum**: Triple barrel roll

### Spin Detection: `spin_detect()` / `spin_update()`

Detects horizontal spins by measuring yaw rotation:

- **Left Spin**: Positive yaw
- **Right Spin**: Negative yaw
- **Threshold**: 180 degrees (counted in 180-degree increments)
- **Maximum**: 720 spin (4 x 180)

---

## Scoring System

### Base Scoring: `stunt_calc_score()`

```c
s32 stunt_calc_score(s32 stuntType) {
    s32 score = 0;

    /* Base points per stunt type */
    if (stuntType & 0x01) score += 500;   /* Right barrel roll */
    if (stuntType & 0x02) score += 500;   /* Left barrel roll */
    if (stuntType & 0x04) score += 750;   /* Front flip */
    if (stuntType & 0x08) score += 750;   /* Back flip */
    if (stuntType & 0x10) score += 300;   /* Clockwise spin */
    if (stuntType & 0x20) score += 300;   /* Counter-clockwise spin */
    if (stuntType & 0x40) score += 200;   /* High speed rotation */

    /* Combo bonuses */
    if ((stuntType & 0x03) && (stuntType & 0x0C)) score += 1000; /* Corkscrew */
    if ((stuntType & 0x0C) && (stuntType & 0x30)) score += 800;  /* Rodeo */

    return score;
}
```

### Landing Multiplier

Landing quality affects final score:

| Landing Type | Multiplier | Angle Difference |
|--------------|------------|------------------|
| Perfect | 3x | < 10 degrees |
| Good | 2x | < 30 degrees |
| Normal | 1x | < 60 degrees |
| Crash | 0x | >= 60 degrees |

### Combo Multiplier

Consecutive stunts increase score:

| Combo Count | Multiplier |
|-------------|------------|
| 1-2 | 1x |
| 3-4 | 2x |
| 5-6 | 3x |
| 7-9 | 4x |
| 10+ | 5x |

### Stunt Multiplier: `stunt_multiplier()`

Additional bonuses based on conditions:

- **Wings Deployed**: Bonus multiplier when wings are active
- **Air Time**: Bonus after 1+ second in air (up to 2x at 4 seconds)
- **Speed**: Higher speed = higher multiplier

---

## Stunt Mode

### Game Mode Setup

Stunt Mode is game mode 2 (after Race and Battle):

```c
case 2:  /* Stunt */
    game_time = 2 * 60 * 60;    /* 2 minutes */
    stunt_scoring_enabled = 1;   /* Enable stunt scoring */
    break;
```

### Stunt Arenas

Rush 2049 includes 8 dedicated stunt arenas:

| ID | Name | Unlock Condition |
|----|------|------------------|
| 0 | Alcatraz | Default |
| 1 | Metro | Default |
| 2 | Pier 39 | Default |
| 3 | Golden Gate | Complete 3 tracks |
| 4 | Twin Peaks | Complete 5 tracks |
| 5 | Downtown | Complete 7 tracks |
| 6 | Obstacle | Complete all main tracks |
| 7 | Hidden | All stunt arenas completed |

Arena track IDs use offset 100+ (e.g., `trackno = selectedArena + 100`).

### Stunt Mode Setup: `stunt_mode_setup()`

The stunt mode configuration screen allows:
- Arena selection
- Time limit adjustment
- Wings enable/disable toggle
- Access to stunt tutorial

---

## Tutorial Information

The in-game stunt tutorial (`draw_text` at line 34849+) teaches:

### Barrel Roll
- Spin car while airborne
- 500 points per rotation

### Flip
- Front/back flip in air
- 750 points per flip

### Wing Glide
- Hold Z to deploy wings
- Enhanced air control and longer hang time

---

## Global Variables

| Address | Name | Description |
|---------|------|-------------|
| 0x8015A318 | stunt_scoring_enabled | 1 if stunt scoring active |
| 0x801461D0 | wing_enabled | 1 if wings available |
| 0x80159D88 | D_80159D88 | Total stunts completed |
| 0x8014C038 | D_8014C038[4] | Player stunt scores |
| 0x8014C03C | D_8014C03C[4] | Player best combos |

---

## Achievements

Stunt-related achievements:

| ID | Name | Requirement |
|----|------|-------------|
| 3 | First Stunt | Score 1000+ stunt points |
| 4 | Stunt Master (100K) | Accumulate 100,000 stunt points |
| 8 | Trick Shot | Perform a specific combo |
| 9 | Airborne | Extended air time |

---

## Audio Cues

Sound effects related to stunts:

| Sound ID | Trigger |
|----------|---------|
| 14-16 | Stunt calls ("Nice!", "Awesome!", "Incredible!") |
| 17 | "Wings deployed" announcement |
| 20 | Bonus/unlock sound |
| 35 | Boost activate |

The `stunt_command_send()` function triggers appropriate audio/visual feedback when stunts are scored.

---

## Loading Tips

Wing/stunt related tips shown during loading:

- "USE WINGS FOR BIG AIR"
- "HOLD Z FOR WING DEPLOY"
- "Wings help with big jumps!"
- "Hit ramps at full speed for stunts."

---

## Function Reference

### Core Wing Functions

| Function | Size | Description |
|----------|------|-------------|
| `wing_deploy()` | 1780 bytes | Deploy car wings |
| `wing_retract()` | ~500 bytes | Retract car wings |
| `wing_state_get()` | ~100 bytes | Query wing state |
| `glide_physics_apply()` | ~400 bytes | Apply aerodynamic forces |

### Core Stunt Functions

| Function | Size | Description |
|----------|------|-------------|
| `trick_detect()` | - | Main stunt detection dispatcher |
| `stunt_update()` | - | Per-frame stunt state update |
| `stunt_calc_score()` | - | Calculate point value for stunts |
| `stunt_combo_update()` | - | Track and chain combos |
| `stunt_multiplier()` | - | Calculate score multiplier |
| `stunt_command_send()` | - | Send stunt notification |

### Detection Functions

| Function | Size | Description |
|----------|------|-------------|
| `combo_end()` / `flip_detect()` | 1448 bytes | Detect front/back flips |
| `stunt_score_add()` / `barrel_roll_detect()` | 3576 bytes | Detect barrel rolls |
| `spin_update()` / `spin_detect()` | 2524 bytes | Detect spins |
| `air_time_track()` | 1336 bytes | Track air time and rotation |
| `landing_detect()` | - | Detect landing quality |

### Mode Functions

| Function | Description |
|----------|-------------|
| `stunt_mode_setup()` | Configure stunt mode options |
| `stunt_arena_preview()` | Render arena preview |

---

## Implementation Notes

1. **No Arcade Equivalent**: This entire system is unique to Rush 2049. The arcade Rush The Rock source has no wing, stunt, or aerial trick code.

2. **Physics Model**: The wing physics use simplified aerodynamic formulas but provide convincing glide behavior.

3. **Rotation Tracking**: Stunts are detected by accumulating angular velocity over time while airborne, then checking against thresholds on landing.

4. **Combo System**: The combo system tracks the last 16 stunts performed and awards bonuses for patterns (triple rolls, variety, etc.).

5. **Landing Detection**: Uses the difference between takeoff orientation and landing orientation to determine quality.

---

## Related Files

- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - Main game code containing all stunt functions (lines 19825-40600)
- No arcade equivalent files (feature is N64-exclusive)
