# Codex Task 3: Process Overnight2 Batch Results

## Context
The second overnight batch (`overnight2_decompiled.txt`) contains ~382 additional decompiled functions.
Combined with overnight_decompiled.txt (350 functions), this completes all 752 game functions.

## Input Files
- `ollama_analysis/overnight2_decompiled.txt` - 382 functions (NEW - just completed)
- `ollama_analysis/overnight_decompiled.txt` - 350 functions (from first batch)
- `ollama_analysis/all_functions_classified.txt` - 762 function classifications

## Tasks

### 1. Extract Symbols from overnight2_decompiled.txt
Parse the overnight2 output and extract function symbols:

```bash
# Run the parser on the new batch
python3 tools/parse_ollama_output.py extract-symbols ollama_analysis/overnight2_decompiled.txt > ollama_analysis/overnight2_symbols.txt
```

Expected format:
```
func_XXXXXXXX = 0xXXXXXXXX; // description_from_analysis
```

### 2. Merge All Symbols
Combine all extracted symbols into a master list:
- `ollama_analysis/cleaned_symbols.txt` (existing ~100 symbols)
- `ollama_analysis/overnight2_symbols.txt` (new from overnight2)

Create `ollama_analysis/all_symbols_merged.txt` with deduplication by address.

### 3. Update symbol_addrs.us.txt
Append new, non-duplicate symbols to `symbol_addrs.us.txt`.
Format must match:
```
func_800XXXXX = 0x800XXXXX; // description
```

Check for duplicates before adding:
```bash
# Only add symbols not already in symbol_addrs.us.txt
grep -v "^#" symbol_addrs.us.txt | cut -d= -f1 | sort -u > /tmp/existing.txt
# Compare against new symbols
```

### 4. Extract Quality C Code
From overnight2_decompiled.txt, extract any functions with clean C code:
- Look for complete function bodies (not just stubs)
- Functions with proper structure: return type, parameters, body
- Skip error responses or incomplete outputs

Append to `ollama_analysis/extracted_c_code.txt`.

### 5. Categorize Functions
Using the classified output, create category files:
- `ollama_analysis/functions_graphics.txt` - rendering, display list functions
- `ollama_analysis/functions_physics.txt` - collision, physics integration
- `ollama_analysis/functions_audio.txt` - sound, music functions
- `ollama_analysis/functions_ai.txt` - drone, pathfinding functions
- `ollama_analysis/functions_game.txt` - state machine, game logic

Format:
```
# Category: graphics
# Count: 132 functions
800XXXXX: brief_description
800XXXXX: brief_description
```

### 6. Update Summary
Update `ollama_analysis/summary.txt` with final counts:
- Total functions: 752
- Functions decompiled: XXX (from overnight1 + overnight2)
- Functions with clean C: XXX
- Functions by category: graphics/physics/audio/ai/game
- Symbols extracted: XXX

### 7. Create Arcade Mapping Report
Extend `ollama_analysis/arcade_mapping.txt` with any new mappings found.
Reference files:
- `reference/repos/rushtherock/game/game.c`
- `reference/repos/rushtherock/game/cars.c`
- `reference/repos/rushtherock/game/drones.c`
- `reference/repos/rushtherock/game/checkpoint.c`

## Output Files
- `ollama_analysis/overnight2_symbols.txt` - NEW symbols from overnight2
- `ollama_analysis/all_symbols_merged.txt` - Combined deduplicated symbols
- `ollama_analysis/functions_*.txt` - Categorized function lists
- `ollama_analysis/summary.txt` - Updated summary
- `symbol_addrs.us.txt` - Updated with new symbols (careful with duplicates!)

## DO NOT
- Delete any original Ollama output files
- Push to git (human will review)
- Modify existing C source files in src/

## Notes
- overnight2_decompiled.txt may have ~45KB partial file if still running
- Check for ERROR entries (network issues) and skip those
- Quality varies - some outputs are pseudo-C, others are clean
- When in doubt about a function name, use func_XXXXXXXX format
