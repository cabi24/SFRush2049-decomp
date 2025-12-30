# Rush 2049 Stunt/Wing System Documentation

This document describes the stunt and wing system unique to San Francisco Rush 2049. Unlike previous Rush games (Rush: The Rock, Rush 2), Rush 2049 introduced deployable wings and a comprehensive aerial trick scoring system.

## Overview

The stunt system is Rush 2049's signature feature, allowing players to:
- Deploy retractable wings for aerial control
- Perform aerial tricks (spins, flips, barrel rolls)
- Chain tricks into combos for score multipliers
- Compete in dedicated Stunt Mode arenas

This system was a major evolution for the Rush series, transforming the game from a pure racer into a hybrid racing/stunt title.

---

## Wing System

### Wing Mechanics

The deployable wing system provides air control during jumps. Wings are activated by player input (C-Down on N64 controller).

#### Wing States

Defined in `include/game/stunt.h`:

| State | Value | Description |
|-------|-------|-------------|
| `WING_STATE_RETRACTED` | 0 | Wings hidden (default) |
| `WING_STATE_EXTENDING` | 1 | Wings deploying |
| `WING_STATE_EXTENDED` | 2 | Wings fully out |
| `WING_STATE_RETRACTING` | 3 | Wings folding back |

#### Wing Deployment

From `src/game/game.c` (wing_deploy function at ~line 40378):

```c
/* Car structure offsets for wing state */
0x1D4: wing state (0=retracted, 1=deploying, 2=deployed)
0x1D8: wing deploy timer
0x1DC: wing angle (0.0 = retracted, 1.0 = fully deployed)
0x1E0: wing lift coefficient
0x1E4: wing drag coefficient
```

**Deployment Timing:**
- Deploy rate: 4.0 units/second (full deploy in ~0.25 seconds)
- Retract rate: 6.0 units/second (faster than deploy)
- Full extension: `WING_EXTEND_TIME` = 15 frames (~0.25 sec)

#### Wing Physics Effects

When wings are deployed, the car experiences aerodynamic forces:

**Lift Force** (reduces fall rate):
```c
liftCoef = 0.5f + (wingAngle * 1.5f);  /* 0.5 base, up to 2.0 */
liftForce = 0.5 * airDensity * speedSq * wingArea * liftCoef * wingAngle;
```

**Drag Force** (reduces forward speed):
```c
dragCoef = 0.1f + (wingAngle * 0.3f);  /* 0.1 base, up to 0.4 */
dragForce = 0.5 * airDensity * speedSq * wingArea * dragCoef * wingAngle;
```

**Air Control** (pitch/roll influence):
```c
pitchControl = angVel[0] * wingAngle * 2.0f;
rollControl = angVel[2] * wingAngle * 2.0f;
```

**Physics Constants** (from glide_physics_apply):
- Air density: 0.002377 slug/ft^3
- Wing area: 20.0 square feet
- Lift factor: `WING_LIFT_FACTOR` = 0.3
- Drag factor: `WING_DRAG_FACTOR` = 0.1
- Control factor: `WING_CONTROL_FACTOR` = 0.5

---

## Stunt Detection System

### Stunt States

Defined in `include/game/stunt.h`:

| State | Value | Description |
|-------|-------|-------------|
| `STUNT_STATE_DISABLED` | 0 | Stunt mode not active |
| `STUNT_STATE_GROUNDED` | 1 | On ground, ready |
| `STUNT_STATE_AIRBORNE` | 2 | In air, can trick |
| `STUNT_STATE_TRICKING` | 3 | Performing a trick |
| `STUNT_STATE_LANDING` | 4 | About to land |
| `STUNT_STATE_CRASHED` | 5 | Crashed during trick |

### Trick Types

