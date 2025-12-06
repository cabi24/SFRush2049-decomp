# Claude Code Project Memory

This file helps Claude maintain context across sessions for the Rush 2049 N64 decompilation project.

## Project Overview

**Goal**: Decompile San Francisco Rush 2049 (N64) into matching C source code.

**Key Advantage**: We have the Rush The Rock arcade source code at `reference/repos/rushtherock/` which shares significant code with the N64 version.

## Current Status

**Phase**: 1 - ROM Analysis (just started)
**Last Updated**: 2025-12-06

### Completed
- [x] Project planning via Spec Kit (constitution, spec, plan, tasks)
- [x] Cloned reference repos (rushtherock, sm64, mk64, perfect_dark, banjo-kazooie)
- [x] Created lessons-learned.md from decomp research
- [x] Set up agent definitions in .claude/agents/
- [x] Verified ROM (US, 12MB)
- [x] Created project structure and README
- [x] Converted ROM from V64 to Z64 format
- [x] Configured splat (splat.us.yaml)
- [x] Initial ROM disassembly (~18K lines MIPS assembly)
- [x] Splat found 80+ potential file boundaries

### In Progress
- [ ] Segment ROM into individual code files
- [ ] Identify libultra functions
- [ ] Match functions to arcade source

### Not Started
- [ ] Function decompilation
- [ ] Build system refinement (IDO compiler)

## Key Files

| File | Purpose |
|------|---------|
| `baserom.us.z64` | Original ROM in Z64 format (not in git) |
| `us.sha1` | ROM hash for verification |
| `splat.us.yaml` | Splat configuration for ROM extraction |
| `asm/us/1050.s` | Main disassembled code (~18K lines) |
| `rush2049.us.ld` | Generated linker script |
| `symbol_addrs.us.txt` | Discovered symbols |
| `.specify/specs/plan.md` | Implementation plan with phases |
| `.specify/specs/tasks.md` | Detailed task breakdown (144 tasks) |
| `reference/lessons-learned.md` | Best practices from other decomps |
| `reference/repos/rushtherock/` | **ARCADE SOURCE** - primary reference |

## Architecture Notes

### Arcade Source Structure (rushtherock/)
The arcade source is organized as:
- `game/` - Main game code (~97K lines)
  - `game.c` - Game loop and state machine
  - `cars.c`, `tires.c`, `collision.c` - Physics
  - `drones.c`, `maxpath.c` - AI
  - `checkpoint.c` - Race logic
  - `camera.c` - Camera system
- `GUTS/` - Arcade system libraries (3dfx)
- `LIB/` - Utility libraries
- `MB/` - Mathbox (physics coprocessor)
- `OS/` - Operating system

### Three-Tier Portability
1. **Portable** (use as-is): game.c, checkpoint.c, drones.c, maxpath.c
2. **Adaptation** (modify): cars.c, tires.c, math functions
3. **Rewrite** (N64-specific): rendering, audio, I/O

### ROM Info
- Size: 12,582,912 bytes (12 MB)
- Format: Z64 (big-endian, converted from V64)
- SHA-1: 3f99351d7bb61656614bdb2aa1a90cfe55d1922c
- Internal name: "uRhs2 40 9"
- Cartridge ID: UR
- Graphics microcode: F3DEX2 (assumed)

## Build Commands (When Ready)

```bash
# Non-matching build (faster iteration)
make VERSION=us NON_MATCHING=1 -j$(nproc)

# Matching build
make VERSION=us -j$(nproc)

# Check progress
make progress

# Verify ROM hash
make test

# Diff a function
./diff.py function_name -m -w
```

## Agent Roles

See `.claude/agents/` for specialized agents:
- **asm-analyzer** - MIPS assembly analysis
- **c-writer** - Write matching C code
- **arcade-comparator** - Find arcade equivalents
- **project-r-analyst** - Study Project R insights
- **n64-decomp-expert** - Apply lessons from other decomps
- **build-runner** - Build and verify

## Development Environment

- **Primary**: Raspberry Pi (current, for light work)
- **Faster machine**: To be set up for heavy compilation
- **Spec Kit**: Installed via `uv tool install specify-cli`

## Next Steps (When Resuming)

1. Update splat.us.yaml with file boundaries from suggestions
2. Re-run splat to split into individual .s files
3. Search for string references to identify functions
4. Start matching functions to arcade source
5. Begin with simple/recognizable functions first

## Quick Reference

### Arcade Game States (game.h)
```c
enum GState {
    ATTRACT, TRKSEL, CARSEL, PLAYGAME,
    ENDGAME, GAMEOVER, HISCORE, PREPLAY,
    PREPLAY2, COUNTDOWN
};
```

### Key Arcade Files to Reference
| N64 Module | Arcade Equivalent | LOC |
|------------|-------------------|-----|
| src/game/game.c | game/game.c | 1,687 |
| src/racing/checkpoint.c | game/checkpoint.c | 1,694 |
| src/racing/cars.c | game/cars.c | 1,892 |
| src/racing/drones.c | game/drones.c | ~24K |
| src/racing/maxpath.c | game/maxpath.c | 3,730 |
| src/camera/camera.c | game/camera.c | 2,043 |

## Session Continuity Tips

When starting a new session:
1. Read this file first
2. Check `.specify/specs/tasks.md` for current phase
3. Check git log for recent changes
4. Run `make progress` (when available) to see status
