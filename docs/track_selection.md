# Rush 2049 N64 Track Selection System

This document provides comprehensive documentation of the track selection system in San Francisco Rush 2049 for Nintendo 64, including comparison with the arcade (Rush: The Rock) source code.

## Table of Contents

1. [Track List and IDs](#track-list-and-ids)
2. [Track Unlock Conditions](#track-unlock-conditions)
3. [Track Selection UI Flow](#track-selection-ui-flow)
4. [Track Preview System](#track-preview-system)
5. [Track Data Loading Process](#track-data-loading-process)
6. [Mirror Mode and Difficulty Settings](#mirror-mode-and-difficulty-settings)
7. [Stunt and Battle Arenas](#stunt-and-battle-arenas)
8. [Comparison to Arcade TrackSel()](#comparison-to-arcade-tracksel)
9. [Key Source References](#key-source-references)

---

## Track List and IDs

### Race Tracks (IDs 0-11)

The N64 version features 12 race tracks, all based on San Francisco locations:

| Track ID | Track Name      | Description                          |
|----------|-----------------|--------------------------------------|
| 0        | MARINA          | Marina District, waterfront course   |
| 1        | HAIGHT          | Haight-Ashbury neighborhood          |
| 2        | SOMA            | South of Market area                 |
| 3        | MISSION         | Mission District                     |
| 4        | NOB HILL        | Nob Hill with steep gradients        |
| 5        | EMBARCADERO     | Waterfront Embarcadero area          |
| 6        | PRESIDIO        | Presidio park area                   |
| 7        | SUNSET          | Sunset District                      |
| 8        | RICHMOND        | Richmond District (bonus track)      |
| 9        | BAY BRIDGE      | Bay Bridge crossing (bonus track)    |
| 10       | GOLDEN GATE     | Golden Gate Bridge (bonus track)     |
| 11       | ALCATRAZ        | Alcatraz Island (ultimate unlock)    |

**Note**: The track list shown in some menus shows a smaller subset (6 tracks in `menu_track_select`), suggesting different track lists for different game modes.

**Alternative Track Names** (found in some menu contexts):
- Position 5: "CHINATOWN" (alternate for EMBARCADERO)
- Position 6: "FISHERMAN'S WHARF" (alternate for PRESIDIO)

### Stunt Arenas (IDs 100-107)

Stunt mode uses arena IDs offset by 100:

| Arena ID | Arena Name    | Notes                        |
|----------|---------------|------------------------------|
| 100      | ALCATRAZ      | Island arena                 |
| 101      | METRO         | Urban subway area            |
| 102      | PIER 39       | Tourist pier location        |
| 103      | GOLDEN GATE   | Bridge-based arena           |
| 104      | TWIN PEAKS    | Hilltop area                 |
| 105      | DOWNTOWN      | City center                  |
| 106      | OBSTACLE      | Obstacle course              |
| 107      | FREEFORM      | Open freestyle area          |

### Battle Arenas (IDs 200-203)

Battle mode uses arena IDs offset by 200:

| Arena ID | Arena Name  |
|----------|-------------|
| 200-203  | 4 arenas    |

---

## Track Unlock Conditions

The unlock system is managed by `unlock_check()` and `unlock_trigger()` functions in `src/game/game.c`.

### Base Unlock State

```c
/* Basic tracks and cars always unlocked */
if (unlockId < 4) {
    return 1;  /* First 4 tracks unlocked */
}
```

**Initial Unlocks:**
- Tracks 0-3 (Marina, Haight, Soma, Mission) - Always unlocked
- Cars 0-3 - Always unlocked (tracks 12-15 in unlock ID space)

### Unlock ID Ranges

| ID Range | Content Type      |
|----------|-------------------|
| 0-11     | Race tracks       |
| 12-15    | Base cars         |
| 16-19    | Bonus cars        |
| 20-31    | Track bonuses     |
| 32-39    | Stunt arenas      |
| 40-47    | Paint jobs/colors |
| 48-55    | Cheats            |
| 56+      | Misc unlocks      |

### Track Unlock Methods

#### 1. Sequential Completion
```c
/* Check if previous track has been completed */
if (trackId > 0) {
    if (best_race_times[trackId - 1] > 0) {
        return 1;
    }
}
```
Completing a track (recording a best time) unlocks the next track.

#### 2. Finishing First
```c
/* Finishing first on a track unlocks next track */
if (player_status[0] == 1 && player_finished_flag[0]) {
    s32 nextTrack = trackId + 1;
    if (nextTrack < 12 && !unlock_check(nextTrack)) {
        unlock_trigger(nextTrack);
    }
}
```

#### 3. Bonus Track Unlocks (Tracks 8-10)
```c
/* Bonus tracks need special unlock conditions */
if (trackId >= 8 && trackId <= 10) {
    /* Unlock if all regular tracks completed */
    s32 allCompleted = 1;
    for (i = 0; i < 8; i++) {
        if (best_race_times[i] == 0) {
            allCompleted = 0;
            break;
        }
    }
    return allCompleted;
}
```
Bonus tracks (Richmond, Bay Bridge, Golden Gate) require completing all 8 regular tracks.

#### 4. Alcatraz Unlock (Track 11)
```c
/* Stunt arena unlocked with any 5 tracks completed */
if (trackId == 11) {
    s32 completedCount = 0;
    for (i = 0; i < 8; i++) {
        if (best_race_times[i] > 0) {
            completedCount++;
        }
    }
    return (completedCount >= 5);
}
```
Alcatraz requires completing at least 5 tracks.

#### 5. Perfect Lap Bonus Cars
```c
/* Perfect lap unlocks bonus car */
if (bestLap < track_par_times[trackId]) {
    s32 bonusCar = 16 + (trackId / 3);  /* Bonus cars 16-19 */
    if (!unlock_check(bonusCar)) {
        unlock_trigger(bonusCar);
    }
}
```
Beating a track's par time unlocks bonus cars (cars 16-19).

#### 6. Coin Collection Bonus
```c
/* All coins collected - trigger unlock */
if (coinCount >= totalCoins && totalCoins > 0) {
    if (coins_complete_flag == 0) {
        coins_complete_flag = 1;
        unlock_trigger(trackId + 20);  /* Unlock bonus for this track */
    }
}
```
Collecting all coins on a track unlocks track-specific bonuses.

---

## Track Selection UI Flow

### Game State Machine

The track selection uses the `gstate` variable with these states:

```c
typedef enum GState {
    ATTRACT,    /* 0 - Attract/demo mode */
    TRKSEL,     /* 1 - Track selection */
    CARSEL,     /* 2 - Car selection */
    PLAYGAME,   /* 3 - Active gameplay */
    ENDGAME,    /* 4 - Race finished */
    GAMEOVER,   /* 5 - Game over */
    HISCORE,    /* 6 - High score entry */
    PREPLAY,    /* 7 - Pre-race setup */
    PREPLAY2,   /* 8 - Final pre-race */
    COUNTDOWN   /* 9 - Starting countdown */
} GState;
```

### Main Menu Flow

1. **Mode Select** (`mode_select_setup`)
   - Race Mode (goes to track select)
   - Time Trial Mode (goes to track select)
   - Stunt Mode (goes to stunt arena select)
   - Battle Mode (goes to battle arena select)

2. **Track Selection** (`track_select_screen` at 0x800D000C)
   ```c
   void track_select_screen(void) {
       /* Handle D-pad input for track scrolling */
       if (input == 4) {  /* Up */
           selectedTrack--;
           if (selectedTrack < 0) selectedTrack = 11;
       } else if (input == 5) {  /* Down */
           selectedTrack++;
           if (selectedTrack > 11) selectedTrack = 0;
       }

       /* Check if track is unlocked */
       trackUnlocked = menu_update(selectedTrack);
       if (input == 1 && trackUnlocked) {  /* A button */
           trackno = selectedTrack;  /* Store selected track */
           /* Proceed to car select or race setup */
       }

       /* Render track list with scroll */
       for (i = 0; i < visibleTracks; i++) {
           s32 trackIdx = scrollOffset + i;
           s32 unlocked = menu_update(trackIdx);
           s32 alpha = (trackIdx == selectedTrack) ? 255 : 180;
           if (!unlocked) alpha = alpha / 2;
           /* Draw track name */
       }

       /* Render track preview */
       track_preview_render(selectedTrack);

       /* Render track info */
       track_info_display(selectedTrack);
   }
   ```

3. **Menu Transitions**
   - Track select -> Car select (`menu_transition(4)`)
   - Track select -> Race options (`menu_transition(3)`)
   - Back to mode select on B button

### Input Handling

```c
input = controller_get_input(sound_handles);

switch (input) {
    case 1:  /* A button - Select */
    case 2:  /* B button - Back */
    case 4:  /* Up */
    case 5:  /* Down */
    case 6:  /* Left */
    case 7:  /* Right */
}
```

---

## Track Preview System

### Track Preview Rendering

The `track_preview_render()` function (at 0x800D08E4) displays a thumbnail of the selected track:

```c
void track_preview_render(s32 trackId) {
    s32 previewX = 180;
    s32 previewY = 70;
    s32 previewW = 120;
    s32 previewH = 90;

    unlocked = menu_update(trackId);

    /* Draw preview frame */
    draw_ui_element(64, previewX - 2, previewY - 2, previewW + 4, previewH + 4, 255);

    /* Get track preview texture ID */
    textureId = 300 + trackId;  /* Preview textures start at 300 */

    if (unlocked) {
        /* Draw track preview texture */
        draw_texture(textureId, previewX, previewY, previewW, previewH, 255);
    } else {
        /* Draw locked placeholder */
        draw_ui_element(60, previewX, previewY, previewW, previewH, 128);
        draw_ui_element(61, previewX + 44, previewY + 29, 32, 32, 255);  /* Lock icon */
    }
}
```

### 3D Rotating Preview

The `menu_track_select()` function features a 3D rotating miniature:

```c
/* Draw track preview (3D rotating miniature) */
render_scene(*selectedTrack, previewAngle, 160, 80);
```

This renders a small 3D view of the track with a rotating camera angle.

### Track Info Display

```c
void track_info_display(s32 trackId) {
    if (!unlocked) {
        draw_text("LOCKED", infoX + 30, infoY + 20, 180);
        draw_text("Complete previous", infoX, infoY + 45, 120);
        draw_text("tracks to unlock", infoX, infoY + 60, 120);
        return;
    }

    /* Track difficulty */
    draw_text("DIFFICULTY:", infoX, infoY, 200);
    /* Draw difficulty bar */

    /* Best time */
    draw_text("BEST:", infoX, infoY + 40, 200);
    bestTime = best_race_times[trackId];
    if (bestTime > 0) {
        minutes = bestTime / 6000;
        seconds = (bestTime / 100) % 60;
        hundredths = bestTime % 100;
        /* Format and display time */
    } else {
        draw_text("--:--.--", infoX + 60, infoY + 40, 150);
    }
}
```

---

## Track Data Loading Process

### Loading Screen

The `loading_screen()` function (at 0x800FA9E4) displays progress during track loading:

```c
void loading_screen(f32 progress) {
    /* Draw background */
    draw_ui_element(70, 0, 0, 320, 240, 255);

    /* Draw progress bar */
    s32 barWidth = (s32)(200.0f * progress);
    draw_ui_element(71, 40, 140, barWidth, 20, 255);

    /* Track name being loaded */
    if (trackno >= 0 && trackno < 12) {
        char *trackNames[12] = {
            "MARINA", "HAIGHT", "SOMA", "MISSION",
            "NOB HILL", "EMBARCADERO", "PRESIDIO", "SUNSET",
            "RICHMOND", "BAY BRIDGE", "GOLDEN GATE", "ALCATRAZ"
        };
        draw_text(trackNames[trackno], 120, 70, 200);
    }
}
```

### Track Segment Loading

Tracks are loaded in segments from ROM using `track_segment_load()` (at 0x800A1648):

```c
void track_segment_load(s32 segmentId) {
    u32 *segmentTable;
    u32 romAddr, segmentSize;
    u8 *destAddr;
    s32 *loadedFlags;

    /* Track segment ROM table at 0x8016B000 */
    segmentTable = (u32 *)0x8016B000;

    /* Get segment info from table */
    romAddr = segmentTable[segmentId * 2];
    segmentSize = segmentTable[segmentId * 2 + 1];

    /* Destination in RAM */
    destAddr = (u8 *)(0x80200000 + segmentId * 0x40000);

    /* DMA from ROM */
    osPiStartDma(&dma_io_msg, OS_MESG_PRI_NORMAL, OS_READ,
                 romAddr, destAddr, segmentSize);
    osRecvMesg(&dma_done_queue, NULL, OS_MESG_BLOCK);

    /* Mark segment as loaded */
    loadedFlags = (s32 *)0x8016A000;
    loadedFlags[segmentId] = 1;

    /* If segment is compressed, decompress it */
    if (*(u8 *)destAddr == 0x1F && *(u8 *)(destAddr + 1) == 0x8B) {
        /* GZIP header detected - decompress in place */
        track_data_decompress(destAddr, destAddr, segmentSize);
    }
}
```

### Track Data Decompression

The `track_data_decompress()` function (at 0x800A1F18) handles GZIP/ZLIB/LZSS compressed track data:

```c
void track_data_decompress(void *dest, void *src, s32 size) {
    u8 *srcPtr, *destPtr;
    u8 *tempBuffer;
    s32 compressedSize;

    srcPtr = (u8 *)src;
    destPtr = (u8 *)dest;

    /* Check compression type */
    if (srcPtr[0] == 0x1F && srcPtr[1] == 0x8B) {
        /* GZIP format - use inflate */
        inflate_entry(destPtr, srcPtr, size);
    } else if (srcPtr[0] == 0x78) {
        /* ZLIB format */
        /* ... ZLIB decompression ... */
    } else {
        /* LZSS or raw data */
        lzss_decode(destPtr, srcPtr, size);
    }
}
```

### Track Collision Setup

After loading, track collision data is initialized:

```c
void track_collision_setup(void *trackData) {
    u8 *data;
    f32 *heightGrid;
    u8 *surfaceGrid;
    s32 gridSize;
    s32 cellSize;

    if (trackData == NULL) {
        return;
    }

    data = (u8 *)trackData;

    /* Track collision data format:
     * 0x00: Grid dimensions (32-bit)
     * 0x04: Cell size (32-bit)
     * 0x08: Height grid offset (32-bit)
     * 0x0C: Surface grid offset (32-bit)
     */
    gridSize = *(s32 *)(data + 0x00);  /* Usually 64x64 */
    cellSize = *(s32 *)(data + 0x04);  /* World units per cell */

    heightGrid = (f32 *)(data + *(s32 *)(data + 0x08));
    surfaceGrid = (u8 *)(data + *(s32 *)(data + 0x0C));
}
```

### Race Initialization

The `race_init()` function sets up the race after track loading:

```c
void race_init(void) {
    s32 trackId = trackno;
    s32 numPlayers = 1;
    s32 numLaps = 3;

    /* Load track data */
    camera_init(trackId);

    /* Initialize checkpoint system */
    mesh_load(trackId, numLaps);

    /* Initialize car positions at starting grid */
    for (i = 0; i < numPlayers; i++) {
        mesh_render(i, i);  /* Player i at grid position i */
    }

    /* Reset coin collection state */
    for (i = 0; i < 16; i++) {
        track_coin_state[trackId * 16 + i] = 0;
    }

    /* Initialize audio */
    sound_stop(trackId, 0.0f);
}
```

---

## Mirror Mode and Difficulty Settings

### Mirror Mode

Mirror mode flips the track left-to-right:

```c
void mirror_mode_toggle(void) {
    /* Toggle mirror transform flag */
    mirror_transform_flag = !mirror_transform_flag;
}
```

**External Variable:** `mirror_transform_flag` at address around 0x80140000

**Arcade Comparison:** The arcade uses `gMirrorMode` global (defined in globals.h):
```c
RUSHGLOBAL S32 gMirrorMode;  /* 0=normal tracks, 1=mirror tracks */
```

The arcade stores mirrored track data separately in `TRACKS/MIRROR1`, `TRACKS/MIRROR2`, etc.

### Race Options Menu

From `race_options_menu()`:

```c
numItems = 7;  /* Mode, Laps, Difficulty, Mirror, Weather, Time, Start */

case 3:  /* Mirror */
    mirror_mode_toggle();

/* Mirror mode display */
draw_text("MIRROR:", 50, 145, (selectedItem == 3) ? 255 : 180);
```

### Difficulty Settings

**External Variables:**
- `setting_difficulty` - General difficulty setting
- `ai_difficulty` - AI opponent difficulty
- `race_difficulty` - Per-race difficulty
- `difficulty_selection` - Menu selection state

**Arcade Difficulty System:**
```c
/* From arcade globals.h */
RUSHGLOBAL S8 difficulty[MAX_TRACKS];     /* 0=easiest, 7=hardest */
RUSHGLOBAL S8 difficulty_save[MAX_TRACKS]; /* Default saved values */
RUSHGLOBAL S8 drone_diff;                  /* 0=easy, 1=medium, 2=hard, 3=real hard */
```

---

## Stunt and Battle Arenas

### Stunt Mode Setup

The `stunt_mode_setup()` function handles stunt arena selection:

```c
void stunt_mode_setup(void) {
    char *arenaNames[8] = {
        "ALCATRAZ", "METRO", "PIER 39", "GOLDEN GATE",
        "TWIN PEAKS", "DOWNTOWN", "OBSTACLE", "FREEFORM"
    };

    s32 targetScores[6] = { 10000, 25000, 50000, 100000, 250000, 0 };
    s32 timeLimits[5] = { 60, 120, 180, 300, 0 };  /* seconds, 0 = unlimited */

    /* Arena selection with left/right */
    if (input == 6) {
        selectedArena--;
        if (selectedArena < 0) selectedArena = 7;
    } else {
        selectedArena++;
        if (selectedArena > 7) selectedArena = 0;
    }

    /* Skip locked arenas */
    while (!arenaUnlocked[selectedArena]) {
        /* ... */
    }

    /* Start stunt mode */
    if (selectedOption == 4 && arenaUnlocked[selectedArena]) {
        trackno = selectedArena + 100;  /* Stunt arena IDs */
        gstate_byte = 7;  /* PREPLAY */
    }
}
```

### Battle Mode Setup

```c
void battle_mode_setup(void) {
    char *arenaNames[4];
    s32 fragLimits[4] = { 5, 10, 15, 20 };
    s32 timeLimits[4] = { 120, 180, 300, 0 };

    /* Start battle mode (requires 2+ players) */
    if (mp_player_count >= 2) {
        trackno = selectedArena + 200;  /* Battle arena IDs */
        gstate_byte = 7;  /* PREPLAY */
    }
}
```

---

## Comparison to Arcade TrackSel()

### Arcade TrackSel() Function

Location: `reference/repos/rushtherock/game/sselect.c` (and `select.c`)

```c
void TrackSel(void) {
    static S32 lastSend;
    static S16 link_state = LS_INIT;

    TrackSelForce();   /* Steering wheel force feedback */
    SelectCam();       /* Camera control */
    waitForNewbees();  /* Wait for other players */

    /* Check for initialization need */
    if (oldTrack == -1) {
        ResetGameLink();
        SetCountdownTimer(TRK_SELECT_TIME);

        /* Check current track selection */
        if (TourneyOn())
            trackno = (gTourney.track >= 0) ? gTourney.track : gTourneyTrack;
        else
            trackno = WheelSection(NTRACKS, 1);  /* Wheel position -> track */

        oldTrack = trackno;
    }

    /* If track is already locked, allow tap-through */
    if (gTrackLocked) {
        ShowTrackSelect(true);
        if (solo_flag) {
            init_car_select();
            return;
        }
        /* ... network negotiation ... */
    }

    /* Check for lock trigger (gas pressed or timer expired) */
    if ((GasPressed(true) || (edges & SW_ABORT) ||
         (GetCountdownTime() <= (ONE_SEC/2)))) {
        /* Lock track selection */
        gTrackLocked = true;
        /* ... notify network ... */
    }

    /* Allow selection changes */
    ShowTrackSelect(true);

    /* Check current track selection */
    trackno = WheelSection(NTRACKS, 1);

    waiting_for_others(1);
}
```

### Key Differences: N64 vs Arcade

| Feature | N64 | Arcade |
|---------|-----|--------|
| **Track Count** | 12 tracks | 7 tracks (NTRACKS) |
| **Input Method** | D-pad navigation | Steering wheel position |
| **Selection Lock** | A button confirm | Gas pedal or timer |
| **Network Support** | No | Yes (linked cabinets) |
| **Track Preview** | Static texture + 3D rotate | Real-time 3D scene |
| **Timer** | None | Countdown to auto-lock |
| **Tournament Mode** | No | Yes (gTourney) |
| **Force Feedback** | N/A | Steering wheel force |
| **Mirror Mode** | Toggle in options | Separate track data |

### Arcade ShowTrackSelect()

```c
void ShowTrackSelect(BOOL active) {
    /* If Track Select doesn't exist yet, create it */
    if (!TrackBlit && active) {
        InitDigitList();
        TrackBlit = NewMultiBlit(0, 0, TrackMultiBlit, NBLITS(TrackMultiBlit));
        /* Add text messages */
        InitMsgs(3, true);
        if (language != LANG_JAPANESE) {
            AddMsg(-256, SCR_B-170, "WAITING FOR OTHERS", 0, AnimateWait, 0);
            /* ... */
        }
        SetCountdownTimer(TRK_SELECT_TIME);
    }

    /* Check if getting rid of this screen */
    if (TrackBlit && !active) {
        RemoveMsgs();
        RemoveBlit(TrackBlit);
        TrackBlit = (void *)0;
    }
}
```

### Arcade Track Loading (loadTrack)

```c
BOOL loadTrack(S32 track, BOOL useHud, BOOL canAbort) {
    const char *dir = get_path(track);

    /* Load static graphics from HD */
    if (demo_game)
        Load(4);
    else
        Load(5);

    /* Load track object and texture data */
    gRoadDat = AllocFile(dir, "roaddat.rom");
    gMaxPath = AllocMem(MAXMPATH * sizeof(MPATH) * 8);
    RdFile(dir, "dpath.rom", MAXMPATH * sizeof(MPATH) * 8, (U32 *)gMaxPath);

    /* Load HUD or High Score display */
    if (useHud)
        LoadModel("HUD", false, canAbort);
    else
        LoadModel("HISCORE", false, canAbort);

    /* Load cars and track model */
    LoadCars(false, canAbort);
    MBOX_SetMinTexBase(0x290000);
    LoadModel((char *)dir, false, canAbort);

    /* Load world objects */
    RdFile(dir, "worlds.rom", WORLD_BSIZE, (U32 *)WORLD_DATA);
    LoadWorld();

    /* Initialize visual effects */
    InitVisuals(true);
    init_cp_data();

    return true;
}
```

---

## Key Source References

### N64 Source Files

| File | Functions | Description |
|------|-----------|-------------|
| `src/game/game.c` | `track_select_screen`, `menu_track_select`, `track_preview_render`, `track_info_display` | Track selection UI |
| `src/game/game.c` | `track_unlock_check`, `unlock_check`, `unlock_trigger` | Unlock system |
| `src/game/game.c` | `track_segment_load`, `track_data_decompress`, `track_collision_setup` | Track loading |
| `src/game/game.c` | `stunt_mode_setup`, `battle_mode_setup` | Arena selection |
| `src/game/game.c` | `loading_screen` | Loading progress display |
| `src/game/game.c` | `race_init`, `race_finish` | Race setup and completion |

### Key Addresses

| Address | Function/Data | Description |
|---------|---------------|-------------|
| 0x800D000C | `track_select_screen` | Main track selection menu |
| 0x800D08E4 | `track_preview_render` | Track thumbnail display |
| 0x800A1648 | `track_segment_load` | Load track segment from ROM |
| 0x800A1F18 | `track_data_decompress` | Decompress track data |
| 0x8016B000 | Track segment ROM table | ROM addresses for track data |
| 0x80158000 | Track spawn points | Starting grid positions |

### Key Global Variables

| Variable | Type | Description |
|----------|------|-------------|
| `trackno` | s32 | Currently selected/active track |
| `unlocked_tracks` | s32 | Bitmask of unlocked tracks |
| `best_race_times[12]` | u32[] | Best race time per track |
| `best_lap_times[12]` | u32[] | Best lap time per track |
| `track_par_times[]` | void*[] | Par times for bonus unlocks |
| `mirror_transform_flag` | s32 | Mirror mode enabled |
| `ghost_track_selection` | s32 | Track for ghost replay |

### Arcade Source Files

| File | Functions | Description |
|------|-----------|-------------|
| `game/sselect.c` | `TrackSel`, `ShowTrackSelect`, `TrackSelForce` | Track selection |
| `game/select.c` | `TrackSel`, `track_negotiation`, `init_car_select` | Alternative track selection |
| `game/sdnl.c` | `loadTrack`, `get_path`, `LoadCars` | Track loading |
| `game/globals.h` | Global definitions | NTRACKS, gMirrorMode, etc. |
| `game/game.h` | GState enum | Game state definitions |
| `game/scpinit.c` | InitTrack*CPs | Checkpoint initialization data |

---

## Summary

The Rush 2049 N64 track selection system is a simplified adaptation of the arcade system:

1. **12 tracks** (vs arcade's 7), organized by San Francisco neighborhoods
2. **Progressive unlock system** based on completion, placement, and time requirements
3. **D-pad navigation** replaces the arcade's steering wheel-based selection
4. **Static/rotating 3D preview** instead of full real-time rendering
5. **No network multiplayer** - eliminates the arcade's negotiation system
6. **Stunt and Battle modes** have separate arena selection systems
7. **Track data loaded from ROM** with GZIP/LZSS compression, similar to arcade's hard drive loading
