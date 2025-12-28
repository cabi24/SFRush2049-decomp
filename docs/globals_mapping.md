# N64 Global Variables Mapping

This document maps the N64 global variables (D_80XXXXXX) to meaningful names based on their usage patterns and comparison with the arcade source code.

## Reference Sources
- N64 decompiled source: `src/game/game.c`
- Arcade source: `reference/repos/rushtherock/game/globals.h`, `game.h`, `modeldat.h`

---

## Core Game State Variables

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_801146EC | s8 | gstate_byte | Simple game state (0-11) | 37 | `gstate` (GState enum) |
| D_801174B4 | u32 | gstate_mask | Game state bitmask for state machine | 27 | N/A (N64-specific) |
| D_801174B8 | u32 | gstate_prev | Previous/requested game state | - | N/A |
| D_80142AFC | u32 | frame_counter | Global frame counter | 42 | `IRQTIME` |
| D_801461D0 | s32 | game_sync | Main game sync/DMA structure | 27 | `gModeldatLock` |

### Game State Values (D_801146EC / gstate_byte)
Based on arcade `game.h` GState enum:
```c
enum GState {
    0 = ATTRACT,    // Attract mode / title screen
    1 = TRKSEL,     // Track selection
    2 = CARSEL,     // Car selection
    3 = PLAYGAME,   // Active racing
    4 = ENDGAME,    // Race finished
    5 = GAMEOVER,   // Game over screen (also used for COUNTDOWN)
    6 = HISCORE,    // High score entry
    7 = PREPLAY,    // Pre-race setup
    8 = PREPLAY2,   // Final pre-race setup
    9 = COUNTDOWN,  // Countdown before race (also HISCORE)
    10 = BATTLE,    // Battle mode (N64-specific)
    11 = STUNT      // Stunt mode (N64-specific)
};
```

---

## Player/Car Data

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80152818 | u8[] | game_car | Car/player state array (stride 0x400) | 29 | `game_car[NCARS]` (CAR_DATA) |
| D_80152770 | u8[] | model_sync | Model data sync structure | 19 | N/A |
| D_80142728 | u8[] | object_sync | Object sync structure | 42 | N/A |

### Car Data Structure Offsets (D_80152818)
Based on arcade `modeldat.h` CAR_DATA struct:
| Offset | Type | Field | Description |
|--------|------|-------|-------------|
| 0x00 | f32[3] | dr_pos | Dead reckoned position |
| 0x0C | f32[3] | dr_vel | Dead reckoned velocity |
| 0x18 | f32[3][3] | dr_uvs | Dead reckoned orientation |
| 0x24 | f32[3] | position | World position (used in game.c) |
| 0x1E0 | s8 | shortcut_flag | Shortcut taken flag |
| 0x359 | s8 | status | Player status byte |

---

## Graphics/Display

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80149438 | Gfx** | gfx_dl_ptr | Current display list pointer | 64 | N/A (N64-specific) |
| D_80149430 | Gfx* | gfx_dl_base | Display list base address | - | N/A |
| D_80149440 | OSTask | gfx_task | Graphics RSP task | - | N/A |
| D_801491F0 | void** | active_object_list | Active object linked list head | 13 | N/A |
| D_801492C8 | void** | render_list | Render object list | - | N/A |

---

## Camera System

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80170020 | void* | active_camera | Current active camera pointer | 18 | N/A |
| D_80158000 | f32[3] | camera_pos | Camera world position | 17 | `gCamPos[3]` |
| D_80159058 | f32[16][6] | camera_keyframes | Camera script keyframes | 21 | N/A |
| D_80170000 | f32 | shake_intensity | Camera shake intensity | - | N/A |
| D_80170004 | f32 | shake_timer | Camera shake timer | - | N/A |
| D_80170008 | f32 | shake_decay | Camera shake decay rate | - | N/A |

---