| Trick | Bitmask | Points | Description |
|-------|---------|--------|-------------|
| Right Barrel Roll | 0x01 | 500 | Full 360 roll clockwise |
| Left Barrel Roll | 0x02 | 500 | Full 360 roll counter-clockwise |
| Front Flip | 0x04 | 750 | Forward somersault |
| Back Flip | 0x08 | 750 | Backward somersault |
| Clockwise Spin | 0x10 | 300 | Horizontal 360 rotation |
| Counter-clockwise Spin | 0x20 | 300 | Horizontal 360 rotation |
| High Speed Rotation | 0x40 | 200 | Bonus for fast tricks |

### Rotation Detection

Tricks are detected by accumulating rotation while airborne:

```c
/* Car structure offsets for stunt tracking (from game.c) */
0x60: forward vector (vec3)
0x6C: up vector (vec3)
0x70: angular velocity (vec3)
0x78: right vector (vec3)
0x1C0: airborne flag
0x300: stunt state
0x304: current stunt bitmask
0x308: combo count
0x30C: combo timer
0x310: total score
0x320: stunt rotation accumulator (vec3)
0x330: landing angle (vec3)
```

**Rotation Thresholds:**
- `SPIN_THRESHOLD` = 330 degrees (horizontal rotation)
- `FLIP_THRESHOLD` = 330 degrees (vertical rotation)
- Full 360 (6.28 radians) required for trick completion

**Stunt Detection Logic** (from stunt_update function):

```c
/* Accumulate rotation while airborne */
rollDelta = carAngVel[0];
pitchDelta = carAngVel[1];
yawDelta = carAngVel[2];

stuntRotation[0] += rollDelta;  /* Roll accumulator */
stuntRotation[1] += pitchDelta; /* Pitch accumulator */
stuntRotation[2] += yawDelta;   /* Yaw accumulator */

/* Detect completed tricks */
if (stuntRotation[0] > 6.28f || stuntRotation[0] < -6.28f) {
    newStunt |= (stuntRotation[0] > 0) ? 0x01 : 0x02; /* Barrel roll */
    stuntRotation[0] = 0.0f; /* Reset for next roll */
}
```

---

## Scoring System

### Base Point Values

From `stunt_calc_score` in `src/game/game.c`:

| Trick Type | Base Points |
|------------|-------------|
| Barrel Roll (left/right) | 500 |
| Flip (front/back) | 750 |
| Spin (left/right) | 300 |
| High Speed Rotation Bonus | 200 |

### Special Combo Bonuses

| Combo Name | Requirements | Bonus |
|------------|--------------|-------|
| Corkscrew | Barrel roll + flip | +1000 |
| Rodeo | Flip + spin | +800 |
| Triple Roll | 3 consecutive barrel rolls | +2000 |
| Triple Flip | 3 consecutive flips | +3000 |
| Variety Bonus | 4+ different trick types in 5-trick combo | +1500 |

### Combo Multipliers

Chain tricks together for increased multipliers:

| Combo Count | Multiplier |
|-------------|------------|
| 1 trick | 1.0x |
| 2 tricks | 1.5x (`COMBO_MULT_2`) |
| 3 tricks | 2.0x (`COMBO_MULT_3`) |
| 4 tricks | 3.0x (`COMBO_MULT_4`) |
| 5+ tricks | 5.0x (`COMBO_MULT_5_PLUS`) |
| 7-9 tricks | 4.0x (from game.c stunt_combo_update) |
| 10+ tricks | 5.0x |

**Combo Timer:**
- 3 seconds (180 frames) to chain next trick
- Combo timer resets after each successful trick

---

## Landing System

### Landing Quality Detection

From `landing_detect` and `landing_bonus_calc` in `src/game/game.c`:

```c
/* Landing angle calculation */
landingAngle = sqrtf(rollAngle * rollAngle + pitchAngle * pitchAngle);
```

**Landing Quality Tiers:**

| Quality | Landing Angle | Impact Velocity | Multiplier |
|---------|---------------|-----------------|------------|
| Perfect | < 0.1 rad | < 30 ft/s | 3x (up.y > 0.95) |
| Great | < 0.3 rad | < 50 ft/s | 2x (up.y > 0.8) |
| Good | < 0.6 rad | < 80 ft/s | 1x (up.y > 0.5) |
| OK | < 1.0 rad | < 120 ft/s | 1x |
| Crash | >= 1.0 rad or upside down | any | 0x (combo reset) |

