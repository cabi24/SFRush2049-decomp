# HUD and UI Rendering System Mapping

This document maps the arcade HUD (Heads-Up Display) and UI rendering system to the N64 Rush 2049 implementation.

## Overview

The arcade Rush The Rock (and SF Rush series) uses a "blit-based" rendering system for 2D HUD elements. Each HUD element is a textured sprite ("blit") with position, animation, and visibility properties. The main HUD source is in `game/hud.c` with supporting definitions in `game/hud.h`.

## Arcade Screen Layout

The arcade uses safe-zone margins to ensure visibility on CRT displays:

```
Screen Constants (from hud.h):
SCR_W   = VIS_H_PIX       (full screen width)
SCR_H   = VIS_V_PIX       (full screen height)
SCR_L   = 5               (safe left edge)
SCR_R   = SCR_W - 10      (safe right edge)
SCR_T   = 5               (safe top edge)
SCR_B   = SCR_H - 5       (safe bottom edge)
SCR_MH  = (SCR_R - SCR_L)/2   (screen middle horizontal)
SCR_MV  = (SCR_B - SCR_T)/2   (screen middle vertical)
```

## HUD Elements

### 1. Speedometer/Tachometer Display

**Location**: Top center of screen

| Element | Arcade Position | Arcade Function | Texture |
|---------|-----------------|-----------------|---------|
| Tach background left | SCR_MH-69, SCR_T-7 | AnimateTach | NEWTACH15L |
| Tach background right | SCR_MH-69+128, SCR_T-7 | AnimateTach | NEWTACH15R |
| MPH/KPH label | SCR_MH-15, SCR_T+20 | AnimateTach | MPH or KPH |
| Tach needle (dot) | SCR_MH-16, SCR_T+50 | AnimateNeedle | DOT |
| Speed digit 1 (100s) | SCR_MH-33, SCR_T+0 | AnimateSpeed | DIGIT0 |
| Speed digit 2 (10s) | SCR_MH-11, SCR_T+0 | AnimateSpeed | DIGIT0 |
| Speed digit 3 (1s) | SCR_MH+11, SCR_T+0 | AnimateSpeed | DIGIT0 |

**Arcade Source**: `hud.c:464-646`
- `AnimateTach()` - Updates mph value, converts to KPH if metric mode
- `AnimateNeedle()` - Positions needle based on RPM (boosted by 1.35x)
- `AnimateSpeed()` - Displays 3-digit speed value

**Speed Calculation**:
```c
hud_mph = (S16)game_car[this_node].mph;
if (metric) hud_mph *= 1.61;  // Convert to KPH
```

**Needle Angle Calculation**:
```c
temp = (game_car[this_node].rpm * 1.35) / 1000.0;  // Fake RPM boost
angle = DEG_TO_RADS * (180 + 35 + temp * (180.0 - (35*2)) / 11.0);
blt->X = SCR_MH - 4 + (60 * fcos(angle));
blt->Y = SCR_T - 7 - 24 - (60 * fsin(angle));
```

### 2. Timer Displays

**Elapsed Time (Top Left)**:

| Element | Position | Function | Format |
|---------|----------|----------|--------|
| TIMETOP background | SCR_L+1, SCR_T+0 | - | Static |
| Minute digit | SCR_L+2, SCR_T+0 | AnimateElapsed | DIGIT1 |
| Second 10s | SCR_L+17, SCR_T+0 | AnimateElapsed | DIGIT1 |
| Second 1s | SCR_L+27, SCR_T+0 | AnimateElapsed | DIGIT1 |
| Hundredth 10s | SCR_L+45, SCR_T+6 | AnimateElapsed | DIGIT2 |
| Hundredth 1s | SCR_L+54, SCR_T+6 | AnimateElapsed | DIGIT2 |

**Checkpoint Time (Below elapsed)**:

| Element | Position | Function |
|---------|----------|----------|
| TIMEBOT background | SCR_L-25, SCR_T+27 | AnimateCheckPt |
| Digits (5 total) | SCR_L+11/28/38/56/65 | AnimateCheckPt |

**Countdown Timer (Top Right)**:

| Element | Position | Function | Format |
|---------|----------|----------|--------|
| Countdown 100s | SCR_R-105, SCR_T+0 | AnimateCTimer | DIGIT9 |
| Countdown 10s | SCR_R-72, SCR_T+0 | AnimateCTimer | DIGIT9 |
| Countdown 1s | SCR_R-39, SCR_T+0 | AnimateCTimer | DIGIT9 |

