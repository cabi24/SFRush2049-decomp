# Attract Mode Flow Documentation

## Overview

Attract mode is the idle demonstration state that runs when no player is actively engaged with the game. It cycles through various screens including title logos, demo gameplay, high score displays, and "Press Start" prompts to attract players to the machine.

This document describes the attract mode implementation in San Francisco Rush 2049, comparing the N64 version with the arcade source code from Rush: The Rock (`reference/repos/rushtherock/game/attract.c`).

## 1. Attract Mode State Machine

### Arcade State Machine (AttractMode enum)

The arcade version uses a comprehensive `AttractMode` enum defined in `attract.h`:

```c
typedef enum AttractMode {
    ATR_HSSETUP=-1,     /* High score setup */
    ATR_HSENTRY=0,      /* High score entry */
    ATR_HISCORE1,       /* High score table track 1 */
    ATR_ATARILOGO,      /* Atari logo animation */
    ATR_HISCORE2,       /* High score table track 2 */
    ATR_MOVIE5,         /* Rush logo movie */
    ATR_HISCORE3,       /* High score table track 3 */
    ATR_TEAM,           /* Team credits */
    ATR_OTEAM,          /* Old team credits */
    ATR_ATARILOGO2,     /* Atari logo 2 */
    ATR_ADVERT,         /* Advertisement screen */
    ATR_MOVIE2,         /* Car 1 rotating movie */
    ATR_HISCORE4,       /* High score table track 4 */
    ATR_MIRROR,         /* Mirror static screen */
    ATR_HISCORE5,       /* High score table track 5 */
    ATR_MOVIE52,        /* Rush logo variant */
    ATR_HISCORE6,       /* High score table track 6 */
    ATR_OCREDITS,       /* Old credits screen */
    ATR_3DFXLOGO,       /* 3dfx logo */
    ATR_ATARILOGO3,     /* Atari logo 3 */
    ATR_HISCORE7,       /* High score table track 7 */
    ATR_MOVIE4,         /* Promotional movie */
    ATR_DEMO,           /* Demo gameplay 1 */
    ATR_CREDITS,        /* Credits screen */
    ATR_MOVIE53,        /* Rush logo variant 3 */
    ATR_ADVISORY,       /* Parental advisory */
    ATR_MOVIE3,         /* Car 2 rotating movie */
    ATR_DEMO2,          /* Demo gameplay 2 */
    ATR_CARS,           /* Cars showcase */
    /* Special modes */
    ATR_TOURNEY,        /* Tournament setup */
    ATR_TRANSIT,        /* Transition screen */
    ATR_JOIN,           /* "Join Now" prompt */
    ATR_GAMESTAT,       /* Game statistics */
    NUM_ATTRACTS
} AttractMode;
```

### N64 State Machine

The N64 version uses a simplified attract state system:

| N64 Address | Function Name | Description |
|-------------|---------------|-------------|
| 0x800DB81C | `attract_handler` | Main attract mode state machine (196 bytes) |
| 0x800DC88C | `attract_mode_start` | Initialize attract mode (1272 bytes) |
| 0x800DC99C | `attract_demo_play` | Demo race playback (1016 bytes) |
| 0x800DCD94 | `attract_idle_check` | Idle timeout detection (96 bytes) |
| 0x800DCDF4 | `attract_video_play` | Video/FMV playback (732 bytes) |
| 0x800DD0D0 | `attract_sequence_update` | State transition logic (988 bytes) |
| 0x800DD4AC | `title_screen` | Title screen display (2816 bytes) |
| 0x800EDDC0 | `attract_or_transition` | Attract/transition rendering |

### State Transition Flow

```
ATTRACT (attract_handler)
    |
    +-> Title Screen (attract_mode_start)
    |       |
    |       +-> Demo Playback (attract_demo_play)
    |       |       |
    |       |       +-> High Scores
    |       |               |
    |       +---------------+
    |
    +-> [Start Pressed] -> TRKSEL (Track Select)
```

## 2. Demo Playback System

### Arcade Demo System

The arcade uses `play_demogame()` to handle demo playback:

```c
BOOL play_demogame(BOOL show, BOOL allow_sound, S16 trk, BOOL canAbort) {
    if (show) {
        if (!gDemoInited) {
            gDemoInited = true;
            demo_game = 1;
            fake_init(trk);         /* Initialize fake game state */

            if (allow_sound) {
                check_sound_time(Initialize);
                attract_effects = attract_effects_save;
            } else {
                attract_effects = false;
            }

            preplay(attractFunc == ATR_DEMO || attractFunc == ATR_DEMO2, canAbort);
            Update_MDrive();        /* Update car physics */
            do_car_setup();
            init_cp_time();         /* Initialize checkpoint times */
        }
        playgame();                 /* Run actual game logic */
    } else {
        gDemoInited = false;
        demo_endgame();
        demo_game = 0;
    }
    return true;
}
```