**Height Thresholds:**
- `MIN_TRICK_HEIGHT` = 3.0 feet (minimum to start tracking tricks)
- `SAFE_LANDING_ANGLE` = 30 degrees (maximum for safe landing)
- `CRASH_LANDING_ANGLE` = 60 degrees (causes crash)

**Air Time Bonuses:**
- 2+ seconds of air time: +1 to landing quality
- 3+ seconds: 2x time multiplier on bonus

### Landing Detection

```c
/* Check for landing (was airborne, now grounded) */
if (*wasAirborne && !(*isAirborne)) {
    /* Calculate landing quality based on up vector Y component */
    landingAngle = upVec[1];  /* 1.0 = perfect, 0 = sideways, <0 = upside down */

    if (landingAngle < 0.0f) {
        /* Crashed - upside down */
        *comboMult = 1;  /* Reset combo */
        return 0;
    }
}
```

---

## Stunt Mode

### Stunt Arenas

Rush 2049 includes dedicated stunt arenas optimized for trick performance:

From `src/game/track.c`:
- Maximum stunt arenas: `MAX_STUNT_ARENAS` = 4
- Arena names: "Stunt Park", etc.
- Track type: `TRACK_TYPE_STUNT` = 2

### Stunt Mode State

```c
extern s32 stunt_mode_state;        /* Stunt mode active flag */
extern s32 stunt_session_timer;     /* Session timer (frames) */
extern s32 session_time_limit;      /* Session time limit */
extern s32 player_stunt_scores[4];  /* Per-player scores */
extern s32 player_combo_mult[4];    /* Per-player combo multipliers */
extern s32 player_best_trick[4];    /* Best single trick per player */
extern s32 player_best_combo[4];    /* Best combo per player */
```

### Arena Collectibles

Stunt arenas feature additional scoring elements:

```c
void stunt_hit_target(s32 car_index, s32 target_id);
void stunt_collect_coin(s32 car_index, s32 coin_value);
```

- **Targets**: Score multiplied by target ID (100 * target_id points)
- **Coins**: Direct point value addition

---

## Key State Variables

### Car Structure Offsets (from game.c analysis)

| Offset | Size | Description |
|--------|------|-------------|
| 0x24 | f32[3] | Car position |
| 0x34 | f32[3] | Car velocity |
| 0x60 | f32[3] | Forward vector |
| 0x6C | f32[3] | Up vector |
| 0x70 | f32[3] | Angular velocity |
| 0x78 | f32[3] | Right vector |
| 0x84 | f32[3] | Angular velocity (alternate) |
| 0x118 | s8[4] | Road codes (all AIR = airborne) |
| 0x190 | s32 | Is airborne flag |
| 0x18C | s32 | Was airborne (previous frame) |
| 0x1A0 | f32 | Air time |
| 0x1A4 | s32 | Current trick ID |
| 0x1AC | s32 | Combo count |
| 0x1BC | f32 | Nitro reserve |
| 0x1C0 | s32/f32 | Airborne / nitro active |
| 0x1C4 | s32 | Air time (frames) |
| 0x1C8 | s32 | Was airborne flag |
| 0x1D0 | f32 | Combo timer |
| 0x1D4 | s32 | Wing state |
| 0x1D8 | f32 | Wing deploy timer |
| 0x1DC | f32 | Wing angle (0-1) |
| 0x1E0 | s32/f32 | Wing deployed flag / lift coef |
| 0x1E4 | f32 | Wing drag coefficient |
| 0x300 | s32 | Stunt state |
| 0x304 | s32 | Current stunt bitmask |
| 0x308 | s32 | Combo count |
| 0x30C | s32 | Combo timer |
| 0x310 | s32 | Total score |
| 0x320 | f32[3] | Stunt rotation accumulators |
| 0x330 | f32[3] | Landing angle |
| 0x340 | s32 | Wing time tracking |
| 0x350 | s32[16] | Combo history |
| 0x390 | s32 | Best combo |
| 0x394 | s32 | Combo score |

