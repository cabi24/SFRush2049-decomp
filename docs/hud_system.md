# Rush 2049 N64 HUD System Documentation

This document covers the Heads-Up Display (HUD) system for the Rush 2049 N64 decompilation project, with analysis of both the arcade source (`reference/repos/rushtherock/game/hud.c`) and the N64 implementation.

## Table of Contents

1. [Overview](#overview)
2. [HUD Element Layout and Positioning](#hud-element-layout-and-positioning)
3. [Speedometer Rendering](#speedometer-rendering)
4. [Lap Timer Display](#lap-timer-display)
5. [Position Indicator](#position-indicator)
6. [Minimap/Radar System](#minimapradar-system)
7. [Split-Screen HUD Adjustments](#split-screen-hud-adjustments)
8. [N64 vs Arcade Differences](#n64-vs-arcade-differences)
9. [Function Mapping](#function-mapping)

---

## Overview

The HUD system displays real-time racing information to the player during gameplay. The arcade version uses a "blit" system (textured 2D sprites with animation callbacks), while the N64 version adapts this for the Reality Display Processor (RDP).

### Arcade Architecture

The arcade HUD uses a `MULTIBLIT` table-driven approach:
- Each HUD element is defined in a static table with position, texture name, and animation callback
- Elements are created via `NewMultiBlit()` and updated per-frame via animation functions
- The `BLIT` structure contains position, size, alpha, Z-order, and animation state

### N64 Architecture

The N64 HUD uses:
- `HudState` structure to track current display values
- RDP commands for 2D sprite rendering
- Viewport-aware positioning for split-screen modes
- Frame-based timing instead of millisecond-based

---

## HUD Element Layout and Positioning

### Arcade Screen Coordinates

The arcade uses safe-area constants defined in `hud.h`:

```c
#define SCR_W           (VIS_H_PIX)         /* Screen width (typically 512) */
#define SCR_H           (VIS_V_PIX)         /* Screen height (typically 384) */
#define SCR_L           (5)                 /* Safe left edge */
#define SCR_R           (SCR_W - 10)        /* Safe right edge */
#define SCR_T           (5)                 /* Safe top edge */
#define SCR_B           (SCR_H - 5)         /* Safe bottom edge */
#define SCR_MH          ((SCR_R - SCR_L)/2) /* Screen middle horizontal */
#define SCR_MV          ((SCR_B - SCR_T)/2) /* Screen middle vertical */
```

### Arcade HUD Layout (HudMultiBlit Table)

| Element | X Position | Y Position | Animation Function |
|---------|-----------|------------|-------------------|
| Player Markers (0-7) | Dynamic | Dynamic | `AnimateMarker` |
| Tachometer Left | SCR_MH - 69 | SCR_T - 7 | `AnimateTach` |
| Tachometer Right | SCR_MH + 59 | SCR_T - 7 | `AnimateTach` |
| MPH/KPH Label | SCR_MH - 15 | SCR_T + 20 | `AnimateTach` |
| Tach Needle | Dynamic | Dynamic | `AnimateNeedle` |
| Speed Digits (3) | SCR_MH - 33 to +11 | SCR_T | `AnimateSpeed` |
| Track Map (2 parts) | SCR_R - TrackWidth | SCR_T + 82 | `AnimateTrackMap` |
| Track Dots (8) | Dynamic | Dynamic | `AnimateDot` |
| Radar Frame | SCR_L (5) | SCR_T + 60 | `AnimateRadar` |
| Traffic Markers (8) | Dynamic | Dynamic | `AnimateTraffic` |
| Shifter Display | SCR_R - 50 | SCR_B - 64 | `AnimateShifter` |
| Countdown Timer (3) | SCR_R - 105 to -39 | SCR_T | `AnimateCTimer` |
| Position ("X of 8") | SCR_R - 37 | SCR_T + 40 | `AnimatePlace` |
| Elapsed Time Header | SCR_L + 1 | SCR_T | (static) |
| Elapsed Digits (5) | SCR_L + 2 to +54 | SCR_T to +6 | `AnimateElapsed` |
| Checkpoint Time | SCR_L + 11 to +65 | SCR_T + 27 to +33 | `AnimateCheckPt` |
| Wrong Way Arrows | SCR_MH +/- 128-170 | SCR_MV - 32 | `AnimateWrongWay` |
| Bonus Message | SCR_MH - 128 | SCR_MV + 64 | `AnimateBonus` |
| Lap Digits | Dynamic (near finish) | Dynamic | `AnimateLapDigit` |
| Finish Flag | Dynamic | Dynamic | `AnimateFlag` |

### N64 HUD Layout

The N64 uses constants from `include/game/hud.h`:

```c
#define HUD_SPEED_X         280     /* Right side of screen */
#define HUD_SPEED_Y         200     /* Lower portion */
#define HUD_LAPS_X          30      /* Upper left */
#define HUD_LAPS_Y          30
#define HUD_POS_X           160     /* Upper center */
#define HUD_POS_Y           30
#define HUD_TIMER_X         160     /* Lower center */
#define HUD_TIMER_Y         220
#define HUD_RADAR_X         30      /* Lower left */
#define HUD_RADAR_Y         180
#define HUD_RADAR_SIZE      50
```

---

## Speedometer Rendering

### Arcade Implementation (`AnimateSpeed`)

The arcade speedometer consists of:
1. **Tachometer background** - Two 128x texture halves (`NEWTACH15L`, `NEWTACH15R`)
2. **Tach needle** - Animated dot positioned via sine/cosine
3. **Speed digits** - Three sprite digits (hundreds, tens, ones)
4. **Unit label** - MPH or KPH based on `metric` flag

```c
static int AnimateSpeed(BLIT *blt) {
    S16 digit, type;

    blt->AnimID = (blt->AnimID & 0xF) + (SpeedDigit << 4);
    blt->X = SCR_MH - 33 + 22 * (blt->AnimID & 0xF);  /* 22px digit spacing */

    digit = hud_mph;
    if (digit < 0) digit = -digit;

    switch (blt->AnimID & 0xF) {
        case 0: digit /= 100; break;  /* Hundreds */
        case 1: digit /= 10;  break;  /* Tens */
        default: break;               /* Ones */
    }

    return UpdateNumber(blt, (char)(digit % 10));
}
```

### Speed Calculation

Speed is derived from the car's `mph` field with metric conversion:

```c
hud_mph = (S16)game_car[this_node].mph;
if (metric) hud_mph *= 1.61;  /* Convert to KPH */
```

### Tachometer Needle (`AnimateNeedle`)

The needle position is calculated from engine RPM:

```c
temp = (game_car[this_node].rpm * 1.35) / 1000.0;  /* 1.35 = fake boost */
if (temp > 10.0) temp = 10.0;

angle = DEG_TO_RADS * (180 + 35 + temp * (180.0 - (35*2))/11.0);
blt->X = SCR_MH - 4 + (60 * fcos(angle));
blt->Y = SCR_T - 7 - 24 - (60 * fsin(angle));
```

### N64 Speed Display

The N64 uses smoothed speed transitions:

```c
gHud.target_speed = (s16)(car->mph * 0.6818f);  /* fps to mph */
if (gHud.speed_display < gHud.target_speed) {
    gHud.speed_display += 2;  /* Smooth increase */
} else if (gHud.speed_display > gHud.target_speed) {
    gHud.speed_display -= 2;  /* Smooth decrease */
}
```

---

## Lap Timer Display

### Time Formats

The arcade `cvt_time()` function supports three formats:

| Format | Output | Use Case |
|--------|--------|----------|
| 'c' | SSS | Countdown timer (max 999 seconds) |
| 's' | M:SS | Elapsed time (seconds only) |
| 'h' | M:SS.hh | Elapsed time with hundredths |

```c
U8 cvt_time(S32 t, U8 *dest, char format) {
    S32 sec, min;

    if (t < 0) t = 0;

    switch (format) {
        case 'c':  /* Countdown: SSS */
            sec = t / 1000;  /* Input is milliseconds */
            if (sec > 999) sec = 999;
            dest[0] = (sec / 100) % 10;
            dest[1] = (sec / 10) % 10;
            dest[2] = sec % 10;
            break;

        case 'h':  /* With hundredths */
            dest[3] = (t / 100) % 10;
            dest[4] = (t / 10) % 10;
            /* Fall through */

        case 's':  /* Seconds only */
            sec = t / 1000;
            min = (sec / 60) % 60;
            sec %= 60;
            dest[0] = min % 10;
            dest[1] = sec / 10;
            dest[2] = sec % 10;
            break;
    }
}
```

### Elapsed Time Display (`AnimateElapsed`)

Shows race elapsed time in MM:SS.hh format, updated via `GetElapsedTime()`:

```c
if (game_car[this_node].place_locked == 1)
    t = game_car[this_node].score;  /* Frozen at finish */
else
    t = GetElapsedTime();

cvt_time(t, TrackTimeStr, 'h');
```

### Countdown Timer (`AnimateCTimer`)

Shows remaining time with audio beep when under 6 seconds:

```c
sec = GetCountdownTime() / 1000;
if ((sec < 6) && (old_sec != sec)) {
    old_sec = sec;
    if (demo_game == 0) SOUND(S_BEEP1);
}

/* Flash when under 5 seconds */
if (sec <= 5 && (IRQTIME & 0x100))
    Hidden(blt, true);
```

### Checkpoint Time Bonus (`AnimateCheckPt`)

Displays time bonus when hitting checkpoints:

```c
#define CHECKPT_TIME (3 * ONE_SEC)  /* 3 second display duration */

void ShowCheckpointTime(S32 delta) {
    gCheckPtTimeout = IRQTIME + CHECKPT_TIME;
    cvt_time(GetElapsedTime(), CheckpointStr, 'h');
}
```

The display fades out during the last second:

```c
remain = gCheckPtTimeout - IRQTIME;
blt->Alpha = (remain > ONE_SEC) ? 0 : 255 - 255 * remain / ONE_SEC;
```

---

## Position Indicator

### Race Position Display (`AnimatePlace`)

Shows current race position (1-8) with ordinal suffix:

```c
static int AnimatePlace(BLIT *blt) {
    char digit = game_car[this_node].place + 1;  /* 0-indexed to 1-indexed */

    if (digit < 1) digit = 1;
    if (digit > 8) digit = 8;

    /* Update arcade cabinet "Leader" lamp */
    if (digit == 1 && lastDigit != 1 && !demo_game)
        ctl_mod_latch(LA_LEADER);
    else if (lastDigit == 1 && digit != 1)
        ctl_mod_latch(~LA_LEADER);

    return UpdateNumber(blt, digit);
}
```

### Hidden Conditions

Position is hidden when:
- Solo mode with no drones: `solo_flag && !(gLink[gThisNode].flags & LINK_DRONES)`
- Car not visible: `!are_we_solid(this_node)`

### Arcade Position Format

Uses `OUTOF8` texture for "/ 8" suffix and `DIGIT8` for position number:
- Position at: SCR_R - 55, SCR_T + 40
- "/ 8" suffix at: SCR_R - 37, SCR_T + 40

### N64 Position Format

The N64 uses text formatting with ordinal suffixes:

```c
void hud_format_position(s32 position, char *buffer) {
    buffer[0] = '0' + position;
    switch (position) {
        case 1: strcpy(&buffer[1], "st"); break;  /* 1st */
        case 2: strcpy(&buffer[1], "nd"); break;  /* 2nd */
        case 3: strcpy(&buffer[1], "rd"); break;  /* 3rd */
        default: strcpy(&buffer[1], "th"); break; /* 4th-8th */
    }
}
```

---

## Minimap/Radar System

The arcade has two distinct map systems:
1. **Track Map** - Static overhead view with car positions as dots
2. **Radar** - Player-centered scrolling view showing nearby cars

### Track Map (`AnimateTrackMap`)

Each track has a dedicated texture and coordinate mapping:

```c
/* Per-track world-to-screen mapping */
static const F32 TrackMapping[MAX_TRACKS][2][4] = {
    /* Track 1 */
    { { 1996.686, -6510.893, -3557.542, 3124.747 },  /* World coords */
      { 0.0, 0.0, 101.0, 174.0 } },                   /* Screen coords */
    /* ... additional tracks ... */
};

static void set_track_mapping(BLIT *blt) {
    gMTrackX = (TrackMapping[trackno][1][2] - TrackMapping[trackno][1][0]) / 40 /
               (TrackMapping[trackno][0][2] - TrackMapping[trackno][0][0]);
    gTTrackX = TrackMapping[trackno][1][0] - gMTrackX * TrackMapping[trackno][0][0] +
               blt->X + TrackSlop[trackno][0];
    /* Similar for Y axis */
}
```

### Track Dots (`AnimateDot`)

Car positions on the track map with color coding:

```c
blt->X = (U16)(c->dr_pos[0] * 40.0 * gMTrackX + gTTrackX);
blt->Y = (U16)(c->dr_pos[2] * 40.0 * gMTrackY + gTTrackY);

/* Color/priority by car type */
if (slot == this_node) {
    blt->Z = 0.0;               /* Draw on top */
    SelectBlit(blt, 1+slot, 7, 7);  /* Player color */
} else if (model[slot].drone_type == HUMAN) {
    blt->Z = 0.1;               /* Middle layer */
    SelectBlit(blt, 1+slot, 7, 7);  /* Human player color */
} else {
    blt->Z = 0.2;               /* Bottom layer */
    SelectBlit(blt, 0, 7, 7);   /* Drone color */
}
```

### Flash States

Dots flash when the car is:
- Dead (`we_died`)
- Crashed (`crashflag`)
- Not collidable
- Hidden
- Resurrecting

```c
flash = c->we_died || m->crashflag || !m->collidable || m->hide_car ||
        !(m->resurrect.moving_state == -1 || mpath_edit || gFlyMode);
blt->Alpha = (IRQTIME & 0x100) && flash ? 0xA0 : 0x00;
```

### Radar System (`AnimateTraffic`)

Player-centered radar showing relative car positions:

```c
#define RADAR_X         SCR_L           /* Left edge of screen */
#define RADAR_Y         (SCR_T + 60)    /* 60px from top */
#define RADAR_WIDE      64.0            /* Width of radar */
#define RADAR_HIGH      256.0           /* Height of radar (tall strip) */
#define RADAR_BWIDE     3.0             /* Border width */

static void set_radar_mapping(F32 lx, F32 ly, F32 hx, F32 hy) {
    gMRadarX = (RADAR_WIDE - RADAR_BWIDE*2) / (hx - lx);
    gMRadarY = (RADAR_HIGH - RADAR_BHIGH*2) / (hy - ly);
    gTRadarX = RADAR_X + RADAR_CENTER_X - TCAR_W/2;
    gTRadarY = RADAR_Y + RADAR_CENTER_Y - TCAR_H/2;
}
```

### Radar Car Icons

Cars show orientation (8 directions) based on relative heading:

```c
/* Get relative direction */
ldir[0] = gc->dr_uvs[0][2];  /* Forward direction */
frwtobod(ldir, rdir, game_car[this_node].dr_uvs[0]);

/* Determine angle bucket (0-3, mirrored to 0-7) */
if (rdir[2] > 0.966) angle = 3;       /* 0-15 degrees */
else if (rdir[2] > 0.707) angle = 2;  /* 15-45 degrees */
else if (rdir[2] > 0.259) angle = 1;  /* 45-75 degrees */
else angle = 0;                        /* 75-90 degrees */

if (flip) angle = 7 - angle;  /* Mirror for other quadrant */
```

### N64 Radar Implementation

The N64 uses simplified world-to-radar mapping:

```c
void set_radar_mapping(f32 min_x, f32 max_x, f32 min_z, f32 max_z) {
    radar_min_x = min_x; radar_max_x = max_x;
    radar_min_z = min_z; radar_max_z = max_z;
    radar_scale_x = (f32)HUD_RADAR_SIZE / (max_x - min_x);
    radar_scale_z = (f32)HUD_RADAR_SIZE / (max_z - min_z);
}

void world_to_radar(f32 world_x, f32 world_z, f32 *radar_x, f32 *radar_y) {
    f32 rel_x = (world_x - radar_min_x) * radar_scale_x;
    f32 rel_z = (world_z - radar_min_z) * radar_scale_z;

    /* Clamp to radar bounds */
    rel_x = CLAMP(rel_x, 0.0f, HUD_RADAR_SIZE);
    rel_z = CLAMP(rel_z, 0.0f, HUD_RADAR_SIZE);

    *radar_x = HUD_RADAR_X + rel_x;
    *radar_y = HUD_RADAR_Y + HUD_RADAR_SIZE - rel_z;  /* Flip Z */
}
```

---

## Split-Screen HUD Adjustments

### N64 Viewport System

The N64 supports multiple players via viewport management (`src/game/display.c`):

```c
void viewport_setup(s32 mode, s32 player, s32 width, s32 height) {
    /* Mode determines split configuration */
    /* Player determines which viewport quadrant */

    gViewportXStart = (gReferenceDisplayState->x_start >> 16) & 0x3FF;
    gViewportXEnd = gReferenceDisplayState->x_start & 0x3FF;
    gViewportYStart = (gReferenceDisplayState->y_start >> 16) & 0x3FF;
    gViewportYEnd = gReferenceDisplayState->y_start & 0x3FF;

    if (mode >= 0) {
        gViewportXOffset = gViewportXOffsetTable[mode * 4];
        gViewportYOffset = gViewportYOffsetTable[mode * 4];
        gCurrentXOffset = gViewportXBoundsTable[mode * 4];
        gCurrentYOffset = gViewportYBoundsTable[mode * 4];

        viewport_scale((f32)width / (f32)gScreenWidth,
                       (f32)height / (f32)gScreenHeight);
    }
}
```

### HUD Scaling for Split-Screen

When in split-screen modes, HUD elements should:
1. Scale positions based on viewport bounds
2. Reduce element sizes proportionally
3. Hide non-essential elements (radar, track map) if viewport too small

### Arcade Solo Flag

The arcade hides certain elements when playing solo:

```c
/* Hide radar in solo mode without drones */
if (solo_flag && !(gLink[gThisNode].flags & LINK_DRONES))
    return Hidden(blt, 1);
```

---

## N64 vs Arcade Differences

### Timing System

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Time unit | Milliseconds (`ONE_SEC = 1000`) | Frames (`FRAMES_PER_SEC = 60`) |
| Timer source | `IRQTIME` hardware timer | Frame counter |
| Animation sync | `IRQTIME` bit-masking | Frame counter modulo |

### Rendering System

| Aspect | Arcade | N64 |
|--------|--------|-----|
| 2D sprites | `BLIT` structure with 3dfx | RDP rectangle commands |
| Texture loading | `MBOX_FindTexture()` | DMA from ROM |
| Alpha blending | Hardware blend modes | RDP combine modes |
| Animation | Callback per-blit | State machine per-element |

### Missing/Changed Features

| Feature | Arcade | N64 |
|---------|--------|-----|
| Radio display | CD player graphic | Likely removed |
| Shifter display | Full gear indicator | Simplified or removed |
| Cabinet lamps | `ctl_mod_latch()` | N/A |
| 3D tach needle | `MBOX_NewObject()` | 2D sprite |
| Dynamic texture reload | Runtime | Pre-loaded |

### Resolution Differences

| Platform | Resolution | Safe Area |
|----------|------------|-----------|
| Arcade | 512x384 | 5px borders |
| N64 (NTSC) | 320x240 | Variable |
| N64 (PAL) | 320x288 | Variable |

---

## Function Mapping

### Arcade to N64 Function Correspondence

| Arcade Function | Arcade Address | N64 Address | N64 Function |
|-----------------|----------------|-------------|--------------|
| `InitHUD` | - | 0x800C8B8C | `hud_setup` |
| `ShowHUD` | - | 0x800C8F00 | `hud_full_update` |
| `RemoveHUD` | - | - | (implicit) |
| `AnimateTach` | - | 0x800C8DA0 | `hud_tachometer_update` |
| `AnimateSpeed` | - | 0x800C84FC | `hud_speed_display` |
| `AnimateElapsed` | - | 0x800C8920 | `draw_timer` |
| `AnimatePlace` | - | 0x800C8864 | `draw_position` |
| `AnimateRadar` | - | 0x800C9404 | `draw_minimap` |
| `AnimateDot` | - | 0x800E2A00 | `hud_minimap_render` |
| `AnimateCheckPt` | - | 0x800CA308 | `draw_message` |
| `cvt_time` | - | (ported) | `cvt_time` |
| `UpdateNumber` | - | 0x800C760C | `draw_number` |
| `Hidden` | - | - | `hud_hide`/`hud_show` |

### Key N64 HUD Symbols

From `symbol_addrs.us.txt`:

```
hud_update = 0x80095600          # Per-frame HUD update
hud_setup = 0x800C8B8C           # HUD initialization
hud_speed_display = 0x800C84FC   # Speedometer rendering
hud_element_render = 0x800C7350  # Generic element draw
hud_tachometer_update = 0x800C8DA0
hud_full_update = 0x800C8F00
hud_nitro_update = 0x800C8E50    # N64-specific (nitro gauge)
hud_fade_effect = 0x800C8F80
hud_render = 0x800CADA4          # Main HUD render (2468 bytes)
hud_render_main = 0x800FB5F4
hud_timer_display = 0x800FB928

# Drawing functions
draw_number = 0x800C760C
draw_speedometer = 0x800C7818
draw_position = 0x800C8864
draw_timer = 0x800C8920
draw_minimap = 0x800C9404
draw_message = 0x800CA308
draw_lap_counter = 0x800C7ED4

# HUD element renderers
hud_speedometer_render = 0x800E2000
hud_tachometer_render = 0x800E2200
hud_lap_render = 0x800E2400
hud_position_render = 0x800E2600
hud_timer_render = 0x800E2800
hud_minimap_render = 0x800E2A00
hud_stunt_render = 0x800E2C00    # N64-specific (stunt mode)
```

### Key HUD State Variables

```
gHudEnabled = 0x80037900         # HUD visible flag
gHudBlitList = 0x80037904        # Element list pointer
gHudBlitCount = 0x80037908       # Number of elements
gSpeedometerValue = 0x8003790C   # Current speed (s16)
gLapDisplayValue = 0x80037914    # Current lap (s8)
gLapTotalValue = 0x80037918      # Total laps (s8)
gPositionDisplayValue = 0x8003791C  # Race position (s8)
gTimerDisplayValue = 0x80037920  # Timer in msec (s32)
gMinimapEnabled = 0x80037928     # Minimap visible
gMinimapCenterX = 0x8003792C     # Minimap X center
gMinimapCenterY = 0x80037930     # Minimap Y center
```

---

## Source Files

### Arcade Source
- `reference/repos/rushtherock/game/hud.c` - Main HUD implementation (1549 lines)
- `reference/repos/rushtherock/game/hud.h` - HUD definitions
- `reference/repos/rushtherock/game/shud.c` - Secondary HUD copy (identical)
- `reference/repos/rushtherock/game/globals.h` - Global state variables

### N64 Source
- `src/game/hud.c` - N64 HUD implementation (659 lines)
- `include/game/hud.h` - N64 HUD header (135 lines)
- `src/game/display.c` - Viewport and display management

---

## Notes for Decompilation

1. **Digit Types**: The arcade uses 10 different digit fonts (`DigitType` enum). Ensure matching font indices when decompiling number rendering.

2. **Blit IDs**: The `AnimID` field encodes multiple values:
   - Lower 4 bits: Element index within group
   - Upper 4 bits: Font/type selector

3. **Z-Order**: The arcade uses floating-point Z for draw order (0.0 = front, higher = back).

4. **Flash Timing**: Uses `IRQTIME & 0x100` for ~256ms toggle, may need frame-based equivalent.

5. **Dead Reckoning**: Positions use `dr_pos` (dead-reckoned position) for smoother network play, not raw `pos`.
