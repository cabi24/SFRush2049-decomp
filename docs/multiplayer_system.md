# Rush 2049 N64 Multiplayer/Split-Screen System

This document describes the multiplayer and split-screen rendering system used in San Francisco Rush 2049 for the Nintendo 64, based on analysis of the decompiled source code in `src/game/game.c` and cross-referencing with the arcade source code in `reference/repos/rushtherock/`.

## Overview

Rush 2049 supports 1-4 players in split-screen multiplayer mode. The system manages:
- Player slot allocation and state tracking
- Dynamic viewport configuration based on player count
- Per-player camera instances
- Per-player HUD rendering
- Controller input routing to the correct player
- Game state synchronization across all players

## Architecture Comparison: N64 vs Arcade

| Aspect | Arcade (Rush The Rock) | N64 (Rush 2049) |
|--------|------------------------|-----------------|
| Max Players | 8 (linked cabinets) | 4 (split-screen) |
| Player ID | `gThisNode` (network node) | Player index 0-3 |
| Screen Mode | Full screen per cabinet | Split-screen viewports |
| Networking | DIS protocol | Local only |
| Player Type | `HUMAN` (2) or `DRONE` (1) | All local human |

### Key Constants

**Arcade (globals.h):**
```c
#define MAX_LINKS   8   /* Max cars active at any time */
#define NCARS      12   /* Max cars selectable */
#define DRONE       1   /* AI-controlled car */
#define HUMAN       2   /* Human-controlled car */
```

**N64:**
- Maximum 4 local players (hardware limitation: 4 controller ports)
- Player data stride: 324 bytes (0x144) per player in main array
- Car state stride: 1088 bytes (17 sub-entries x 64 bytes) per player
- Player entries stride: 2056 bytes (0x808) per player

## Player Slot Management

### Data Structures

The N64 version uses several parallel arrays to track player state:

```
Address         Size    Description
-----------     ----    -----------
0x80152818      324*4   Main player data array (flags, position, etc.)
0x80156CF0      16*4    Player status array (active/inactive flags)
0x8015A118      76*4    Player state entries (extended state)
0x801569B8      124*4   Effect slots (join state for multiplayer setup)
```

### Key Global Variables

```c
extern s32 player_count;           /* Total configured players */
extern s32 active_player_count;    /* Players currently in-game */
extern s32 num_players_active;     /* Active for rendering */
extern s32 mp_player_count;        /* Multiplayer player count */
extern s32 split_num_players;      /* Split-screen config (2-4) */
extern s32 current_player_idx;     /* Currently processing player */
```

### Player State Management Functions

| Function | Address | Description |
|----------|---------|-------------|
| `player_state_set(idx, val)` | 0x800C9158 | Set player state byte |
| `player_mode_set(idx, mode)` | 0x800C84C0 | Set player mode (pass -1 for all) |
| `player_cleanup_slots()` | 0x800D6EF4 | Reset all player slots |
| `player_states_reset()` | 0x800D7AA0 | Clear player state entries |
| `players_frame_update()` | 0x800D5798 | Per-frame player update |
| `players_finish_check()` | 0x800D60B4 | Check if all players finished |

### Player Join Flow

The multiplayer setup uses a state machine in `effect_slots[]`:

```
State 0: Empty (waiting for controller)
State 1: Joined (player pressed START)
State 2: Ready (player confirmed)
```

```c
void player_join_screen(void) {
    for (i = 0; i < numPlayers; i++) {
        buttons = controllerInput[i] & 0xFFFF;

        if (buttons & 0x1000) {  /* START */
            if (effect_slots[i] == 0) {
                effect_slots[i] = 1;  /* Join */
            } else if (effect_slots[i] == 1) {
                effect_slots[i] = 2;  /* Ready */
            }
        }

        if (buttons & 0x4000) {  /* B */
            if (effect_slots[i] == 2) {
                effect_slots[i] = 1;  /* Unready */
            } else if (effect_slots[i] == 1) {
                effect_slots[i] = 0;  /* Leave */
            }
        }
    }

    /* Start game when 2+ players ready */
    if (joinedPlayers >= 2 && allReady) {
        split_param2++;
        if (split_param2 >= 90) {  /* 1.5 sec countdown */
            mp_player_count = joinedPlayers;
            gstate_byte = 7;  /* PREPLAY */
        }
    }
}
```

## Split-Screen Viewport Configuration

### Screen Layout Modes

The N64 renders at 320x240 resolution. Viewport configurations:

```c
extern s32 split_layout_mode;  /* 0=Horizontal, 1=Vertical, 2=Quad */
extern s32 split_toggle;       /* Handicap on/off */
extern s32 split_param1;       /* Team mode on/off */
```