## Track/Race Data

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80159A08 | s32 | trackno | Current track number | 23 | `trackno` |
| D_80159A0C | s32 | car_selection | Selected car index | - | N/A |
| D_80159A04 | s32 | game_mode | Game mode (0=race, 1=time, 2=stunt, 3=battle) | - | N/A |
| D_80159A10 | u32[12] | best_race_times | Best race time per track | 20 | N/A |
| D_80159D10 | u32[12] | best_lap_times | Best lap time per track | 16 | N/A |
| D_8015A004 | s32 | race_start_frame | Frame when race started | - | N/A |

---

## Timer System

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80159B00 | u64[8][6] | timer_data | Timer array (8 timers) | 36 | N/A |

### Timer Structure (D_80159B00[id])
| Index | Type | Field | Description |
|-------|------|-------|-------------|
| [0] | u64 | running | 0=stopped, 1=running, 2=paused |
| [1] | u64 | start_time | Start time (osGetTime) |
| [2] | u64 | elapsed | Elapsed time |
| [3-5] | u64 | reserved | Reserved/extra fields |

---

## Random Number Generation

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80143500 | u32 | random_seed | Main PRNG seed (LCG) | 27 | N/A |
| D_80159A20 | s32 | random_seed2 | Secondary random seed | - | N/A |

### PRNG Algorithm
```c
seed = seed * 1103515245 + 12345;  // Standard LCG
```

---

## Audio System

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80158100 | void*[] | sound_handles | Sound channel handles | 49 | N/A |
| D_80158120 | void*[] | sound_volumes | Sound channel volumes | - | N/A |
| D_80158140 | s32 | voice_handle | Current voice sound handle | - | N/A |
| D_80159300 | s32 | sfx_volume | SFX volume setting (0-100) | 10 | N/A |
| D_80159304 | s32 | music_volume | Music volume setting (0-100) | 10 | N/A |
| D_80160080 | f32[16] | channel_volumes | Audio channel volumes | - | N/A |
| D_801600C0 | f32[16] | channel_pans | Audio channel pans | - | N/A |
| D_80160100 | f32[16] | channel_pitches | Audio channel pitches | - | N/A |
| D_80160180 | f32[3] | listener_pos | Audio listener position | - | N/A |
| D_80160300 | s32 | reverb_type | Current reverb type | - | N/A |
| D_80160304 | f32 | reverb_mix | Reverb wet/dry mix | - | N/A |

### Audio Streaming
| Address | Type | Proposed Name | Purpose |
|---------|------|---------------|---------|
| D_80160700 | u32 | stream_rom_offset | Stream ROM offset |
| D_80160704 | u32 | stream_total_size | Stream total size |
| D_80160708 | u32 | stream_bytes_read | Stream bytes read |
| D_8016070C | void*[2] | stream_buffers | Stream double buffers |
| D_80160714 | s32 | stream_active_buf | Active buffer index |
| D_80160718 | s32 | stream_state | Stream state |

---

## Menu System

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80159210 | s32 | menu_selection | Current menu selection index | 13 | N/A |
| D_80159214 | s32 | menu_item_count | Number of menu items | 12 | N/A |
| D_80159218 | s32 | menu_items_ptr | Pointer to menu items array | 13 | N/A |
| D_80159330 | void*[] | control_mapping | Controller button mapping | 13 | N/A |
| D_80159400 | void*[] | main_menu_items | Main menu items array | - | N/A |
| D_80159480 | s32 | pause_menu_items | Pause menu items | - | N/A |
| D_80159500 | s32 | options_menu_items | Options menu items | - | N/A |

### Controller Mapping (D_80159330)
| Index | Button | Default Action |
|-------|--------|----------------|
| [0] | A | Accelerate |
| [1] | B | Brake |
| [2] | Z | Handbrake |
| [3] | R | Look back |
| [4] | L | Change view |

---

## Multiplayer/Network

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80159600 | s32 | net_host_state | Network hosting state (0=off, 1=host) | 10 | N/A |
| D_80159604 | s32 | session_id | Multiplayer session ID | 11 | N/A |
| D_8015960C | void*[] | player_slots | Player slot status array | 11 | N/A |
| D_8015A108 | s32 | player_count | Number of active players | 11 | `num_humans` |
| D_8015A110 | s32 | game_unlock_id | Current game unlock ID | 10 | N/A |
| D_8015A118 | s32 | save_queue_flag | Save data queue flag | 11 | N/A |

