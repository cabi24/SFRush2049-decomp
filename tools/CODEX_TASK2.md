# Codex Task 2: Merge Symbols and Process New Batches

## Previous Work Done
Codex already processed:
- `all_functions_classified.txt` - 762 function classifications
- `priority_decompiled.txt` - 9 core game functions
- `batch2_decompiled.txt` - 13 physics/collision functions
- Created `cleaned_symbols.txt` with ~100 meaningful symbol names
- Created `extracted_c_code.txt` with cleaned C snippets

## New Input Files
- `ollama_analysis/batch3_decompiled.txt` - 14 state handler/camera functions (NEW)
- `ollama_analysis/batch4_decompiled.txt` - 14 AI/drone/checkpoint functions (errors - server was down)
- `ollama_analysis/overnight_decompiled.txt` - Will contain ~350 functions when complete

## Tasks

### 1. Merge Cleaned Symbols into symbol_addrs.us.txt
The file `ollama_analysis/cleaned_symbols.txt` contains ~100 new function symbols.
Merge them into `symbol_addrs.us.txt`:

```bash
# Append new symbols (avoiding duplicates)
cat ollama_analysis/cleaned_symbols.txt >> symbol_addrs.us.txt
# Then manually dedupe by address
```

Format should match existing entries:
```
func_XXXXXXXX = 0xXXXXXXXX; // description
```

### 2. Process batch3_decompiled.txt
Extract any valid C code and update `extracted_c_code.txt` with batch3 functions:
- State handlers (GSTATE_INIT, SETUP, MENU, CAMERA, READY, PLAY, CARS, FINISH)
- Camera functions (camera_view_transform, camera_update_nested, camera_race_setup)
- UpdateActiveObjects, state_dispatch, vi_retrace_callback

### 3. Create Arcade Function Mapping
Create `ollama_analysis/arcade_mapping.txt` mapping N64 functions to arcade source:

```
# N64 Function -> Arcade Equivalent
# Format: N64_addr N64_name -> arcade_file:function_name confidence

800FD464 game_loop -> game/game.c:game HIGH
800CA3B4 game_update -> game/game.c:playgame MEDIUM
800DB81C attract_mode -> game/game.c:attract HIGH
800C85F0 checkpoint_check -> game/checkpoint.c:CheckCPs HIGH
800C9AE0 input_handler -> game/game.c:ProcessPDUs MEDIUM
800B37E8 audio_control -> game/audio.c:sound_control HIGH
80097CA0 camera_transform -> game/camera.c:CameraUpdate MEDIUM
800BAE24 physics_calc -> game/cars.c:Update_MDrive MEDIUM
```

Reference arcade source at: `reference/repos/rushtherock/`
Key arcade files:
- `game/game.c` - Main game loop and state machine
- `game/checkpoint.c` - Checkpoint system
- `game/cars.c` - Vehicle physics
- `game/drones.c` - AI opponents
- `game/camera.c` - Camera system
- `game/visuals.c` - Rendering

### 4. Create Game State Enum Header
Create `include/game/gstate.h` based on analysis:

```c
#ifndef GSTATE_H
#define GSTATE_H

// Game states (bitmask flags used in N64 version)
#define GSTATE_INIT     0x00040000
#define GSTATE_SETUP    0x00080000
#define GSTATE_MENU     0x00100000
#define GSTATE_CAMERA   0x00200000
#define GSTATE_READY    0x00400000
#define GSTATE_PLAY     0x00800000
#define GSTATE_CARS     0x01000000
#define GSTATE_FINISH   0x02000000

// Arcade-style enum (for reference)
typedef enum {
    ATTRACT,
    TRKSEL,     // Track select
    CARSEL,     // Car select
    PREPLAY,
    COUNTDOWN,
    PREPLAY2,
    PLAYGAME,
    ENDGAME,
    GAMEOVER,
    HISCORE
} GState;

// Key globals
extern u32 gstate;           // 0x801146EC
extern void* car_array;      // 0x80152818
extern void* game_struct;    // 0x801461D0
extern u32 frame_counter;    // 0x80142AFC

#endif
```

### 5. Summary Report
Update `ollama_analysis/summary.txt` with:
- Total functions identified
- Functions with C code extracted
- Functions mapped to arcade source
- Remaining functions needing analysis

## Output Files
- `symbol_addrs.us.txt` - Updated with merged symbols
- `ollama_analysis/arcade_mapping.txt` - N64 to arcade mapping
- `include/game/gstate.h` - Game state definitions
- `ollama_analysis/summary.txt` - Updated summary

## DO NOT
- Delete original Ollama output files
- Push to git (human will review)
- Modify existing C source files in src/

## Notes
- batch4_decompiled.txt has errors (server was offline) - skip for now
- overnight_decompiled.txt may still be generating - process what's available
- When in doubt about a mapping, mark as LOW confidence
