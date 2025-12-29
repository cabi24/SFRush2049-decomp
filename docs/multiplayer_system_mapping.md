# Rush 2049 N64 Multiplayer/Split-Screen System

## Overview

This document analyzes the multiplayer and split-screen system for San Francisco Rush 2049 on N64, comparing it against the arcade source code from Rush The Rock.

## Architecture Comparison

### Arcade (Rush The Rock)

The arcade version uses a **networked multiplayer** architecture:

| Component | Description |
|-----------|-------------|
| `MAX_LINKS` | 8 players maximum (linked cabinets) |
| `gLink[MAX_LINKS]` | Array tracking linked players |
| `this_node` | Local player slot number |
| `gThisNode` | Current cabinet's node ID |
| `num_humans` | Count of human players |
| `num_drones` | Count of AI drones |
| `humans[MAX_LINKS]` | Slot list for human players |
| `drones[MAX_LINKS]` | Slot list for AI players |

The arcade uses network PDUs (Protocol Data Units) for inter-cabinet communication:
- `Event_CanPlay` - Node available to link
- `Event_CanJoin` - Node available with track/car info
- `Event_Joined` - Player joined the race
- `Event_GameOver` - Race ended

### N64 (Rush 2049)

The N64 version uses a **local split-screen** architecture:

| Component | Address/File | Description |
|-----------|--------------|-------------|
| `mp_player_count` | 0x80159BC0 | Active human players |
| `split_num_players` | 0x80159BA8 | Split-screen player count |
| `split_layout_mode` | 0x80159BAC | Screen layout (H/V/Quad) |
| `split_screen_config[]` | 0x80159B78 | Configuration data |
| `player_count` | 0x8015A100 | Total active players |
| `active_player_count` | 0x80117248 | Players in current race |

Key files:
- `src/game/multiplayer.c` - Main multiplayer system
- `src/game/display.c` - Viewport management
- `include/game/multiplayer.h` - Structure definitions

---

## Player Count Handling

### Maximum Players

| Platform | Max Human | Max Total | Notes |
|----------|-----------|-----------|-------|
| Arcade   | 8         | 8         | Via cabinet linking |
| N64      | 4         | 8         | 4 human + 4 AI |

### N64 Player State Machine

```
PLAYER_STATE_INACTIVE (0) --> Controller detected
           |
           v
PLAYER_STATE_WAITING (1)  --> Press START
           |
           v
PLAYER_STATE_READY (2)    --> All ready + countdown
           |
           v
PLAYER_STATE_RACING (3)   --> Cross finish line
           |
           v
PLAYER_STATE_FINISHED (4) or PLAYER_STATE_DNF (5)
```

### Join Screen Flow

From `player_join_screen()` in game.c:

```c
/* Check each player slot */
for (i = 0; i < numPlayers; i++) {
    /* Check for Start button to join */
    if (buttons & 0x1000) {
        if (effect_slots[i] == 0) {
            effect_slots[i] = 1;  /* Player joining */
        } else if (effect_slots[i] == 1) {
            effect_slots[i] = 2;  /* Player ready */
        }
    }
}

/* Start game when all required players ready */
if (joinedPlayers >= 2 && allReady) {
    split_param2++;  /* Countdown timer */
    if (split_param2 >= 90) {  /* 1.5 seconds at 60fps */
        mp_player_count = joinedPlayers;
        gstate_byte = 7;  /* PREPLAY state */
    }
}
```

---

## Split-Screen Viewport Setup

### Viewport Layouts

Defined in `multiplayer.c`:

```c
static const s16 sViewportLayouts[NUM_MP_MODES][MP_MAX_PLAYERS][4] = {
    /* Single player - full screen */
    { {0, 0, 320, 240}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} },

    /* 2 player horizontal - top/bottom split */
    { {0, 0, 320, 120},
      {0, 120, 320, 120},
      {0,0,0,0}, {0,0,0,0} },

    /* 2 player vertical - left/right split */
    { {0, 0, 160, 240},
      {160, 0, 160, 240},
      {0,0,0,0}, {0,0,0,0} },

    /* 4 player quad */
    { {0, 0, 160, 120},
      {160, 0, 160, 120},
      {0, 160, 160, 120},
      {160, 120, 160, 120} }
};
```

### Camera Viewport Setup

From `camera_split_screen_setup()` in game.c (func_800C1CBC):