### Global Variables

| Address | Name | Description |
|---------|------|-------------|
| 0x801146EC | gstate | Game state byte |
| 0x80142AFC | frame_counter | Global frame counter |
| 0x80152818 | car_array | Array of car data |
| - | stunt_combo_timer | Combo timer |
| - | stunt_combo_count | Current combo count |
| - | wing_enabled | Wings enabled flag |
| - | stunt_mode_state | Stunt mode active |
| - | player_stunt_scores[4] | Player scores |

---

## Boost Integration

Stunts integrate with the boost system for additional speed benefits:

From `include/game/boost.h`:

```c
#define BOOST_TYPE_STUNT        3   /* Stunt completion boost */
#define BOOST_DURATION_STUNT    45  /* 0.75 seconds */
#define BOOST_MULT_STUNT        1.2f /* 20% speed increase */
#define BOOST_METER_STUNT       25.0f /* Meter from stunt */
```

**Stunt-to-Boost Connection:**
```c
void boost_on_stunt(s32 player, f32 bonus) {
    /* Add meter based on stunt quality */
    boost_add_meter(player, BOOST_METER_STUNT * bonus);

    /* Activate stunt boost */
    boost_activate(player, BOOST_TYPE_STUNT, BOOST_SOURCE_STUNT);
}
```

---

## Sound Effects

Stunt-related audio cues:

| Sound ID | Trigger |
|----------|---------|
| 0x20 | Nice combo (3 tricks) |
| 0x21 | Great combo (5 tricks) |
| 0x22 | Insane combo (10 tricks) |
| 0x40 | Perfect landing |
| 0x41 | Good landing |
| 0x42 | Rough landing |
| 0x43 | Crash |
| SFX_WING_EXTEND | Wing deployment |
| SFX_CHECKPOINT | Trick completion / target hit |
| SFX_CRASH_HEAVY | Crash landing |

---

## Controller Mapping

N64 controller inputs for stunt system:

| Button | Action |
|--------|--------|
| C-Down | Deploy/retract wings |
| Analog Stick | Air control (when wings deployed) |
| C-Up | View change |

From `player_read_input` in game.c:
```c
wingDeploy = (s32 *)((u8 *)player + 0xFC);
*wingDeploy = (buttons & 0x0004) ? 1 : 0;  /* C-Down */
```

---

## Implementation Files

| File | Purpose |
|------|---------|
| `src/game/stunt.c` | Main stunt system implementation |
| `src/game/game.c` | Core game loop stunt integration |
| `include/game/stunt.h` | Stunt system header/definitions |
| `include/game/boost.h` | Boost system integration |
| `src/game/boost.c` | Stunt-to-boost connection |
| `src/game/effects.c` | Visual effects (wing trails, etc.) |

---

## Differences from Arcade

Rush 2049 was developed specifically for N64 (not a port), but the stunt system shares some concepts with arcade Rush titles:

**N64-Specific Features:**
- Full deployable wing system with physics
- Comprehensive trick detection
- Combo multiplier system
- Dedicated stunt arenas
- Integration with boost meter

The arcade Rush: The Rock source (`reference/repos/rushtherock/`) does NOT contain wing/stunt code, as this was a Rush 2049 innovation.

---

## Function Address Reference

### Core Stunt Functions

| Address | Function | Size | Description |
|---------|----------|------|-------------|
| 0x800F5000 | stunt_update | 3832 | Main stunt system update per frame |
| 0x800F5EF8 | stunt_calc_score | 588 | Calculate base score for trick type |
| 0x800F6144 | stunt_combo_update | 1888 | Update combo chain and multipliers |
| 0x800F6894 | wing_deploy | 580 | Deploy/retract wings |
| 0x800F6AD8 | trick_detect | 740 | Detect current trick from rotation |
| 0x800F6DBC | landing_detect | 1168 | Evaluate landing quality |
| 0x800F724C | stunt_multiplier | 252 | Calculate total score multiplier |
| 0x80101D8C | trick_detect_main | 1168 | Main stunt detection dispatcher |
| 0x8010221C | trick_register | 564 | Record completed trick and award points |
| 0x80102450 | air_time_track | 1336 | Update air time and rotation accumulator |
| 0x801019AC | landing_bonus_calc | 1040 | Calculate landing bonus points |
| 0x80101370 | combo_trick_add | 2412 | Add trick to combo chain |