---

## Leaderboard/High Scores

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80158300 | u8[12][10][16] | leaderboard | Leaderboard data [track][rank][data] | - | N/A |
| D_80158FC0 | s32 | leaderboard_track | Current leaderboard track | - | N/A |
| D_80159E10 | s32 | hiscore_char_pos | High score character position | - | N/A |

---

## Weather/Effects

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80157604 | f32 | weather_intensity | Weather effect intensity (0-1) | - | N/A |
| D_80157608 | f32 | weather_target | Target weather intensity | - | N/A |
| D_80159C20 | void*[] | effect_slots | Effect slot status | 12 | N/A |

---

## Object Management

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_801597F0 | u8* | current_object | Current object pointer | 22 | N/A |
| D_80159B60 | u8 | mode_byte_1 | Mode byte value 1 | - | N/A |
| D_80159B70 | s32 | mode_byte_2 | Mode byte value 2 | - | N/A |

---

## Save Data / Progress

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_80159200 | s32 | buttons_state | Current button state / save progress | 10 | `edges`, `levels` |
| D_80159204 | s32 | repeat_counter | Button repeat counter / unlocks | 10 | N/A |

---

## Score System

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_8014B00C | s32[4] | player_scores | Player score array | - | N/A |
| D_8014B01C | s32[4] | player_lives | Player lives array | - | N/A |
| D_8014C00C | s32[4] | stunt_scores | Player stunt score array | - | N/A |
| D_80143404 | f32 | player_distance | Player distance traveled | - | `distance` (in CAR_DATA) |

---

## Misc Flags/State

| Address | Type | Proposed Name | Purpose | Refs | Arcade Equivalent |
|---------|------|---------------|---------|------|-------------------|
| D_801170FC | s32 | render_enable | Render enable flag | - | N/A |
| D_80159F54 | s32 | pause_start_frame | Frame when pause started | - | N/A |
| D_80035470 | s8 | mode_flag_3 | Mode flag 3 | - | N/A |
| D_80035471 | s8 | mode_flag_2 | Mode flag 2 | - | N/A |
| D_80035472 | s8 | mode_flag_1 | Mode flag 1 | - | N/A |

---

## Memory Regions

Based on address ranges:
| Range | Purpose |
|-------|---------|
| 0x80020000-0x80086A50 | Static data/BSS |
| 0x80086A50-0x80124AF0 | Decompressed game code |
| 0x80140000-0x80160000 | Game state variables |
| 0x80160000-0x80170000 | Audio system |
| 0x80170000+ | Runtime data (camera, effects) |

---

## Arcade Source Cross-Reference

### Direct Equivalents
| N64 Symbol | Arcade Symbol | File |
|------------|---------------|------|
| D_801146EC (gstate_byte) | gstate | globals.h |
| D_80142AFC (frame_counter) | IRQTIME | globals.h |
| D_80159A08 (trackno) | trackno | globals.h |
| D_80152818 (game_car) | game_car[NCARS] | globals.h |
| D_80158000 (camera_pos) | gCamPos[3] | globals.h |
| D_8015A108 (player_count) | num_humans | globals.h |
| D_80159200 (buttons_state) | edges, levels | globals.h |

### Structural Equivalents
| N64 Array | Arcade Struct | Notes |
|-----------|---------------|-------|
| D_80152818 | CAR_DATA | 952 bytes per car vs arcade 0x3B8 |
| (unknown) | MODELDAT | Full physics model data |
| (unknown) | RECKON | Dead reckoning data |
| (unknown) | MPCTL | Maxpath control data |

---

## Usage Statistics