**Key features:**
- Runs actual game code with `demo_game = 1` flag
- Uses pre-recorded input data
- Can play demos for specific tracks (`trk` parameter)
- Sound effects can be enabled/disabled via `allow_sound`
- Duration: 35 seconds per demo (configurable via `gAttractTimes[ATR_DEMO]`)

### N64 Demo System

Located at `attract_demo_play` (0x800DC99C):

```c
void attract_demo_play(void) {
    s32 frame = demo_playback_frame;
    u8 *inputData = demo_input_data;

    if (inputData == NULL) return;

    /* Read demo input for this frame */
    u8 demoInput = inputData[frame];

    /* Apply to player 0 car */
    f32 *throttle = &game_car.throttle;
    f32 *steering = &game_car.steering;
    f32 *brake = &game_car.brake;

    /* Decode demo input byte */
    *throttle = ((demoInput >> 4) & 0x0F) / 15.0f;
    *steering = (((demoInput & 0x0F) - 8) / 7.0f);
    *brake = (demoInput & 0x80) ? 1.0f : 0.0f;

    frame++;
    demo_playback_frame = frame;

    /* Check for demo end (0xFF marker) */
    if (inputData[frame] == 0xFF) {
        demo_playback_frame = 0;
        gstate_byte = 0;  /* Return to attract */
    }
}
```

**Demo Input Format:**
| Bits | Purpose |
|------|---------|
| 7 | Brake flag |
| 6-4 | Throttle (0-15) |
| 3-0 | Steering (-8 to +7) |
| 0xFF | End-of-demo marker |

## 3. High Score Display Sequence

### Arcade High Score System

The arcade cycles through multiple high score tables (one per track):

```c
/* In InitAttract() */
gAttractTimes[ATR_HSENTRY] = 20;  /* 20 seconds for name entry */
gAttractTimes[ATR_HISCORE1] = 20; /* 20 seconds per track display */
gAttractTimes[ATR_HISCORE2] = 20;
gAttractTimes[ATR_HISCORE3] = 20;
gAttractTimes[ATR_HISCORE4] = 20;
gAttractTimes[ATR_HISCORE5] = 20;
gAttractTimes[ATR_HISCORE6] = 20;
gAttractTimes[ATR_HISCORE7] = 20;
```

High scores are displayed via `ShowHiScore(show, track_num)` and rendered with blinking animations using `AnimateBlink()`.

### N64 High Score Display

The N64 version simplifies this with a single high score state that rotates through tracks:

```c
/* In attract_mode_start at 0x800DC88C */
case 2:  /* High scores state */
    draw_fill_rect(0, 0, 320, 240, 0xFF101030);
    draw_text(105, 40, "HIGH SCORES", 0xFFFFFFFF);

    if (timer > 300) {  /* 5 seconds */
        state = 0;      /* Back to title */
        timer = 0;
    }
    break;
```

### Function Mapping

| Purpose | Arcade Function | N64 Address |
|---------|-----------------|-------------|
| Display high scores | `ShowHiScore()` | Part of 0x800DC88C |
| Score entry | `ShowScoreEntry()` | 0x800FBF88 (`hiscore_handler`) |
| Animate score | `AnimateGOScore()` | Part of game rendering |

## 4. Transition to Gameplay

### Arcade Transition Flow

```c
/* In attract() - checking for game start */
if (gStartDuringLoad || chk_start()) {
    attract_music = false;
    explosion_state = false;

    /* Stop sounds and set game volume */
    SOUND(S_STOP_ALL_SNDS);
    SOUND(S_GAME_MODE);

    /* Remove current screen, show transition */
    ShowAttract(gLastFunc, false);
    ShowAttract(attractFunc = ATR_TRANSIT, true);

    /* Reset linking variables */
    gSimID = 0;
    gTrackLocked = 0;
    demo_game = 0;
    gDemoInited = false;

    /* Proceed to track selection */
    /* gstate transitions to TRKSEL */
}
```

The transition uses `ShowTransit()` which displays animated "RUSH" letters while loading the track selection world.

### N64 Transition

```c
/* At 0x800DC88C - attract_mode_start */
/* Any input exits attract mode */
if (input != 0) {
    attract_state = 0;
    attract_timer = 0;
    gstate_byte = 1;  /* Go to title/menu */
    return;
}
```

### Main Game State Machine Integration

From arcade `game.c`:

```c
switch (gstate) {
    case ATTRACT:
        attract();      /* Run attract mode */
        break;

    case TRKSEL:
        TrackSel();     /* Track selection */
        break;

    case CARSEL:
        CarSel();       /* Car selection */
        break;

    case PREPLAY:
        preplay(false, false);
        break;

    case COUNTDOWN:
        CountDown();
        break;

    case PREPLAY2:
        /* Final setup before race */
        gstate = PLAYGAME;
        break;

    case PLAYGAME:
        playgame();     /* Active racing */
        break;

    case ENDGAME:
        endgame();
        break;

    case GAMEOVER:
        gameover();
        break;

    case HISCORE:
        hiscore();
        break;
}
```

## 5. Timeout Handling