### Viewport Dimensions by Player Count

#### 1 Player - Full Screen
```
+---------------------------+
|                           |
|         Player 1          |
|        320 x 240          |
|                           |
+---------------------------+
```

#### 2 Players - Horizontal Split (Default)
```
+---------------------------+
|         Player 1          |
|        320 x 120          |
+---------------------------+
|         Player 2          |
|        320 x 120          |
+---------------------------+
```

Aspect ratio: 8:3 (wider than normal 4:3)

#### 3 Players - Hybrid Layout
```
+---------------------------+
|         Player 1          |
|        320 x 120          |
+-------------+-------------+
|  Player 2   |  Player 3   |
|  160 x 120  |  160 x 120  |
+-------------+-------------+
```

Player 1 gets top half (full width), Players 2-3 share bottom half.

#### 4 Players - Quad Split
```
+-------------+-------------+
|  Player 1   |  Player 2   |
|  160 x 120  |  160 x 120  |
+-------------+-------------+
|  Player 3   |  Player 4   |
|  160 x 120  |  160 x 120  |
+-------------+-------------+
```

Aspect ratio: 4:3 (normal)

### Viewport Setup Code

```c
void camera_split_screen_setup(s32 playerCount) {
    void **cameras = (void **)0x80158000;
    s32 screenW = 320;
    s32 screenH = 240;

    switch (playerCount) {
        case 1:
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH);
            break;

        case 2:
            /* Top/bottom split */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH/2);
            camera_viewport_setup(cameras[1], 0, screenH/2, screenW, screenH/2);
            break;

        case 3:
            /* Top full, bottom split */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH/2);
            camera_viewport_setup(cameras[1], 0, screenH/2, screenW/2, screenH/2);
            camera_viewport_setup(cameras[2], screenW/2, screenH/2, screenW/2, screenH/2);
            break;

        case 4:
            /* Quad split */
            camera_viewport_setup(cameras[0], 0, 0, screenW/2, screenH/2);
            camera_viewport_setup(cameras[1], screenW/2, 0, screenW/2, screenH/2);
            camera_viewport_setup(cameras[2], 0, screenH/2, screenW/2, screenH/2);
            camera_viewport_setup(cameras[3], screenW/2, screenH/2, screenW/2, screenH/2);
            break;
    }

    /* Adjust FOV for smaller viewports */
    for (i = 0; i < playerCount; i++) {
        f32 *fov = (f32 *)((u8 *)cameras[i] + 0x40);
        *fov = (playerCount >= 2) ? 1.2f : 0.9f;  /* Wider FOV for split */
    }
}
```

### Per-View Camera Setup

```c
void camera_multi_view_setup(s32 viewIndex, void *camera) {
    f32 *aspect = (f32 *)((u8 *)camera + 0x30);
    s32 viewportX, viewportY, viewportW, viewportH;

    switch (num_players_active) {
        case 1:
            viewportX = 0; viewportY = 0;
            viewportW = 320; viewportH = 240;
            *aspect = 4.0f / 3.0f;
            break;

        case 2:
            viewportX = 0;
            viewportY = viewIndex * 120;
            viewportW = 320; viewportH = 120;
            *aspect = 8.0f / 3.0f;  /* Wider aspect */
            break;

        case 3:
        case 4:
            viewportX = (viewIndex & 1) * 160;
            viewportY = (viewIndex / 2) * 120;
            viewportW = 160; viewportH = 120;
            *aspect = 4.0f / 3.0f;
            break;
    }

    /* Store in camera structure */
    *(s32 *)((u8 *)camera + 0x44) = viewportX;
    *(s32 *)((u8 *)camera + 0x48) = viewportY;
    *(s32 *)((u8 *)camera + 0x4C) = viewportW;
    *(s32 *)((u8 *)camera + 0x50) = viewportH;
}
```

## Input Routing

### Controller Input Sources

```c
extern void *sound_handles;      /* Controller 0 (reused variable) */
extern s32 controller_input_1;   /* Controller 1 buttons */
extern s32 controller_input_2;   /* Controller 2 buttons */
extern s32 controller_input_3;   /* Controller 3 buttons */
```

### Input Reading Pattern

```c
u32 controllerInput[4];

controllerInput[0] = sound_handles;       /* P1 - cast from void* */
controllerInput[1] = controller_input_1;  /* P2 */
controllerInput[2] = controller_input_2;  /* P3 */
controllerInput[3] = controller_input_3;  /* P4 */

for (i = 0; i < numPlayers; i++) {
    s32 buttons = controllerInput[i] & 0xFFFF;
    /* Process player i's input */
}
```