Top 40 globals by reference count in game.c:
| Rank | Address | Refs | Proposed Name |
|------|---------|------|---------------|
| 1 | D_80149438 | 64 | gfx_dl_ptr |
| 2 | D_80158100 | 49 | sound_handles |
| 3 | D_80142AFC | 42 | frame_counter |
| 4 | D_80142728 | 42 | object_sync |
| 5 | D_801146EC | 37 | gstate_byte |
| 6 | D_80159B00 | 36 | timer_data |
| 7 | D_80152818 | 29 | game_car |
| 8 | D_801461D0 | 27 | game_sync |
| 9 | D_80143500 | 27 | random_seed |
| 10 | D_801174B4 | 27 | gstate_mask |
| 11 | D_80159A08 | 23 | trackno |
| 12 | D_801597F0 | 22 | current_object |
| 13 | D_80159058 | 21 | camera_keyframes |
| 14 | D_80159A10 | 20 | best_race_times |
| 15 | D_80152770 | 19 | model_sync |
| 16 | D_80170020 | 18 | active_camera |
| 17 | D_80158000 | 17 | camera_pos |
| 18 | D_80159D10 | 16 | best_lap_times |
| 19 | D_80159A20 | 16 | random_seed2 |
| 20 | D_801491F0 | 13 | active_object_list |

---

## Recommended Symbol File Updates

Add these to `symbol_addrs.us.txt`:
```
// Core game state
gstate_byte = 0x801146EC; // type:data s8 - simple game state enum (0-11)
gstate_mask = 0x801174B4; // type:data u32 - game state bitmask
gstate_prev = 0x801174B8; // type:data u32 - previous game state
frame_counter = 0x80142AFC; // type:data u32 - global frame counter

// Car/player data
game_car = 0x80152818; // type:data - car state array (stride 0x3B8)
model_sync = 0x80152770; // type:data - model data sync structure
object_sync = 0x80142728; // type:data - object sync structure
game_sync = 0x801461D0; // type:data - main game sync structure

// Graphics
gfx_dl_ptr = 0x80149438; // type:data Gfx** - display list pointer
gfx_dl_base = 0x80149430; // type:data Gfx* - display list base
active_object_list = 0x801491F0; // type:data void** - active objects head

// Camera
active_camera = 0x80170020; // type:data void* - current camera pointer
camera_pos = 0x80158000; // type:data f32[3] - camera position
camera_keyframes = 0x80159058; // type:data f32[16][6] - camera script data
shake_intensity = 0x80170000; // type:data f32 - camera shake intensity

// Track/race
trackno = 0x80159A08; // type:data s32 - current track number
best_race_times = 0x80159A10; // type:data u32[12] - best race times
best_lap_times = 0x80159D10; // type:data u32[12] - best lap times
race_start_frame = 0x8015A004; // type:data s32 - race start frame

// Timer system
timer_data = 0x80159B00; // type:data u64[8][6] - timer array

// Random
random_seed = 0x80143500; // type:data u32 - PRNG seed (LCG)
random_seed2 = 0x80159A20; // type:data s32 - secondary random seed

// Audio
sound_handles = 0x80158100; // type:data void*[] - sound channel handles
sfx_volume = 0x80159300; // type:data s32 - SFX volume 0-100
music_volume = 0x80159304; // type:data s32 - music volume 0-100
listener_pos = 0x80160180; // type:data f32[3] - audio listener position

// Menu
menu_selection = 0x80159210; // type:data s32 - current menu selection
menu_item_count = 0x80159214; // type:data s32 - menu item count
menu_items_ptr = 0x80159218; // type:data s32 - menu items pointer
control_mapping = 0x80159330; // type:data void*[] - controller mapping

// Multiplayer
player_count = 0x8015A108; // type:data s32 - number of active players
net_host_state = 0x80159600; // type:data s32 - network host state
session_id = 0x80159604; // type:data s32 - multiplayer session ID

// Scores
player_scores = 0x8014B00C; // type:data s32[4] - player scores
stunt_scores = 0x8014C00C; // type:data s32[4] - stunt scores

// Leaderboard
leaderboard = 0x80158300; // type:data u8[12][10][16] - leaderboard data
```

---

*Document generated: 2025-12-28*
*Based on analysis of src/game/game.c and arcade source reference/repos/rushtherock/*