### Menu/UI Functions

| Address | Function | Size | Description |
|---------|----------|------|-------------|
| 0x800D3B28 | stunt_mode_setup | 5068 | Stunt mode configuration menu |
| 0x800D3E50 | stunt_arena_preview | 384 | Render arena preview in menu |
| 0x800D5C90 | high_scores_display | 220 | Show stunt high scores |
| 0x800E2C00 | hud_stunt_render | - | Draw stunt score on HUD |
| 0x800DEF68 | mode_select_screen | 2976 | Race/Battle/Stunt mode selection |

### Detection Subsystem Functions

| Address | Function | Description |
|---------|----------|-------------|
| 0x800E5000 | stunt_detect | Detect stunt type from air state |
| 0x800E5200 | stunt_score_calc | Calculate stunt score |
| 0x800E5400 | stunt_combo_update | Update combo multiplier |
| 0x800E5600 | stunt_landing_check | Check for safe landing |
| 0x800E5800 | stunt_trick_register | Register completed trick |
| 0x800E5A00 | stunt_height_bonus | Calculate height bonus |

### Support Functions

| Address | Function | Size | Description |
|---------|----------|------|-------------|
| 0x800B39BC | crowd_cheer_play | 396 | Play crowd cheer sound for stunts |

---

## Stunt Mode Setup Details

From `stunt_mode_setup()` at 0x800D3B28:

### Arena Selection

| Arena ID | Name | Difficulty |
|----------|------|------------|
| 0 | Alcatraz | Easy |
| 1 | Metro | Easy |
| 2 | Pier 39 | Medium |
| 3 | Golden Gate | Medium |
| 4 | Twin Peaks | Medium |
| 5 | Downtown | Hard |
| 6 | Obstacle | Hard |
| 7 | Freeform | Hard |

### Time Limits

| Option | Time (seconds) |
|--------|----------------|
| 0 | 60 (1 minute) |
| 1 | 120 (2 minutes) |
| 2 | 180 (3 minutes) |
| 3 | 300 (5 minutes) |
| 4 | 0 (Unlimited) |

### Target Scores

| Option | Score |
|--------|-------|
| 0 | 10,000 |
| 1 | 25,000 |
| 2 | 50,000 |
| 3 | 100,000 |
| 4 | 250,000 |
| 5 | None (unlimited) |

---

## Stunt Tutorial Content

The in-game tutorial (displayed from stunt_mode_setup menu) shows:

```
TRICKS AND SCORING:

BARREL ROLL
- Spin car while airborne
- 500 pts per rotation

FLIP
- Front/back flip in air
- 750 pts per flip

WING GLIDE
- Hold Z to deploy wings
- 100 pts per second

COMBO BONUS
- Chain tricks for 2x-5x
```

---

## Technical Notes

1. **Frame Rate**: All timers assume 60 FPS operation
2. **Rotation Units**: Internal tracking uses radians; display uses degrees
3. **Height Units**: All heights in feet (consistent with arcade physics)
4. **Combo Window**: 3 seconds (180 frames) between tricks to maintain combo
5. **Wing Physics**: Simplified aerodynamic model (lift/drag coefficients)
6. **Stunt Arena Track IDs**: Base track ID for stunt arenas is 100+ (track ID = arena index + 100)
7. **Wheel Contact Check**: All 4 wheels must have road code = 4 (AIR) for airborne status

---

## Revision History

| Date | Change |
|------|--------|
| 2025-12-30 | Added function addresses, stunt mode setup details, arena list |
| Previous | Initial documentation from decompilation analysis |