### Button Mapping (from N64 controller)

| Bit | Button | Usage |
|-----|--------|-------|
| 0x1000 | START | Join/Ready/Pause |
| 0x4000 | B | Cancel/Leave |
| 0x8000 | A | Select/Confirm |

### Player Array Processing

Each player's car data is processed independently:

```c
void player_array_process(void) {
    for (i = 0; i < 4; i++) {
        u8 *playerBase = (u8 *)(0x80152818 + i * 0x144);
        u8 flags = *(playerBase + 5);

        if (flags == 0) continue;  /* Skip inactive */

        ai_update(playerBase, i);  /* Update physics/AI */
    }
}
```

## Per-Player Camera System

### Camera Array

```c
extern void *multi_view_cameras[];  /* 4 camera instances */
extern void *active_camera;         /* Currently rendering camera */
```

### Camera Structure (partial)

```
Offset  Size    Field
------  ----    -----
0x00    12      Position (x, y, z)
0x0C    12      Target (x, y, z)
0x18    12      Up vector
0x24    12      Reserved
0x30    4       Aspect ratio
0x34    4       Camera mode
0x40    4       FOV (field of view)
0x44    4       Viewport X
0x48    4       Viewport Y
0x4C    4       Viewport W
0x50    4       Viewport H
0x70    4       Camera state
0x74    4       State timer
```

### Camera Modes

```c
switch (gstate_byte) {
    case 0:  /* ATTRACT */
        camera_mode = 0;  /* Default/cinematic */
        break;
    case 5:  /* COUNTDOWN */
        camera_mode = 4;  /* Cinematic starting grid */
        break;
    case 6:  /* PLAYGAME */
        camera_mode = 2;  /* Third person chase */
        break;
    case 7:  /* ENDGAME */
        camera_mode = 5;  /* Victory camera */
        break;
}
```

### Split-Screen Configuration

```c
void camera_split_screen_config(s32 numPlayers) {
    if (numPlayers < 1) numPlayers = 1;
    if (numPlayers > 4) numPlayers = 4;

    num_players_active = numPlayers;

    for (i = 0; i < numPlayers; i++) {
        if (multi_view_cameras[i] != NULL) {
            camera_multi_view_setup(i, multi_view_cameras[i]);
        }
    }
}
```

## Per-Player HUD

### HUD Setup Function

```c
extern void hud_setup(s32 x, s32 y, s32 w, s32 h,
                      s32 flags, f32 scale, f32 distance, s32 mode);
extern void hud_init(void);
extern void hud_speed_display(void *hud, f32 speed);
```

### HUD Configuration by Mode

| Mode | X | Y | W | H | Flags | Description |
|------|---|---|---|---|-------|-------------|
| Normal | 32 | 16 | 16 | 5 | 64 | Standard race HUD |
| Compact | 8 | 8 | 0 | 0 | 0 | Minimal HUD for split |

### HUD Elements

The HUD displays per-player:
- Speedometer
- Lap counter
- Position indicator
- Race time
- Split time comparisons (vs best)

### Split Time Display

```c
extern void *split_display_active[];  /* Active split indicators */
extern s32 split_time_frames[4];      /* Frame when split was set */

/* Update split times per player */
for (i = 0; i < 4; i++) {
    if (split_display_active[i] > 0) {
        s32 splitAge = currentFrame - split_time_frames[i];
        if (splitAge > 180) {  /* 3 seconds */
            split_display_active[i] = 0;  /* Clear */
        }
    }
}
```

## Race State Synchronization

### Player Race State Arrays

```c
extern void *player_status[];       /* Race position (1st, 2nd, etc.) */
extern void *player_lap_count[];    /* Current lap number */
extern void *player_checkpoint[];   /* Last checkpoint passed */
extern void *player_race_time[];    /* Individual race time */
extern void *player_finished_flag[];/* Finished race flag */
extern void *player_respawn_count[];/* Number of respawns */
```

### Finish Check

```c
s32 players_finish_check(void) {
    s32 finished_count = 0;

    for (i = 0; i < active_car_count; i++) {
        u8 *entry = player_entries + i * 2056;
        s8 status = *(s8*)(entry + 0x7C9);

        if (status != 0) {
            finished_count++;
        }
    }

    players_finished_count = finished_count;
    return (finished_count >= active_car_count) ? 1 : 0;
}
```

### Per-Player Coordinate Updates

For split-screen with 2+ players, coordinate transforms are synchronized:

```c
void player_coords_update(void) {
    pre_update();

    if (game_unlock_id != 2 || player_count < 2) {
        goto done;
    }

    /* Update each player's coordinate data */
    for (i = 1; i < player_count; i++) {
        audio_stream_update(srcBase[123], src, dst);
        src += 952;  /* Stride per player */
        dst += 952;
    }

done:
    post_update(0);
}
```

## Multiplayer Setup Menu

### Configuration Variables

```c
extern u32 split_screen_config[32]; /* Menu state/selections */
extern s32 split_num_players;       /* 2-4 players */
extern s32 split_layout_mode;       /* 0=Horiz, 1=Vert, 2=Quad */
extern s32 split_toggle;            /* Handicap on/off */
extern s32 split_param1;            /* Team mode on/off */
extern s32 split_param2;            /* Start countdown */
```

### Menu Options

```c
void multiplayer_setup(void) {
    /* Menu items */
    case 0:  /* Player count (2-4) */
        if (numPlayers < 2) numPlayers = 4;
        if (numPlayers > 4) numPlayers = 2;
        break;

    case 1:  /* Split screen mode */
        split_layout_mode = (split_layout_mode + delta + 3) % 3;
        /* "HORIZONTAL", "VERTICAL", "QUAD" */
        break;

    case 2:  /* Handicap */
        split_toggle = !split_toggle;
        break;

    case 3:  /* Team mode */
        split_param1 = !split_param1;
        break;

    case 4:  /* Continue to player join */
        menu_transition(20);
        break;

    case 5:  /* Back */
        menu_back();
        break;
}
```

## Sound Per-Player

### Car Sound Management

Each player has 17 sound slots (64 bytes each = 1088 bytes total):

```c
void car_sounds_clear(void) {
    /* Player index from register s4 (non-standard) */
    u8 *car_state = car_state_base + (player_idx * 17 * 64);

    for (offset = 0; offset < 1088; offset += 64) {
        s32 handle = *(s32*)(car_state + offset + 4);

        if (handle != -1) {
            gfx_flush((s16)handle, 0, 0);  /* Stop sound */
            *(s32*)(car_state + offset + 4) = -1;
        }
    }

    player_state_flags[player_idx] = 0;
}
```

### Global Sound Cleanup

```c
void all_player_sounds_clear(void) {
    for (i = 0; i < 4; i++) {
        player_flags_1[i] = 0;
        player_data_ptr[i] = 0;
        menu_main();  /* Clear player i's sounds */
    }

    /* Stop global sounds */
    if (global_sound_handle != NULL) {
        /* ... */
    }
}
```

## Battle/Stunt Mode Extensions

### Battle Mode Variables

```c
extern s32 player_battle_scores[4];  /* Kill scores */
extern s32 player_lives[4];          /* Remaining lives */
extern s32 player_frags[];           /* Frag count */
extern s32 player_deaths[];          /* Death count */
```

### Stunt Mode Variables

```c
extern s32 player_stunt_scores[4];   /* Stunt points */
extern s32 player_score_array[];     /* Current combo score */
extern s32 player_combo[];           /* Combo count */
extern s32 player_multiplier[];      /* Score multiplier */
```

## Memory Layout Summary

### Player Data Base Addresses

| Address | Size | Description |
|---------|------|-------------|
| 0x80152818 | 0x510 | Main player array (4 x 324 bytes) |
| 0x80156CF0 | 0x40 | Player status flags (4 x 16 bytes) |
| 0x80158000 | 0x100 | Camera array (4 cameras) |
| 0x8015A118 | 0x130 | Extended player state (4 x 76 bytes) |
| 0x801569B8 | 0x1F0 | Effect/join slots (4 x 124 bytes) |

## Comparison with Arcade Source

The arcade version (Rush The Rock) uses a fundamentally different approach:

1. **Network-based**: Each cabinet runs independently with network sync
2. **Single viewport**: Full screen per player (no split)
3. **`gThisNode`**: Identifies which cabinet "this" player is
4. **DIS Protocol**: Distributed Interactive Simulation for sync

The N64 adapts this to local multiplayer by:
- Replacing network sync with direct array access
- Adding viewport management for split-screen
- Simplifying player identification to array index
- Rendering multiple views per frame

## Related Files

- `src/game/game.c` - Main game loop and multiplayer logic
- `src/game/camera.c` - Camera system (referenced, not analyzed)
- `reference/repos/rushtherock/game/game.c` - Arcade game loop
- `reference/repos/rushtherock/game/camera.c` - Arcade camera system
- `reference/repos/rushtherock/game/hud.c` - Arcade HUD system
- `reference/repos/rushtherock/game/globals.h` - Arcade constants
