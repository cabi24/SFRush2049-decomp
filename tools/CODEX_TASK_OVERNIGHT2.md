# Codex Task: Process Overnight2 Batch Results (382 functions)

## Context
The second overnight batch (`overnight2_decompiled.txt`) contains 382 decompiled functions from the N64 game code. This completes the remaining functions not covered by overnight1 (350 functions). Ollama (qwen2.5-coder:7b) generated pseudo-C code for each function.

## Input Files
- `ollama_analysis/overnight2_decompiled.txt` - 382 functions, 26,465 lines, 723KB
- `ollama_analysis/all_functions_classified.txt` - 762 function classifications
- `symbol_addrs.us.txt` - Existing symbols (has ~723 from overnight1 + earlier batches)

## File Format
Each function in overnight2_decompiled.txt is delimited by:
```
// === func_XXXXXXXX ===
```

Example:
```c
// === func_800C3594 ===
void func_800C3594(u32 a0, u32 a1) {
    // ... decompiled C code ...
}
```

## Tasks

### 1. Extract Function List
Parse overnight2_decompiled.txt and create a list of all 382 function addresses:

```bash
grep "^// === func_" ollama_analysis/overnight2_decompiled.txt | sed 's/.*func_//' | sed 's/ ===//' > ollama_analysis/overnight2_functions.txt
```

### 2. Extract New Symbols
For each function, extract the address and create symbol entries.
Output to `ollama_analysis/overnight2_symbols.txt`:

```
func_800C3594 = 0x800C3594; // from overnight2 decompilation
func_800C3B8C = 0x800C3B8C; // from overnight2 decompilation
...
```

### 3. Check for Duplicates Against Existing Symbols
Before adding to symbol_addrs.us.txt, check which are truly new:

```bash
# Get existing function names
grep "^func_" symbol_addrs.us.txt | cut -d' ' -f1 | sort -u > /tmp/existing_funcs.txt

# Compare with overnight2
comm -23 ollama_analysis/overnight2_functions.txt /tmp/existing_funcs.txt > ollama_analysis/overnight2_new_funcs.txt
```

### 4. Extract Quality C Code
Scan through overnight2_decompiled.txt and identify functions with:
- Complete function bodies (has opening/closing braces)
- Reasonable C structure (not just error messages)
- Proper types (u32, s32, void*, etc.)

Create `ollama_analysis/overnight2_quality_c.txt` with the best examples.

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

Create `ollama_analysis/overnight2_key_functions.txt`:
```
800XXXXX: references gstate, likely state handler
800XXXXX: references car_array, likely car physics
```

### 6. Categorize by Address Range
Functions in certain address ranges likely belong to categories:
- 0x800C0000-0x800D0000: AI/Drones
- 0x800D0000-0x800E0000: Menu/UI
- 0x800E0000-0x800F0000: Audio/sound
- 0x800F0000-0x80100000: State machine/game loop
- 0x80100000-0x80110000: Additional game code

Create `ollama_analysis/overnight2_by_category.txt`:
```
# AI/Drones (0x800C0000-0x800D0000)
func_800C3594
func_800C3B8C
...

# Menu/UI (0x800D0000-0x800E0000)
func_800D0000
...

# Audio (0x800E0000-0x800F0000)
func_800E0000
...

# State machine (0x800F0000-0x80100000)
func_800FD464
...
```

### 7. Find Arcade Source Matches
Look for patterns in the decompiled code that match arcade source:

Reference files:
- `reference/repos/rushtherock/game/game.c` - game loop, state machine
- `reference/repos/rushtherock/game/cars.c` - car physics
- `reference/repos/rushtherock/game/drones.c` - AI drones
- `reference/repos/rushtherock/game/checkpoint.c` - checkpoint logic
- `reference/repos/rushtherock/game/audio.c` - audio control

Patterns to look for:
- Switch statements on gstate (game state machine)
- Loops over car arrays
- Audio/sound control patterns
- Menu state handling

Append matches to `ollama_analysis/arcade_mapping.txt`:
```
N64: func_800XXXXX -> Arcade: game/drones.c:DoDrones()
Confidence: High
Evidence: Same loop structure, references car_array
```

### 8. Update Summary
Update `ollama_analysis/summary.txt` with:
- overnight2: 382 functions decompiled
- New symbols extracted: XXX
- Quality C code examples: XXX
- Key functions identified: XXX
- Arcade matches found: XXX

## Output Files
- `ollama_analysis/overnight2_functions.txt` - List of all 382 function addresses
- `ollama_analysis/overnight2_symbols.txt` - Symbol entries for all functions
- `ollama_analysis/overnight2_new_funcs.txt` - Functions not already in symbol_addrs
- `ollama_analysis/overnight2_quality_c.txt` - Best decompiled C code examples
- `ollama_analysis/overnight2_key_functions.txt` - Functions referencing key globals
- `ollama_analysis/overnight2_by_category.txt` - Functions organized by category
- `ollama_analysis/arcade_mapping.txt` - Updated with new arcade matches
- `ollama_analysis/summary.txt` - Updated summary

## DO NOT
- Delete overnight2_decompiled.txt
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
| 800C0000-800D0000 | AI/Drones |
| 800D0000-800E0000 | Menu/UI |
| 800E0000-800F0000 | Audio |
| 800F0000-80100000 | State machine |
| 80100000-80110000 | Additional game code |

## Combined Stats After This Task
- overnight1: 350 functions
- overnight2: 382 functions
- Total: 732 functions (97% of 752 game functions)
