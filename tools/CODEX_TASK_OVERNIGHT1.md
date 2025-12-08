# Codex Task: Process Overnight1 Batch Results (350 functions)

## Context
The first overnight batch (`overnight_decompiled.txt`) contains 350 decompiled functions from the N64 game code. This is approximately half of the 752 total functions. Ollama (qwen2.5-coder:7b) generated pseudo-C code for each function.

## Input Files
- `ollama_analysis/overnight_decompiled.txt` - 350 functions, 12,481 lines, 305KB
- `ollama_analysis/all_functions_classified.txt` - 762 function classifications (if available)
- `symbol_addrs.us.txt` - Existing symbols (already has ~373 from earlier batches)

## File Format
Each function in overnight_decompiled.txt is delimited by:
```
// === func_XXXXXXXX ===
```

Example:
```c
// === func_80087068 ===
void func_80087068(u32 v0, u32 a0) {
    // ... decompiled C code ...
}
```

## Tasks

### 1. Extract Function List
Parse overnight_decompiled.txt and create a list of all 350 function addresses:

```bash
grep "^// === func_" ollama_analysis/overnight_decompiled.txt | sed 's/.*func_//' | sed 's/ ===//' > ollama_analysis/overnight1_functions.txt
```

### 2. Extract New Symbols
For each function, extract the address and create symbol entries.
Output to `ollama_analysis/overnight1_symbols.txt`:

```
func_80087068 = 0x80087068; // from overnight1 decompilation
func_80087118 = 0x80087118; // from overnight1 decompilation
...
```

### 3. Check for Duplicates Against Existing Symbols
Before adding to symbol_addrs.us.txt, check which are truly new:

```bash
# Get existing function names
grep "^func_" symbol_addrs.us.txt | cut -d' ' -f1 | sort -u > /tmp/existing_funcs.txt

# Compare with overnight1
comm -23 ollama_analysis/overnight1_functions.txt /tmp/existing_funcs.txt > ollama_analysis/overnight1_new_funcs.txt
```

### 4. Extract Quality C Code
Scan through overnight_decompiled.txt and identify functions with:
- Complete function bodies (has opening/closing braces)
- Reasonable C structure (not just error messages)
- Proper types (u32, s32, void*, etc.)

Create `ollama_analysis/overnight1_quality_c.txt` with the best examples.

Quality indicators:
- Has proper function signature
- Uses N64 types (u32, s32, f32, etc.)
- References known globals (0x80152818, 0x801146EC, etc.)
- Has control flow (if/else, loops, switch)

### 5. Identify Key Functions
Look for functions that reference known important addresses:
- `0x801146EC` - gstate (game state)
- `0x80152818` - car_array
- `0x801461D0` - game_struct
- `0x80142AFC` - frame_counter

Create `ollama_analysis/overnight1_key_functions.txt`:
```
800XXXXX: references gstate, likely state handler
800XXXXX: references car_array, likely car physics
```

### 6. Categorize by Address Range
Functions in certain address ranges likely belong to categories:
- 0x80086A50-0x80090000: Low-level/utility functions
- 0x80090000-0x800A0000: Rendering/graphics functions
- 0x800A0000-0x800B0000: Game logic
- 0x800B0000-0x800C0000: Physics/collision
- 0x800C0000-0x800D0000: AI/drones
- 0x800D0000-0x800E0000: Menu/UI
- 0x800E0000-0x800F0000: Audio/sound
- 0x800F0000-0x80100000: State machine/game loop

Create `ollama_analysis/overnight1_by_category.txt`:
```
# Graphics (0x80090000-0x800A0000)
80099BFC: render function (10KB)
...

# Physics (0x800B0000-0x800C0000)
800B37E8: audio control
...
```

### 7. Find Arcade Source Matches
Look for patterns in the decompiled code that match arcade source:

Reference files:
- `reference/repos/rushtherock/game/game.c` - game loop, state machine
- `reference/repos/rushtherock/game/cars.c` - car physics
- `reference/repos/rushtherock/game/drones.c` - AI drones
- `reference/repos/rushtherock/game/checkpoint.c` - checkpoint logic
- `reference/repos/rushtherock/game/camera.c` - camera control

Patterns to look for:
- Switch statements on gstate (game state machine)
- Loops over car arrays
- Collision detection (bounding sphere/box tests)
- Display list commands (gSP*, gDP*)

Append matches to `ollama_analysis/arcade_mapping.txt`:
```
N64: func_800XXXXX -> Arcade: game/cars.c:UpdateCar()
Confidence: High
Evidence: Same loop structure, references car_array
```

### 8. Update Summary
Update `ollama_analysis/summary.txt` with:
- overnight1: 350 functions decompiled
- New symbols extracted: XXX
- Quality C code examples: XXX
- Key functions identified: XXX
- Arcade matches found: XXX

## Output Files
- `ollama_analysis/overnight1_functions.txt` - List of all 350 function addresses
- `ollama_analysis/overnight1_symbols.txt` - Symbol entries for all functions
- `ollama_analysis/overnight1_new_funcs.txt` - Functions not already in symbol_addrs
- `ollama_analysis/overnight1_quality_c.txt` - Best decompiled C code examples
- `ollama_analysis/overnight1_key_functions.txt` - Functions referencing key globals
- `ollama_analysis/overnight1_by_category.txt` - Functions organized by category
- `ollama_analysis/arcade_mapping.txt` - Updated with new arcade matches
- `ollama_analysis/summary.txt` - Updated summary

## DO NOT
- Delete overnight_decompiled.txt
- Push to git (human will review)
- Modify C source files in src/
- Add duplicate symbols to symbol_addrs.us.txt

## Known Globals (for reference)
| Address | Name | Purpose |
|---------|------|---------|
| 0x801146EC | gstate | Game state byte |
| 0x80152818 | car_array | Car state array |
| 0x801461D0 | game_struct | Main game structure |
| 0x80142AFC | frame_counter | Frame count |
| 0x801174B4 | secondary_state | Secondary state var |

## Address Ranges (for categorization)
| Range | Likely Category |
|-------|-----------------|
| 80086A50-80090000 | Utility/init |
| 80090000-800A0000 | Graphics |
| 800A0000-800B0000 | Game logic |
| 800B0000-800C0000 | Physics |
| 800C0000-800D0000 | AI/Drones |
| 800D0000-800E0000 | Menu/UI |
| 800E0000-800F0000 | Audio |
| 800F0000-80100000 | State machine |