```c
void camera_split_screen_setup(s32 playerCount) {
    void **cameras = (void **)0x80158000;
    s32 screenW = 320, screenH = 240;

    switch (playerCount) {
        case 1:
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH);
            break;

        case 2:
            /* Top/bottom split */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH / 2);
            camera_viewport_setup(cameras[1], 0, screenH / 2, screenW, screenH / 2);
            break;

        case 3:
            /* Top full, bottom split in half */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH / 2);
            camera_viewport_setup(cameras[1], 0, screenH / 2, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[2], screenW / 2, screenH / 2, screenW / 2, screenH / 2);
            break;

        case 4:
            /* Quad split */
            camera_viewport_setup(cameras[0], 0, 0, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[1], screenW / 2, 0, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[2], 0, screenH / 2, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[3], screenW / 2, screenH / 2, screenW / 2, screenH / 2);
            break;
    }

    /* Adjust FOV for smaller viewports */
    for (i = 0; i < playerCount; i++) {
        f32 *fov = (f32 *)((u8 *)cameras[i] + 0x40);
        *fov = (playerCount >= 2) ? 1.2f : 0.9f;
    }
}
```

### RDP Viewport Commands

From `camera_viewport_setup()` (func_800C17F4):

```c
void camera_viewport_setup(void *camera, s32 x, s32 y, s32 w, s32 h) {
    /* Store viewport in camera structure */
    *(s32 *)((u8 *)camera + 0x60) = x;
    *(s32 *)((u8 *)camera + 0x64) = y;
    *(s32 *)((u8 *)camera + 0x68) = w;
    *(s32 *)((u8 *)camera + 0x6C) = h;

    /* Update aspect ratio */
    *(f32 *)((u8 *)camera + 0x44) = (f32)w / (f32)h;

    /* Set up RDP viewport (G_MOVEMEM) */
    s16 *vpData = (s16 *)0x80155000;
    vpData[0] = (w * 2) << 1;       /* vscale x */
    vpData[1] = (h * 2) << 1;       /* vscale y */
    vpData[4] = ((x * 2) + w) << 1; /* vtrans x */
    vpData[5] = ((y * 2) + h) << 1; /* vtrans y */

    /* gDPSetScissor */
    dl->words.w0 = 0xED000000 | ((x << 2) << 12) | (y << 2);
    dl->words.w1 = (((x + w) << 2) << 12) | ((y + h) << 2);
}
```

---

## Per-Player State Management

### N64 Player Structure (MPPlayer)

```c
typedef struct MPPlayer {
    /* Identity */
    u8      active;         /* Player slot active */
    u8      human;          /* Human (1) or AI (0) */
    u8      player_index;   /* Player number (0-3) */
    u8      car_index;      /* Car slot in race */

    /* Controller */
    s8      controller;     /* Controller index (-1 if AI) */
    u8      join_state;     /* JOIN_STATE_* */
    u8      state;          /* PLAYER_STATE_* */
    u8      ready;          /* Ready to start */

    /* Viewport */
    Viewport viewport;      /* Player's viewport bounds */

    /* Car selection */
    u8      car_type;       /* Selected car */
    u8      car_color;      /* Car color index */
    u8      transmission;   /* Auto/manual */

    /* Race state */
    s32     position;       /* Race position (1st, 2nd, etc.) */
    s32     lap;            /* Current lap */
    u32     race_time;      /* Total race time */
    u32     best_lap_time;  /* Best lap */
    s32     checkpoint;     /* Last checkpoint passed */

    /* Statistics */
    s32     wins;           /* Session wins */
    s32     points;         /* Session points */
} MPPlayer;
```

### Arcade Player Structure (LinkInfo)

```c
typedef struct LinkInfo {
    U8  heartbeat;  /* Network heartbeat */
    U8  slot;       /* Current pole position */
    U8  flags;      /* Drones, catchup, active, blink flags */
    /* ... additional networking fields ... */
} LinkInfo;
```

### Key Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Player data | `gLink[8]` + `model[8]` | `gMultiplayer.players[4]` |
| Car data | `game_car[12]` shared | Per-player car structures |
| Position tracking | Network sync'd | Local calculation |
| Viewport | Full screen per cabinet | Split viewports |

---

## Input Routing

### N64 Controller Assignment

```c
typedef struct ControllerSlot {
    u8      connected;      /* Controller connected */
    u8      player;         /* Assigned player index */
    u8      type;           /* Controller type */
    u8      pak_type;       /* Accessory (rumble/memory) */
} ControllerSlot;
```

Controller detection and assignment:

```c
void mp_detect_controllers(void) {
    for (i = 0; i < 4; i++) {
        gMultiplayer.controllers[i].connected = input_is_connected(i);
    }
}

s32 mp_add_player(s32 controller) {
    if (!gMultiplayer.controllers[controller].connected) {
        return -1;
    }

    /* Find free player slot */
    for (i = 0; i < 4; i++) {
        if (!gMultiplayer.players[i].active) {
            gMultiplayer.players[i].controller = controller;
            gMultiplayer.controllers[controller].player = i;
            return i;
        }
    }
    return -1;
}
```

### Arcade Input