### Arcade Timeout System

The arcade uses a countdown timer system:

```c
/* Check for timeout */
if (TimeOut() || attractFunc == ATR_HSENTRY || raceAvailable()) {
    ResetJoinIn();

    /* Advance to next attract state */
    do {
        if (++attractFunc >= ATR_TOURNEY) {
            attractFunc = ATR_HSENTRY + 1;
        }
    } while (/* skip conditions */);

    SetCountdownTimer(gAttractTimes[attractFunc] * ONE_SEC);
}
```

**Timeout durations (in seconds):**

| State | Duration |
|-------|----------|
| ATR_DEMO/ATR_DEMO2 | 35 |
| ATR_HISCORE* | 20 |
| ATR_ATARILOGO* | 10 |
| ATR_MOVIE4 | 10 |
| ATR_MOVIE5/52/53 | 20 |
| ATR_MOVIE2/3 | 6 |
| ATR_CREDITS/OCREDITS | 8 |
| ATR_TEAM | 10 |
| ATR_OTEAM | 5 |
| ATR_JOIN | 15 |
| ATR_3DFXLOGO | 3 |
| ATR_ADVISORY | 8 |

### N64 Timeout System

```c
/* At 0x800DCD94 - attract_idle_check */
#define IDLE_TIMEOUT (30 * 60)  /* 30 seconds at 60fps */

s32 attract_idle_check(void) {
    s32 input = controller_get_input(sound_handles);

    if (input != 0) {
        idle_frame_counter = 0;
        return 0;
    }

    idle_frame_counter++;

    if (idle_frame_counter >= IDLE_TIMEOUT) {
        idle_frame_counter = 0;
        return 1;  /* Idle timeout reached */
    }

    return 0;
}
```

## 6. N64 vs Arcade Differences

### Major Differences

| Feature | Arcade | N64 |
|---------|--------|-----|
| **Number of attract states** | 30+ states | ~5 simplified states |
| **FMV playback** | Full motion video | Limited/no FMV |
| **Linking support** | Multi-cab networking, "Join Now" | Single player only |
| **Credits system** | Coin-op with fractional credits | N/A (cartridge game) |
| **Tournament mode** | Full tournament support | Not present |
| **Demo tracks** | 2 separate demo sequences | Rotating demo cycle |
| **3dfx branding** | 3dfx logo screen | Removed |
| **Resolution** | 512x384 | 320x240 |

### Removed Features on N64

1. **Join Now System**: Arcade allows players to "join" an in-progress game on linked cabinets
2. **Tournament Mode**: Arcade supports tournament brackets
3. **Credit Display**: Coin counting and credit fractions
4. **External Start Button**: Arcade supports external start switches
5. **Multicast Networking**: Arcade cabinet linking

### Simplified Features on N64

1. **High Score Tables**: Single rotating display vs. per-track tables
2. **Screen Sequence**: Fewer attract screens
3. **Animation Blits**: Simpler 2D overlay system
4. **Sound Control**: Unified N64 audio system

### N64-Specific Additions

1. **Controller Input Checking**: N64 controller polling
2. **Memory Constraints**: More aggressive memory management
3. **Save Data**: Controller Pak support for high scores
4. **Resolution Scaling**: Adapted UI for lower resolution

## Function Address Summary

### Attract Mode Functions

| N64 Address | Symbol Name | Arcade Equivalent | Size |
|-------------|-------------|-------------------|------|
| 0x800DB81C | `attract_handler` | `attract()` | 196 bytes |
| 0x800DC88C | `attract_mode_start` | `InitAttract()` + state loop | 1272 bytes |
| 0x800DC99C | `attract_demo_play` | `play_demogame()` | 1016 bytes |
| 0x800DCD94 | `attract_idle_check` | `TimeOut()` | 96 bytes |
| 0x800DCDF4 | `attract_video_play` | `AttractMovie()` | 732 bytes |
| 0x800DD0D0 | `attract_sequence_update` | `ShowAttract()` state machine | 988 bytes |
| 0x800DD4AC | `title_screen` | `ShowLogo()` + `ShowJoin()` | 2816 bytes |
| 0x800EDDC0 | `attract_or_transition` | N/A | Variable |

### Related Global Variables

| N64 Address | Purpose | Arcade Equivalent |
|-------------|---------|-------------------|
| 0x801146EC | `gstate_byte` | `gstate` |
| 0x80114XXX | `attract_state` | `attractFunc` |
| 0x80114XXX | `attract_timer` | Countdown timer |
| 0x80114XXX | `demo_playback_frame` | Demo frame counter |
| 0x80114XXX | `demo_input_data` | Demo input buffer |

## References

- Arcade source: `/reference/repos/rushtherock/game/attract.c` (2100+ lines)
- Arcade header: `/reference/repos/rushtherock/game/attract.h`
- N64 game.c implementation: `/src/game/game.c`
- N64 attract.c stub: `/src/game/attract.c`
- Symbol addresses: `/symbol_addrs.us.txt`