**Time Conversion** (`cvt_time()`):
```c
// Format 'c' - countdown (SSS)
dest[0] = (sec / 100) % 10;
dest[1] = (sec / 10) % 10;
dest[2] = sec % 10;

// Format 'h' - elapsed (MM:SS.hh)
dest[0] = min % 10;
dest[1] = (sec / 10);
dest[2] = sec % 10;
dest[3] = (t / 100) % 10;  // hundredths
dest[4] = (t / 10) % 10;
```

### 3. Position Indicator

**Location**: Top right, below countdown

| Element | Position | Function |
|---------|----------|----------|
| Place digit | SCR_R-55, SCR_T+40 | AnimatePlace |
| "OUTOF8" suffix | SCR_R-37, SCR_T+40 | AnimatePlace |

**Arcade Source**: `hud.c:1166-1187`
```c
digit = game_car[this_node].place + 1;  // 0-indexed to 1-indexed
if (digit < 1) digit = 1;
if (digit > 8) digit = 8;
```

### 4. Lap Counter

**Location**: On track map (finish flag position)

| Element | Position | Function |
|---------|----------|----------|
| Lap digit 1s | gFinishX+6, gFinishY-8 | AnimateLapDigit |
| Lap digit 10s | gFinishX-6, gFinishY-8 | AnimateLapDigit |
| Finish flag | gFinishX, gFinishY | AnimateFlag |

**Variables**: `gLapsLeft` (laps remaining to display)

### 5. Radar/Traffic Map

**Location**: Left side, below elapsed time

```c
#define RADAR_X       SCR_L           // Position on screen
#define RADAR_Y       (SCR_T + 60)
#define RADAR_WIDE    64.0            // Width of radar frame
#define RADAR_HIGH    256.0           // Height
#define RADAR_BWIDE   3.0             // Border width
```

| Element | Position | Function |
|---------|----------|----------|
| MAPFRAME1 (top) | RADAR_X, RADAR_Y | AnimateRadar |
| MAPFRAME2 (left) | 1+RADAR_X, 32+RADAR_Y | AnimateRadar |
| MAPFRAME2 (right) | 60+RADAR_X, 32+RADAR_Y | AnimateRadar |
| MAPFRAME3 (bottom) | RADAR_X, 240+RADAR_Y | AnimateRadar |
| TRAFFIC cars (x8) | Dynamic | AnimateTraffic |

**Car Icon Size**: 7x8 pixels (TCAR_W x TCAR_H)

**Coordinate Mapping**:
```c
x = (rpos[0] * gMRadarX) + gTRadarX;
y = (rpos[2] * gMRadarY) + gTRadarY;
```

### 6. Track Map (Minimap)

**Location**: Top right area

| Element | Function | Texture |
|---------|----------|---------|
| Track background | AnimateTrackMap | TRACK1_1, TRACK2_1, etc. |
| Car dots (x8) | AnimateDot | TMAPDOT |

**Track Map Coordinates** (world to screen mapping per track):
```c
TrackMapping[MAX_TRACKS][2][4] = {
    // Track 1: world coords, window coords
    {{ 1996.686, -6510.893, -3557.542, 3124.747 },
     { 0.0, 0.0, 101.0, 174.0 }},
    // Track 2, 3, etc...
};
```

### 7. Gear/Shifter Display

**Location**: Bottom right

| Element | Position | Function | Texture |
|---------|----------|----------|---------|
| Shifter | SCR_R-50, SCR_B-64 | AnimateShifter | SHIFTER or AUTOSHIFT |

**Gear States**:
- Manual: Displays current gear (man_gear)
- Automatic: Shows R (reverse), N (neutral), D (drive)

### 8. Radio/CD Display

**Location**: Bottom center

| Element | Position | Function |
|---------|----------|----------|
| CD Player | SCR_MH-128, SCR_B-64 | AnimateRadio |
| Station digit | SCR_MH+55, SCR_B-34 | AnimateRadio |

**Visibility**: Appears for 3 seconds when radio button pressed

### 9. Wrong Way Indicator

**Location**: Center screen (animates to corner)

| Element | Position | Function |
|---------|----------|----------|
| WRONGWAYTEXT | SCR_MH-128, SCR_MV-32 | AnimateWrongWay |
| HUDARROW (x4) | Various | AnimateWrongWay |

**Stray Threshold**: 3.0 * PI / 8.0 radians (~67.5 degrees off course)

### 10. Bonus Time Message

**Location**: Center-bottom

| Element | Position | Function |
|---------|----------|----------|
| BONUS text | SCR_MH-128, SCR_MV+64 | AnimateBonus |

### 11. Player Markers

**Location**: Above each player's car (3D to 2D projection)

| Element | Function | Purpose |
|---------|----------|---------|
| MARKERS (x8) | AnimateMarker | Identifies human players |

## Digit Font Types