Arcade uses hardware I/O mapped to specific player:
- Steering wheel, gas, brake, clutch per cabinet
- `gGasVal`, `gBrakeVal`, `gWheelVal` globals
- `this_node` determines which slot receives input

---

## Camera Per Player

### N64 Implementation

Each player has an independent camera stored in a camera array:

```c
void **cameras = (void **)0x80158000;  /* Camera array base */

/* Camera structure offsets */
+0x00: Position (x, y, z)
+0x0C: Target (x, y, z)
+0x24: Up vector (x, y, z)
+0x40: FOV
+0x44: Aspect ratio
+0x60: Viewport X
+0x64: Viewport Y
+0x68: Viewport Width
+0x6C: Viewport Height
```

Per-player camera update (from game loop):
```c
for (i = 0; i < active_player_count; i++) {
    /* Update camera for this player */
    camera_update(cameras[i], cars[i]);

    /* Apply viewport */
    mp_apply_viewport(i);

    /* Render scene from this player's perspective */
    render_scene(cameras[i]);
}
```

### Arcade Camera

Single camera following `this_node`:
- `gCamPos[3]` - Camera world position
- `gCamUvs[3][3]` - Camera orientation matrix
- View modes 1-10 (worm, driver, hover, death, circle, fixed, etc.)

```c
void setcamview(void) {
    CAR_DATA *car = &game_car[this_node];

    /* Copy car position to camera reference */
    for (i = 0; i < 3; i++) {
        gCamPos[i] = car->dr_pos[i];
        for (j = 0; j < 3; j++)
            gCamUvs[i][j] = car->dr_uvs[j][i];
    }

    switch (view) {
        case 1: /* Worm's eye */ break;
        case 2: /* Driver's eye */ break;
        case 3: /* Hover eye (default) */ break;
        case 4: /* Death cam */ break;
        /* ... etc ... */
    }

    MBOX_UpdateCamera(CAMLINK0, gCamPos, gCamUvs);
}
```

---

## Arcade vs N64 Differences Summary

| Feature | Arcade (Rush The Rock) | N64 (Rush 2049) |
|---------|------------------------|-----------------|
| **Max Players** | 8 (linked cabinets) | 4 (split-screen) |
| **Multiplayer Type** | Networked | Local |
| **Screen Mode** | Full screen per cabinet | Split viewports |
| **Controller** | Physical cab controls | N64 controllers (4) |
| **Synchronization** | Network PDUs | Shared memory |
| **Camera** | Single, view modes | Per-player cameras |
| **AI Drones** | Shared across network | Local only |
| **State Machine** | 10 states (ATTRACT-HISCORE) | Similar but local |

---

## Key N64 Functions

| Function | Address | Purpose |
|----------|---------|---------|
| `mp_init` | - | Initialize multiplayer system |
| `mp_setup_viewports` | - | Configure split-screen layout |
| `mp_add_player` | - | Add player to session |
| `camera_split_screen_setup` | 0x800C1CBC | Setup camera viewports |
| `camera_viewport_setup` | 0x800C17F4 | Configure single viewport |
| `camera_split_screen_config` | 0x800C2960 | Configure split mode |
| `multiplayer_setup` | 0x800D2E94 | Multiplayer options menu |
| `player_join_screen` | 0x800D349C | Player join handling |
| `viewport_setup` | 0x80001B44 | Low-level viewport setup |
| `display_update` | 0x800015F0 | Display list viewport updates |

---

## Global Variables

| Variable | Address | Type | Description |
|----------|---------|------|-------------|
| `mp_player_count` | 0x80159BC0 | s32 | Human players in game |
| `split_num_players` | 0x80159BA8 | s32 | Players in split mode |
| `split_layout_mode` | 0x80159BAC | s32 | Layout (0=H, 1=V, 2=Quad) |
| `split_toggle` | 0x80159BB0 | s32 | Handicap mode |
| `split_param1` | 0x80159BB4 | s32 | Team mode |
| `active_player_count` | 0x80117248 | s32 | Active racers |
| `player_count` | 0x8015A100 | s32 | Total players |
| `gMultiplayer` | - | struct | Main multiplayer state |

---

## Notes

1. **3-Player Mode**: The N64 uses an asymmetric layout with player 1 getting a full-width top half, while players 2-3 share the bottom half.

2. **FOV Adjustment**: Split-screen modes automatically increase FOV from 0.9 to 1.2 to compensate for reduced viewport size.

3. **Render Order**: Players are rendered in sequence, with viewport/scissor updated between each to prevent bleed.

4. **Memory Constraints**: N64 split-screen reduces texture quality and draw distance to maintain framerate with multiple viewports.

5. **AI in Multiplayer**: When AI opponents are added, they share the car state arrays but do not have viewports or controllers.
