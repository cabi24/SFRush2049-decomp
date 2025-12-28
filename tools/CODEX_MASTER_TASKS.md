# Codex Master Task List

## Overview
These tasks involve deep analysis of the arcade source code (`reference/repos/rushtherock/`) to inform the N64 decompilation. Each task produces documentation in `docs/` that will guide stub function implementation.

## Task Files

| File | Focus Area | Priority | Est. Output |
|------|------------|----------|-------------|
| `CODEX_PHYSICS_ANALYSIS.md` | Tire model, drivetrain, collision | HIGH | 4 docs |
| `CODEX_AI_ANALYSIS.md` | Drone AI, pathfinding, rubber-band | HIGH | 4 docs |
| `CODEX_AUDIO_ANALYSIS.md` | Engine sound, positional audio, music | MEDIUM | 5 docs |
| `CODEX_GAMESTATE_CAMERA.md` | State machine, camera modes | MEDIUM | 6 docs |
| `CODEX_STRUCTS_STUBS.md` | Data structures, stub implementations | HIGH | 10 docs |

## Execution Order

### Phase 1: Foundation (Do First)
1. **CODEX_STRUCTS_STUBS.md - Task 1**: Document MODELDAT structure
   - This is the core data structure used everywhere
   - Understanding it unlocks all other analysis

2. **CODEX_STRUCTS_STUBS.md - Task 4**: Global variable mapping
   - Map D_80XXXXXX to meaningful names
   - Critical for understanding game.c code

### Phase 2: Core Systems
3. **CODEX_PHYSICS_ANALYSIS.md**: Full physics analysis
   - Tire model is central to gameplay
   - Collision affects all game modes

4. **CODEX_AI_ANALYSIS.md**: AI/Drone system
   - Maxpath is complex but well-documented in arcade
   - Rubber-banding is key to gameplay balance

### Phase 3: Polish Systems
5. **CODEX_GAMESTATE_CAMERA.md**: State machine and camera
   - State machine is simpler, good for validation
   - Camera affects player experience

6. **CODEX_AUDIO_ANALYSIS.md**: Audio system
   - Lower priority but well-documented in arcade
   - Engine sound is satisfying to get right

### Phase 4: Implementation
7. **CODEX_STRUCTS_STUBS.md - Tasks 5-8**: Stub implementations
   - Use all previous analysis
   - Produce actionable code

## Key Arcade Files Reference

### Most Important
| File | Lines | Content |
|------|-------|---------|
| game/game.c | 1,687 | Main game loop, state machine |
| game/maxpath.c | 3,730 | AI pathfinding |
| game/drones.c | 24K | Drone AI |
| game/tires.c | 2,500 | Tire physics |
| game/drivsym.c | 20K | Physics integration |
| game/camera.c | 40K | Camera system |
| game/carsnd.c | 33K | Car audio |

### Header Files
| File | Content |
|------|---------|
| game/modeldat.h | MODELDAT, CAR_DATA structures |
| game/tiredes.h | Tire parameters |
| game/globals.h | Global variables, enums |

## Output Location

All documentation should be created in:
```
docs/
├── physics_tire_mapping.md
├── physics_drivetrain_mapping.md
├── physics_collision_mapping.md
├── physics_integration_mapping.md
├── ai_maxpath_mapping.md
├── ai_drone_behavior_mapping.md
├── ai_racing_tactics_mapping.md
├── ai_data_structures.md
├── audio_engine_mapping.md
├── audio_positional_mapping.md
├── audio_music_mapping.md
├── audio_sfx_catalog.md
├── audio_architecture.md
├── gamestate_mapping.md
├── gamestate_attract.md
├── gamestate_race_flow.md
├── camera_modes_mapping.md
├── camera_math_mapping.md
├── camera_replay_mapping.md
├── struct_modeldat.md
├── struct_car_data.md
├── struct_tiredes.md
├── globals_mapping.md
├── stubs_physics.md
├── stubs_ai.md
├── stubs_audio.md
├── stubs_camera.md
├── memory_map.md
└── size_comparison.md
```

## Quality Standards

Each document should include:
1. **Summary**: 2-3 sentence overview
2. **Mapping Table**: Arcade → N64 function/struct mapping
3. **Code Examples**: Relevant arcade code snippets
4. **N64 Adaptation Notes**: What changes for N64
5. **Confidence Ratings**: High/Medium/Low for each mapping

## Verification

After completing each document:
1. Cross-reference function addresses with `symbol_addrs.us.txt`
2. Verify arcade code snippets compile conceptually
3. Check that N64 function names match game.c
4. Ensure no duplicate mappings across documents

## Time Estimate

- Phase 1: ~2 hours
- Phase 2: ~4 hours
- Phase 3: ~3 hours
- Phase 4: ~3 hours
- **Total**: ~12 hours of analysis work

## Getting Started

```bash
# Navigate to project
cd /home/cburnes/projects/rush2049-decomp

# Open first task
cat tools/CODEX_STRUCTS_STUBS.md

# Start with MODELDAT analysis
cat reference/repos/rushtherock/game/modeldat.h | head -200
```