From `hud.h`:
```c
typedef enum DigitType {
    SpeedDigit = 0,     // 22x17 - speedometer
    YelDigit,           // 12x32 - yellow large
    SmallYelDigit,      // 8x16  - yellow small
    RedDigit,           // 12x32 - red large
    SmallRedDigit,      // 8x16  - red small
    LapDigit,           // 12x32 - lap counter
    HudTimerDigit,      // 32x38 - HUD timer
    CountdownDigit,     // 32x64 - countdown
    PlaceDigit,         // 24x32 - race position
    TimerDigit,         // 32x32 - general timer
    NUM_DIGIT_TYPES
} DigitType;
```

## Core Arcade Functions

| Function | Purpose | File:Line |
|----------|---------|-----------|
| InitHUD() | Initialize HUD state | hud.c:405-413 |
| ShowHUD() | Create/display HUD blits | hud.c:417-435 |
| RemoveHUD() | Destroy HUD blits | hud.c:439-456 |
| UpdateNumber() | Update digit blit | hud.c:1287-1339 |
| Hidden() | Set/get blit visibility | hud.c:1270-1278 |
| cvt_time() | Convert ms to display format | hud.c:1509-1548 |
| CropBlit() | Adjust blit source region | hud.c:918-930 |
| EnableRadioHUD() | Show radio display | hud.c:874-887 |
| ShowCheckpointTime() | Display checkpoint delta | hud.c:1032-1047 |
| ShowFinishFlag() | Position finish flag | hud.c:1154-1160 |

## Data Sources

| Variable | Source | Purpose |
|----------|--------|---------|
| game_car[].mph | mdrive.c | Car speed |
| game_car[].rpm | mdrive.c | Engine RPM |
| game_car[].place | checkpoint.c | Race position (0-7) |
| game_car[].dr_pos[] | reckon.c | Dead-reckoned position |
| man_gear | mdrive.c | Current gear |
| play_time | game.c | Remaining game time (ms) |
| trackno | game.c | Current track index |
| gstate | game.c | Game state enum |

## N64 Equivalents (from game.c analysis)

| N64 Function | Arcade Equivalent | Notes |
|--------------|-------------------|-------|
| hud_setup() | InitHUD() | HUD initialization |
| hud_init() | ShowHUD() | Create HUD elements |
| hud_speed_display() | AnimateSpeed() | Speed digit updates |
| hud_full_update() | Multiple animate funcs | Per-frame HUD update |
| draw_text() | UpdateNumber() | Text/digit rendering |
| draw_ui_element() | SelectBlit() | Element positioning |
| countdown_handler() | AnimateCTimer() | Countdown timer |
| timer_init() | - | Timer initialization |
| timer_tick() | - | Timer update |
| lap_record_time() | PassedCP() | Lap completion |
| race_calc_positions() | CheckCPs() | Position calculation |

## N64 HUD-Related Globals

From `src/game/game.c`:
```c
extern s32 message_timer;
extern s32 countdown_timer;
extern s32 lap_count_selection;
extern u32 best_lap_times[12];
extern s32 current_lap_time;
extern s32 unlock_display_timer;
extern s32 ui_cursor_state;
extern s32 ui_cursor_frame;
extern f32 shake_timer;
extern s32 message_active;  // HUD enabled flag
```

## Game States (Shared)

Both arcade and N64 use the same state machine:
```c
enum GState {
    ATTRACT,    // 0 - Demo/attract mode
    TRKSEL,     // 1 - Track selection
    CARSEL,     // 2 - Car selection
    PLAYGAME,   // 3 - Active gameplay
    ENDGAME,    // 4 - Game ending
    GAMEOVER,   // 5 - Game over screen
    HISCORE,    // 6 - High score entry
    PREPLAY,    // 7 - Pre-race setup
    PREPLAY2,   // 8 - Final pre-race
    COUNTDOWN,  // 9 - Race countdown
};
```

## Implementation Notes

1. **Blit System**: The arcade uses a linked-list based blit manager with automatic z-sorting and animation callbacks. The N64 likely uses a simpler RDP-based 2D rendering approach.

2. **Animation**: Each HUD element has an `AnimFunc` callback that runs every frame to update position, visibility, and texture selection.

3. **Metrics**: The arcade supports MPH/KPH toggle via `metric` variable.

4. **Multiplayer**: HUD elements like markers and dots support up to 8 players (MAX_LINKS).

5. **Track-Specific**: The track map uses per-track coordinate mapping tables.

## See Also

- `reference/repos/rushtherock/game/hud.c` - Main arcade HUD source
- `reference/repos/rushtherock/game/hud.h` - HUD definitions
- `reference/repos/rushtherock/LIB/blit.h` - Blit structure definition
- `reference/repos/rushtherock/game/checkpoint.c` - Race position logic
- `src/game/game.c` - N64 HUD functions
